#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "zm/game_protocol.h"
#include "zm/server/player.h"
#include <Box2D/Box2D.h>
class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();
	void jump();
	GameState getState();
private:
	Player player;
  b2World* world;
  b2Vec2* gravity;

  b2BodyDef groundBodyDef;
  b2Body* groundBody;
};

#endif
