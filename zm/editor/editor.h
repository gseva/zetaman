#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/application.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>
#include <gtkmm/builder.h>
#include <gtkmm/viewport.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/entry.h>
#include <glibmm/refptr.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "zm/json/jsonserializer.h"

#define ALTO 9
#define ANCHO 12

class Editor {
  private:
    unsigned int mapLen;

    std::string imagenSeleccionada;
    Gtk::Button* pBtnCrearTerreno;
    Gtk::Button* pBtnCrearJugador;
    Gtk::Button* pBtnCrearEscalera;
    Gtk::Button* pBtnBorrarTile;
    Gtk::Button* pBtnSaveMap;
    Gtk::Window* pWindowEditor;
    Gtk::ComboBoxText* pComboBoxEnemy;
    Gtk::ScrolledWindow* pScrolledWindow;
    Gtk::Grid* pGrid;
    Gtk::Viewport* pViewPort;
    Glib::RefPtr<Gtk::Application> app;

    Gtk::EventBox** eventBoxMatrix;
    Gtk::Image** imageMatrix;
    std::string** imageNamesCurrent;


    std::map<std::string, int> nameToSpawnNumber;
    std::map<std::string, std::string> nameToSpawnType;
    std::map<std::string, std::string> nameToPhysics;
    std::map<std::string, std::string> ddlToName;

    std::string mapName;

  public:
    void on_buttonCrearJugador_clicked();
    void on_buttonCrearTerreno_clicked();
    void on_buttonCrearEscalera_clicked();
    void on_buttonBorrarTile_clicked();
    void on_buttonSaveMap_clicked();
    void on_ddlEnemy_changed();
    bool on_eventbox_button_press(GdkEventButton* eventButton,
                                   Gtk::Image* imagen, int col, int row);
    void on_windowEditor_hidden();
    explicit Editor(Glib::RefPtr<Gtk::Application> appl,
    unsigned int len, std::string mapName);
    void connectButtonsWithSignals();
    void createEmptyGrid();
    void runEditor();
    void exportCreatedMap();
    JsonMap createJsonMap();
    void initializeRelationships();
    void importExistingMap(std::string path);
};

class EditorMenu {
  private:
    unsigned int mapLen;

    // Gtk::Window* pWindowEditor;
    Gtk::SpinButton* pSpinLength;
    Glib::RefPtr<Gtk::Application> app;

    Gtk::Window* pWindowNewLevel;
    Gtk::Button* pBtnAcceptExport;
    Gtk::Entry* pEntryExportMapName;

    Gtk::Window* pWindowMenu;
    Gtk::Button* pBtnCreateLevel;
    Gtk::Button* pBtnEditLevel;

    std::string mapName;

  public:
    void on_buttonAcceptExport_clicked();
    void on_buttonCreateLevel_clicked();
    void on_buttonEditLevel_clicked();
    explicit EditorMenu(Glib::RefPtr<Gtk::Application> appl);
    void connectButtonsWithSignals();
    void runEditorMenu();
};
