#include "editor.h"
#include <iostream>

#define IMAGEN_JUGADOR "/zm/editor/images/player.png"
#define IMAGEN_TERRENO "/zm/editor/images/grass_mid.png"
#define IMAGEN_ENEMIGO "/zm/editor/images/enemy.png"
#define IMAGEN_BLANCO "/zm/editor/images/void.png"

void Editor::on_buttonCrearJugador_clicked()
{
  std::cout << "Crear jugador seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_JUGADOR;
}

void Editor::on_buttonCrearTerreno_clicked()
{
  std::cout << "Crear terreno seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_TERRENO;
}

void Editor::on_buttonCrearEnemigo_clicked()
{
  std::cout << "Crear enemigo seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_ENEMIGO;
}

void Editor::on_buttonBorrarTile_clicked()
{
  std::cout << "Borrar tile seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_BLANCO;
}

void Editor::on_buttonSaveMap_clicked()
{
  std::cout << "Guardando mapa" << std::endl;

  exportCreatedMap();
}

void Editor::on_buttonAddScreen_clicked()
{
  createNewScreen();
}

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton,
                                      Gtk::Image* imagen, int col, int row)
{
  imagen->set_from_resource(imagenSeleccionada);
  imageNamesCurrent[col][row] = imagenSeleccionada;
  std::cout << "Clickeo en division" << col << " "<< row << std::endl;
  return true;
}

Editor::Editor(Glib::RefPtr<Gtk::Application> appl)
{
  app = appl;
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearEnemigo", pBtnCrearEnemigo);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("btnBorrarTile", pBtnBorrarTile);
  builder->get_widget("btnSaveMap", pBtnSaveMap);
  builder->get_widget("btnAddScreen", pBtnAgregarPantalla);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("grid1", pGrid);

  pwindow->set_default_size(1024, 768);

  connectButtonsWithSignals();

  createEmptyGrid();
}

void Editor::connectButtonsWithSignals()
{
  if (pBtnCrearJugador)
  {
    pBtnCrearJugador->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearJugador_clicked));
  }

  if (pBtnCrearTerreno)
  {
    pBtnCrearTerreno->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearTerreno_clicked));
  }

  if (pBtnCrearEnemigo)
  {
    pBtnCrearEnemigo->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearEnemigo_clicked));
  }

  if (pBtnBorrarTile)
  {
    pBtnBorrarTile ->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonBorrarTile_clicked));
  }

  if (pBtnSaveMap)
  {
    pBtnSaveMap->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonSaveMap_clicked));
  }

  if (pBtnAgregarPantalla)
  {
    pBtnAgregarPantalla->signal_clicked().connect(
      sigc::mem_fun(this, &Editor::on_buttonAddScreen_clicked));
  }
}

void Editor::createEmptyGrid()
{
  /*Agrego los event box a la grid*/
  for (int i = 0; i < ANCHO; i++)
  {
      for (int j = 0; j < ALTO; j++)
      {
          pGrid->attach(eventBoxMatrix[i][j], i, j, 1, 1);
      }
  }

  /*Agrego las imagenes a los event box*/
  for (int i = 0; i < ANCHO; i++)
  {
      for (int j = 0; j < ALTO; j++)
      {
          eventBoxMatrix[i][j].add(imageMatrix[i][j]);
      }
  }

  /*Seteo las imagenes*/
  for (int i = 0; i < ANCHO; i++)
  {
      for (int j = 0; j < ALTO; j++)
      {
          imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
      }
  }

  /*Seteo el evento en los event box*/
  for (int i=0; i<ANCHO; i++)
  {
    for (int j=0; j<ALTO; j++)
    {
      eventBoxMatrix[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
      eventBoxMatrix[i][j].signal_button_press_event().connect(
        sigc::bind<Gtk::Image*>(sigc::mem_fun(
            this,&Editor::on_eventbox_button_press), &imageMatrix[i][j], i, j));
    }
  }

  /*Seteo datos de esta pantalla*/
  currentScreenNumber = 0;
  for (int i=0; i<ANCHO; i++)
  {
    for (int j=0; j<ALTO; j++)
    {
      imageNamesCurrent[i][j] = IMAGEN_BLANCO;
    }
  }
}

void Editor::runEditor()
{
  pGrid->show_all_children();

  app->run(*pwindow);
}


void Editor::createNewScreen()
{
  ScreenContent currentScreen;
  currentScreen.screenNumber = currentScreenNumber;

  for (int i=0; i<ANCHO; i++)
  {
    for (int j=0; j<ALTO; j++)
    {
      currentScreen.imageNamesMatrix[i][j] = imageNamesCurrent[i][j];
      imageNamesCurrent[i][j] = IMAGEN_BLANCO;
    }
  }

  contenidoPantallas.push_back(currentScreen);
  currentScreenNumber++;

  for (int i = 0; i < ANCHO; i++)
  {
    for (int j = 0; j < ALTO; j++)
    {
      imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
    }
  }
}

void Editor::saveLastScreen()
{
  ScreenContent currentScreen;
  currentScreen.screenNumber = currentScreenNumber;

  for (int i=0; i<ANCHO; i++)
  {
    for (int j=0; j<ALTO; j++)
    {
      currentScreen.imageNamesMatrix[i][j] = imageNamesCurrent[i][j];
    }
  }

  contenidoPantallas.push_back(currentScreen);
}

void Editor::exportCreatedMap()
{
  JsonSerializer s;
  
  JsonMap jMap;

  saveLastScreen();

  jMap = createJsonMap();

  s.exportMap("exportandoEditor.json", jMap);
}

JsonMap Editor::createJsonMap()
{
  JsonMap jMap;

  for (int i=0; i<ALTO; i++)
  {
    for (unsigned int k=0; k<contenidoPantallas.size(); k++)
    {
      for (int j=0; j<ANCHO; j++)
      {
        jMap.imageNames.push_back(
          contenidoPantallas.at(k).imageNamesMatrix[j][i]);
      }
    }
  }

  return jMap;
}
