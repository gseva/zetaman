
#ifndef __ZM_SERVER_PHYSICS_CONTACTS__
#define __ZM_SERVER_PHYSICS_CONTACTS__

#include <Box2D/Box2D.h>

namespace zm {

class ContactListener : public b2ContactListener
{
  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
};

}
#endif
