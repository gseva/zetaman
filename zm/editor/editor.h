#include <gtkmm.h>

class Editor {
  
  private:
    bool crearJugador = false;
    bool crearTerreno = false;
    bool crearEnemigo = false;

  public:
    void on_buttonCrearJugador_clicked();
    void on_buttonCrearTerreno_clicked();
    void on_buttonCrearEnemigo_clicked();
    bool on_eventbox_button_press(GdkEventButton*);
};