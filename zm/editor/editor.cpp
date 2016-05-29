#include "editor.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

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

void Editor::on_buttonNextScreen_clicked()
{
  if ((currentScreenNumber!=totalScreenCount-1)
    && contenidoPantallas.size()!=0 )
  {
    saveUnsavedScreen();

    for (int i = 0; i < ANCHO; i++)
    {
      for (int j = 0; j < ALTO; j++)
      {
        imageMatrix[i][j].set_from_resource(
        contenidoPantallas[currentScreenNumber+1].imageNamesMatrix[i][j]);
      }
    }

    for (int i = 0; i < ANCHO; i++)
    {
      for (int j = 0; j < ALTO; j++)
      {
        imageNamesCurrent[i][j] = 
        contenidoPantallas[currentScreenNumber+1].imageNamesMatrix[i][j];
      }
    }

    currentScreenNumber++;
  }

  std::cout << currentScreenNumber << std::endl;
}

void Editor::on_buttonPreviousScreen_clicked()
{
  if (currentScreenNumber!=0)
  {
    saveUnsavedScreen();

    for (int i = 0; i < ANCHO; i++)
    {
      for (int j = 0; j < ALTO; j++)
      {
        imageMatrix[i][j].set_from_resource(
        contenidoPantallas[currentScreenNumber-1].imageNamesMatrix[i][j]);
      }
    }

    for (int i = 0; i < ANCHO; i++)
    {
      for (int j = 0; j < ALTO; j++)
      {
        imageNamesCurrent[i][j] = 
        contenidoPantallas[currentScreenNumber-1].imageNamesMatrix[i][j];
      }
    }

    currentScreenNumber--;  
  }

  std::cout << currentScreenNumber << std::endl;
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
  builder->get_widget("btnNextScreen", pBtnNextScreen);
  builder->get_widget("btnPreviousScreen", pBtnPreviousScreen);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("grid1", pGrid);

  pwindow->set_default_size(1024, 768);

  connectButtonsWithSignals();

  createEmptyGrid();

  nameToSpawnNumber.insert({IMAGEN_ENEMIGO,0});
  nameToSpawnType.insert({IMAGEN_ENEMIGO,"enemy"});
  nameToSpawnNumber.insert({IMAGEN_JUGADOR,1});
  nameToSpawnType.insert({IMAGEN_JUGADOR,"player"});
  nameToPhysics.insert({IMAGEN_TERRENO,"solid"});
  nameToPhysics.insert({IMAGEN_BLANCO,"void"});
  nameToPhysics.insert({IMAGEN_JUGADOR,"void"});
  nameToPhysics.insert({IMAGEN_ENEMIGO,"void"});

  currentScreenNumber = 0;
  totalScreenCount = 1;
  imagenSeleccionada = IMAGEN_TERRENO;
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

  if (pBtnNextScreen)
  {
    pBtnNextScreen->signal_clicked().connect(
      sigc::mem_fun(this, &Editor::on_buttonNextScreen_clicked));
  }

  if (pBtnPreviousScreen)
  {
    pBtnPreviousScreen->signal_clicked().connect(
      sigc::mem_fun(this, &Editor::on_buttonPreviousScreen_clicked));
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
  saveUnsavedScreen();

  for (int i=0; i<ANCHO; i++)
  {
    for (int j=0; j<ALTO; j++)
    {
      imageNamesCurrent[i][j] = IMAGEN_BLANCO;
    }
  }

  totalScreenCount++;
  currentScreenNumber = totalScreenCount-1;

  for (int i = 0; i < ANCHO; i++)
  {
    for (int j = 0; j < ALTO; j++)
    {
      imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
    }
  }
}

void Editor::saveUnsavedScreen()
{
  /*Si no esta guardada la ultima*/
  if (currentScreenNumber==contenidoPantallas.size())
  {
    ScreenContent currentScreen;

    for (int i=0; i<ANCHO; i++)
    {
      for (int j=0; j<ALTO; j++)
      {
        currentScreen.imageNamesMatrix[i][j] = imageNamesCurrent[i][j];
      }
    }

  std::cout << "Pantalla guardada" << std::endl;
  contenidoPantallas.push_back(currentScreen);
  } else {
    for (int i=0; i<ANCHO; i++)
    {
      for (int j=0; j<ALTO; j++)
      {
        contenidoPantallas[currentScreenNumber].imageNamesMatrix[i][j] =
         imageNamesCurrent[i][j];
      }
    }
  }
}

void Editor::exportCreatedMap()
{
  JsonSerializer s;
  
  JsonMap jMap;

  saveUnsavedScreen();

  jMap = createJsonMap();

  s.exportMap("exportandoEditor.json", jMap);
}

JsonMap Editor::createJsonMap()
{
  JsonMap jMap;
  std::map<std::string, int> nameToNumber;
  int numeroImagen = 0;

  for (int i=0; i<ALTO; i++)
  {
    for (unsigned int k=0; k<contenidoPantallas.size(); k++)
    {
      for (int j=0; j<ANCHO; j++)
      {
        std::string image = contenidoPantallas.at(k).imageNamesMatrix[j][i];

        if (nameToNumber.count(image) == 0)
        {
          nameToNumber.insert({image,numeroImagen});
          numeroImagen++;

          /*Si la imagen es de un spawn se tiene que dibujar aire*/
          if (nameToSpawnNumber.count(image)==0)
          {
            jMap.imageNames.push_back(image);
          } else {
            jMap.imageNames.push_back(IMAGEN_BLANCO);
          }
          jMap.physics.push_back(nameToPhysics[image]);
        }

        jMap.imageNumbers.push_back(nameToNumber[image]);

        if (nameToSpawnNumber.count(image)!=0)
        {
          SpawnData data;
          data.column = j;
          data.row = i;
          data.type = nameToSpawnNumber[image];
          jMap.spawnsData.push_back(data);
        }
      }
    }
  }

  for (std::map<std::string, int>::iterator it = nameToSpawnNumber.begin();
       it != nameToSpawnNumber.end(); ++it)
  {
    jMap.spawnTypes.push_back(nameToSpawnType[it->first]);
  }

  return jMap;
}
