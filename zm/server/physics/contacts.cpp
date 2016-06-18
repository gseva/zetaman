#include "zm/server/physics/world.h"
#include "zm/server/physics/contacts.h"
#include "zm/server/physics/bullets.h"
#include "zm/server/physics/enemies.h"

namespace zm {


void ContactListener::BeginContact(b2Contact* contact) {
  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

  if (bodyUserDataB &&
      static_cast<Body*>(bodyUserDataB)->type == BodyType::Bullet) {
    void* aux = bodyUserDataA;
    bodyUserDataA = bodyUserDataB;
    bodyUserDataB = aux;
  }

  if (bodyUserDataA &&
      static_cast<Body*>(bodyUserDataA)->type == BodyType::Bullet) {
    std::cout << "Deleteo bala\n";
    static_cast<Body*>(bodyUserDataA)->impact();
    if (bodyUserDataB &&
        static_cast<Body*>(bodyUserDataB)->type == BodyType::Enemy) {
      std::cout << "Deleteo enemigo\n";
      static_cast<Enemy*>(bodyUserDataB)->impact();
    }
  }
}

void ContactListener::EndContact(b2Contact* contact) {
}


} // zm
