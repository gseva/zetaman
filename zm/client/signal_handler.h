#ifndef __ZM_CLIENT_SIGNAL_HANDLER_H__
#define __ZM_CLIENT_SIGNAL_HANDLER_H__


#include <sigc++/sigc++.h>

#include "zm/game_protocol.h"

class GameUpdateHandler {
public:
  typedef sigc::signal<void, zm::proto::Game> type_signal_game_update;
  type_signal_game_update signal_game_update();

protected:
  type_signal_game_update gameUpdateSignal;
};

#endif
