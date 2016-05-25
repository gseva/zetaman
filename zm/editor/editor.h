#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/application.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>
#include <gtkmm/builder.h>
#include <glibmm/refptr.h>
#include <string>
#include <iostream>
#include "zm/json/jsonserializer.h"

#define ALTO 9
#define ANCHO 12

class Editor {
  private:
    std::string imagenSeleccionada;
    Gtk::Button* pBtnCrearTerreno;
    Gtk::Button* pBtnCrearEnemigo;
    Gtk::Button* pBtnCrearJugador;
    Gtk::Button* pBtnBorrarTile;
    Gtk::Button* pBtnSaveMap;
    Gtk::Window* pwindow;
    Gtk::Grid* pGrid;
    Glib::RefPtr<Gtk::Application> app;

    Gtk::EventBox eventBoxMatrix[ANCHO][ALTO];
    Gtk::Image imageMatrix[ANCHO][ALTO];

    std::string imageNamesMatrix[ANCHO][ALTO];

  public:
    void on_buttonCrearJugador_clicked();
    void on_buttonCrearTerreno_clicked();
    void on_buttonCrearEnemigo_clicked();
    void on_buttonBorrarTile_clicked();
    void on_buttonSaveMap_clicked();
    bool on_eventbox_button_press(GdkEventButton* eventButton,
                                   Gtk::Image* imagen, int col, int row);
    explicit Editor(Glib::RefPtr<Gtk::Application> appl);
    void connectButtonsWithSignals();
    void createEmptyGrid();
    void runEditor();
    void createNewScreen();
    void exportCreatedMap();
    JsonMap createJsonMap();
};
