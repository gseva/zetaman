#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "zm/server/config.h"
#include "zm/server/server.h"
#include "zm/server/player.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "build/maps/"
#define GAME_STEP_FREQUENCY 1000/60


Server::Server(const char* port) : accepter_(NULL), port_(port) {
}

Server::~Server() {
  if (accepter_)
    delete accepter_;
}

void Server::run() {
  accepter_ = new zm::Socket();
  accepter_->bindAndListen(port_);

  zm::Game game(*this);

  game.acceptHost(accepter_);
  game.acceptPlayers(accepter_);
  while (true) {
    game.startLevel();
    game.gameLoop();
  }
}

void Server::stopAccepting() {
  accepter_->close();
}



namespace zm {

Game::Game(Server& s) : server_(s), accepting_(false),  playing_(false),
    currentLevel(NULL) {
  mapPath_ = "";
}

void Game::acceptHost(zm::Socket* accepter) {
  auto hostSock = std::make_shared<zm::ProtectedSocket>(accepter->accept());
  newPlayer(hostSock);
  zm::proto::ServerEvent event(zm::proto::connectedAsHost);
  std::string ev = event.serialize();
  std::cout << "Envio evento: " << ev << std::endl;
  hostSock->write(ev);
}


void Game::acceptPlayers(zm::Socket* accepter) {
  accepting_ = true;
  while (accepting_) {
    auto playerSock = accepter->accept();
    if (playerSock == NULL) break;
    std::cout << "Acepto un jugador!" << std::endl;
    zm::proto::ServerEvent event(zm::proto::connected);
    std::string ev = event.serialize();
    std::cout << "Envio evento: " << ev << std::endl;
    playerSock->write(ev);
    newPlayer(std::make_shared<zm::ProtectedSocket>(playerSock));
  }
}

void Game::newPlayer(std::shared_ptr<zm::ProtectedSocket> sock) {
  // Envio los nombres de los jugadores conectados hasta el momento
  for (auto&& pair : players) {
    proto::ServerEvent ev(proto::ServerEventType::playerConnected);
    ev.payload = pair.second->name;
    sock->write(ev.serialize());
  }

  std::string name = "player " + std::to_string(players.size());

  // Escribo el nombre del nuevo jugador a los jugadores conectados
  for (auto&& pair : proxies) {
    proto::ServerEvent ev(proto::ServerEventType::playerConnected);
    ev.payload = name;
    pair.second->getSocket()->write(ev.serialize());
  }

  Player* player = new Player(*this, name, !players.size());
  players.insert({name, player});
  zm::ClientProxy* cp = new ClientProxy(player, sock);
  proxies.insert({name, cp});
  cp->startListening();
}

void Game::selectLevel(int level) {
  std::cout << "Selecciono nivel " << level << std::endl;
  switch (level) {
    case 0: mapPath_ = "level_1.json"; break;
    case 1: mapPath_ = "level_2.json"; break;
    case 2: mapPath_ = "level_3.json"; break;
    case 3: mapPath_ = "level_4.json"; break;
    case 4: mapPath_ = "level_5.json"; break;
  }
  mapPath_ = DEFAULT_PATH + mapPath_;

  if (accepting_) {
    accepting_ = false;
    server_.stopAccepting();
  } else {
    cond.signal();
  }
}

void Game::notifyPlayerReady() {
  Lock l(m_);
  bool notify = true;
  for (auto&& pair : players) {
    if (!pair.second->isReady) {
      notify = false;
      break;
    }
  }
  if (notify) {
    cond.signal();
  }
}

void Game::startLevel() {
  std::cout << "Mi map path es " << mapPath_ << std::endl;
  if (mapPath_ == "") {
    cond.wait();
  }

  JsonSerializer js;
  char curPath[255];
  getcwd(curPath, 255);
  std::string path(curPath);
  path += "/" + mapPath_;
  std::cout << "Importo path " << path << std::endl;
  currentMap_ = js.importMap(path);

  std::string map = currentMap_.getReducedString();

  std::cout << "Escribo mapa" << std::endl;
  for (auto&& pair : proxies) {
    pair.second->sendMap(map);
  }
  std::cout << "starteo game" << std::endl;
  for (auto&& pair : proxies) {
    pair.second->startGame();
  }

  // Espero a que todos los jugadores envien readyToPlay
  cond.wait();

  if (currentLevel)
    delete currentLevel;

  std::vector<Player*> playersVec;
  for (auto pair: players)
      playersVec.push_back(pair.second);

  std::cout << "Creo new level! " << path << std::endl;
  currentLevel = new Level(playersVec, currentMap_);
  mapPath_ = "";
}

void Game::gameLoop() {
  playing_ = true;

  while (playing_) {
    currentLevel->step();

    std::this_thread::sleep_for(
         std::chrono::milliseconds(zm::config::gamePeriod));

    updateState();
  }

  finishLevel();
}

void Game::finishLevel() {
  if (currentLevel->state == zm::proto::won) {
    std::cout << "Win!" << std::endl;
    for (auto&& pair : proxies) {
      pair.second->sendLevelWon();
    }
  } else {
    std::cout << "Lose!" << std::endl;
  }
}


void Game::updateState() {
  zm::proto::Game game = currentLevel->getState();
  if (game.state != zm::proto::playing) {
    playing_ = false;
  }
  for (auto&& pair : proxies) {
    pair.second->updateState(game);
  }
}

void Game::shutdown(const std::string& name) {
  Player* p = players[name];

  if (p->isHost) {
    for (auto it = proxies.cbegin(); it != proxies.cend();)  {
      proxies.erase(it++);
      (*it).second->shutdown();
    }
  } else {
    proxies.erase(name);
    proxies[name]->shutdown();
  }
}

Game::~Game() {
  for (auto item : players) {
    delete item.second;
  }

  for (auto item : proxies) {
    delete item.second;
  }

  if (currentLevel)
    delete currentLevel;
}

} // zm

