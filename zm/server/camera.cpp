
#include <vector>

#include "zm/server/camera.h"
#include "zm/server/physics/players.h"
#include "zm/json/jsonserializer.h"

#define INITIAL_X 4
#define INITIAL_Y 0
#define ALTO_TOTAL 12
#define ANCHO_PANTALLA 16
#define DELTA 0.5

Camera::Camera(std::vector<Player*>& players, const JsonMap& jm) : 
players(players) {
  position.x = INITIAL_X;
  position.y = INITIAL_Y;

  bossPosition.x = jm.imageNumbers.size()/ALTO_TOTAL - ANCHO_PANTALLA - 1;
  bossPosition.y = 0;
  xMin=0;
  xMax = jm.imageNumbers.size()/ALTO_TOTAL - ANCHO_PANTALLA - 1;
}

Camera::~Camera(){}

bool Camera::canMoveRight(Player* player){
  if ( areInBossFrame() )
    return player->getPosition().x < position.x + ANCHO_PANTALLA -1  - DELTA;
  bool canMove = true;
  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    int dist = player->getPosition().x - (*iPlayer)->getPosition().x;
    canMove &= dist < 10;
  }
  return canMove;
}

bool Camera::canMoveLeft(Player* player){
 if ( areInBossFrame() )
  return player->getPosition().x > position.x + DELTA;
 bool canMove = true;
  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    int dist = player->getPosition().x - (*iPlayer)->getPosition().x;
    canMove &= dist > -10;
  }
  return canMove;
}

bool Camera::isInFrame(Body* body){
  int x = body->getPosition().x;
  int center = position.x + 8;
  int dist = x - center;
  return (-8 < dist) && (dist < 8);
}

bool Camera::areInBossFrame(){
  return position.x >= bossPosition.x;
}

bool Camera::isInCenter(PlayerBody* playerBody){
  int x = playerBody->getPosition().x;
  int center = position.x + 8;
  int dist = x - center;
  return (-5 < dist) && (dist < 5);
}

zm::proto::Position Camera::getPosition(){
  int xmin, xmax, xo;
  int yo = 0;

  xmin = (*(players.begin)())->getPosition().x;
  xmax = xmin;

  for ( std::vector<Player*>::iterator player = players.begin();
    player != players.end(); ++player ) {
    if ( ((*player)->getPosition().x ) < xmin )
      xmin = (*player)->getPosition().x;
    if ( ((*player)->getPosition().x ) > xmax )
      xmax = (*player)->getPosition().x;
  }
  xo = (xmax + xmin) / 2 - 8;
  if ( xo > xMax )
    xo = xMax;
  if ( xo < xMin )
    xo = xMin;

  zm::proto::Position newPos;
  newPos.x = xo;
  newPos.y = yo;
  return newPos;
}

zm::proto::Position Camera::calculatePosition(){
  if (bossPosition.x - 8 <= position.x && 
    bossPosition.y == position.y ){
    position = bossPosition;
    return position;
}

  bool areInCenter = true;
  std::vector<Player*>::iterator iPlayer;

  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    areInCenter &= isInCenter((*iPlayer)->body);
  }

  if ( !areInCenter )
    position = getPosition();

  // si todos estan cerca del centro no mueve la camara.
  return position;
}
