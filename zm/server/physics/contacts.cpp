
#include "zm/server/physics/contacts.h"
#include "zm/server/physics/bullets.h"

namespace zm {


void ContactListener::BeginContact(b2Contact* contact) {

  void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

  if (bodyUserDataA && !bodyUserDataB) {
    std::cout << "Deleteo body A\n";
    static_cast<Bullet*>( bodyUserDataA )->destroy();
  }

 if (!bodyUserDataA && bodyUserDataB) {
    std::cout << "Deleteo body B\n";
    static_cast<Bullet*>( bodyUserDataB )->destroy();
 }

  //check if fixture B was a ball

}

void ContactListener::EndContact(b2Contact* contact) {

  //check if fixture A was a ball
  // void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
  // if ( bodyUserData )
  //   static_cast<Ball*>( bodyUserData )->endContact();

  //check if fixture B was a ball
  // bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
  // if ( bodyUserData )
  //   static_cast<Ball*>( bodyUserData )->endContact();

}


} // zm
