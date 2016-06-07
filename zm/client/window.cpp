
#include <string>

#include "zm/client/window.h"
#include "zm/client/client.h"

namespace zm {


Window::Window(Client* c) : c_(c), startButton_("Empezar!") {
  add(box_);

  // signal_key_press_event().connect(
  //   sigc::mem_fun(*this, &Window::keyReleased));
  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

  startButton_.signal_clicked().connect(
        sigc::mem_fun(this, &Window::on_startButton_clicked));

  selectedWidget_ = &startButton_;
  box_.pack_start(startButton_);
  startButton_.show();
  box_.show();
}

void Window::showLevelSelection() {
  panel_ = Gtk::manage(new LevelSelectionPanel(c_));
  box_.remove(*selectedWidget_);
  box_.pack_start(*panel_);
  selectedWidget_ = panel_;
  panel_->show();
  // std::cout << "Muestro panel\n";
}

void Window::showWaitingScreen() {
  waitingScreen_ = Gtk::manage(new WaitingScreen());
  box_.remove(*selectedWidget_);
  box_.pack_start(*waitingScreen_);
  selectedWidget_ = waitingScreen_;
  waitingScreen_->show();
  c_->getMap();
}

void Window::showCanvas() {
  box_.remove(*selectedWidget_);
  canvas_ = Gtk::manage(new Canvas(c_->serverProxy));

  c_->serverProxy.updateHandler.signal_game_update().connect(
          sigc::mem_fun(*canvas_, &Canvas::updateGameState) );

  box_.pack_start(*canvas_);
  canvas_->show();
  c_->serverProxy.startLevel();
}

bool Window::on_key_press_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_space) {
    c_->serverProxy.jump();
  } else if (event->keyval == GDK_KEY_Right) {
    c_->serverProxy.moveRight();
  } else if (event->keyval == GDK_KEY_Left) {
    c_->serverProxy.moveLeft();
  } else if (event->keyval == GDK_KEY_Up) {
    c_->serverProxy.up();
  } else if ((event->keyval == GDK_KEY_A) || (event->keyval == GDK_KEY_a)) {
    c_->serverProxy.shoot();
  }

  return Gtk::Window::on_key_press_event(event);
}

bool Window::on_key_release_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_Right) {
    c_->serverProxy.stopHorizontalMove();
  } else if (event->keyval == GDK_KEY_Left) {
    c_->serverProxy.stopHorizontalMove();
  }

  return Gtk::Window::on_key_release_event(event);
}

void Window::on_startButton_clicked() {
  c_->startConnection();
  if (c_->serverProxy.isHost) {
    showLevelSelection();
  } else {
    showWaitingScreen();
  }
}


LevelSelectionPanel::LevelSelectionPanel(Client* c) : c_(c) {
  for (unsigned i = 0; i < 5; ++i) {
    levelButtons[i].set_label("Nivel " + std::to_string(i + 1));
    add(levelButtons[i]);

    levelButtons[i].signal_clicked().connect(
      sigc::bind<int>(sigc::mem_fun(*this,
        &LevelSelectionPanel::on_button_clicked), i + 1) );
  }
  show_all_children();
}

void LevelSelectionPanel::on_button_clicked(int i) {
  std::cout << "Boton presiona2 " << i << std::endl;
  c_->serverProxy.selectLevel(i);
  c_->getMap();
}

WaitingScreen::WaitingScreen() :
  label_("Esperando la selección del nivel...") {
  add(label_);
  label_.show();
}

} // zm
