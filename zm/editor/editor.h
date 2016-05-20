#include <gtkmm.h>
#include <string>

class Editor {
  private:
    std::string imagenSeleccionada;

  public:
    void on_buttonCrearJugador_clicked();
    void on_buttonCrearTerreno_clicked();
    void on_buttonCrearEnemigo_clicked();
    bool on_eventbox_button_press(GdkEventButton* eventButton,
                                   Gtk::Image* imagen);
};
