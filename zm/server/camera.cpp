#include "camera.h"
#include <vector>
#define XMAX 47
#define XMIN 0
#define INITIAL_X 4
#define INITIAL_Y 0

Camera::Camera(std::vector<Player*>& players) : players(players){
  position.x = INITIAL_X;
  position.y = INITIAL_Y;
}

Camera::~Camera(){}

bool Camera::canMoveRight(Player* player){
  bool canMove = true;
  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    int dist = player->getPosition().x - (*iPlayer)->getPosition().x;
    canMove &= dist < 10;
  }
  return canMove;
}

bool Camera::canMoveLeft(Player* player){
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
  if ( xo > XMAX )
    xo = XMAX;
  if ( xo < XMIN )
    xo = XMIN;

  zm::proto::Position newPos;
  newPos.x=xo;
  newPos.y=yo;
  return newPos;
}

zm::proto::Position Camera::calculatePosition(){
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
