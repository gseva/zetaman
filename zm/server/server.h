#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "player.h"
#include "../game_protocol.h"
class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();
	void jump();
	GameState getState();
private:
	GameState state_;
	//vector<Player> players;
	ServerProxy& serverProxy;
	Player player;
};

#endif
