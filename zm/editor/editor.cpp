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
  imagenSeleccionada = ddlToName[pComboBoxEnemy->get_active_text()];
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

void Editor::on_buttonAcceptExport_clicked()
{
  if (pEntryLevelLength->get_text_length()>0
    && pEntryExportMapName->get_text_length()>0)
  {
    std::stringstream ss;
    ss << pEntryLevelLength->get_text();
    ss >> mapLength;
    mapName = pEntryExportMapName->get_text();
    pWindowNewLevel->hide();
    pWindowEditor->show();
  }
}

void Editor::on_buttonCreateLevel_clicked()
{
  pWindowNewLevel->show();
}

void Editor::on_buttonEditLevel_clicked()
{
}

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton,
                                      Gtk::Image* imagen, int col, int row)
{
  imagen->set_from_resource(imagenSeleccionada);
  imageNamesCurrent[col][row] = imagenSeleccionada;
  return true;
}

Editor::Editor(Glib::RefPtr<Gtk::Application> appl)
{
  app = appl;
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  /* Elementos de la ventana del editor*/
  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearEnemigo", pBtnCrearEnemigo);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("btnBorrarTile", pBtnBorrarTile);
  builder->get_widget("btnSaveMap", pBtnSaveMap);
  builder->get_widget("applicationwindow1", pWindowEditor);
  builder->get_widget("grid1", pGrid);
  builder->get_widget("viewport1", pViewPort);
  builder->get_widget("scrolledwindow1", pScrolledWindow);
  builder->get_widget("ddlEnemy", pComboBoxEnemy);
  
  builder->get_widget("windowNewLevel", pWindowNewLevel);
  builder->get_widget("btnAcceptExport", pBtnAcceptExport);
  builder->get_widget("entryExportMapName", pEntryExportMapName);
  builder->get_widget("entryLevelLength", pEntryLevelLength);

  /* Elementos del menu */
  builder->get_widget("windowMenu", pWindowMenu);
  builder->get_widget("btnCreateLevel", pBtnCreateLevel);
  builder->get_widget("btnEditLevel", pBtnEditLevel);

  pWindowEditor->set_default_size(1024, 768);
  pScrolledWindow->set_size_request(768,768);

  pComboBoxEnemy->append("Enemigo 1");
  pComboBoxEnemy->set_active(0);

  connectButtonsWithSignals();

  createEmptyGrid();

  initializeRelationships();


  imagenSeleccionada = IMAGEN_TERRENO;
}

void Editor::initializeRelationships()
{
  /*Relacion entre nombre de la imagen con numero de spawn*/
  nameToSpawnNumber.insert({IMAGEN_ENEMIGO,0});
  nameToSpawnNumber.insert({IMAGEN_JUGADOR,1});
  /*Relacion entre nombre de la imagen con tipo de spawn*/
  nameToSpawnType.insert({IMAGEN_ENEMIGO,"enemy"});
  nameToSpawnType.insert({IMAGEN_JUGADOR,"player"});
  /*Relacion entre nombre de la imagen con la fisica*/
  nameToPhysics.insert({IMAGEN_TERRENO,"solid"});
  nameToPhysics.insert({IMAGEN_BLANCO,"void"});
  nameToPhysics.insert({IMAGEN_JUGADOR,"void"});
  nameToPhysics.insert({IMAGEN_ENEMIGO,"void"});
  /*Relacion entre nombre en ddl con la imagen*/
  ddlToName.insert({"Enemigo 1", IMAGEN_ENEMIGO});
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

  if (pBtnAcceptExport)
  {
    pBtnAcceptExport->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonAcceptExport_clicked));
  }

  if (pBtnCreateLevel)
  {
    pBtnCreateLevel->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonCreateLevel_clicked));
  }

  if (pBtnEditLevel)
  {
    pBtnEditLevel->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonEditLevel_clicked));
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

  app->run(*pWindowMenu);
}

void Editor::exportCreatedMap()
{
  JsonSerializer s;
  
  JsonMap jMap;

  jMap = createJsonMap();

  s.exportMap(mapName + ".json", jMap);
}

JsonMap Editor::createJsonMap()
{
  JsonMap jMap;
  std::map<std::string, int> nameToNumber;
  int numeroImagen = 1;
  nameToNumber.insert({IMAGEN_BLANCO,0});

  for (int i=0; i<ALTO; i++)
  {
    for (int j=0; j<ANCHO; j++)
    {
      std::string image = imageNamesCurrent[j][i];

      if (nameToNumber.count(image) == 0 && nameToSpawnNumber.count(image)==0)
      {
        nameToNumber.insert({image,numeroImagen});
        numeroImagen++;          
        
        jMap.imageNames.push_back(image);
        
        jMap.physics.push_back(nameToPhysics[image]);
      }

      if (nameToSpawnNumber.count(image)==0)
      {
        jMap.imageNumbers.push_back(nameToNumber[image]);  
      } else {
        jMap.imageNumbers.push_back(nameToNumber[IMAGEN_BLANCO]);
      }

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

  for (std::map<std::string, int>::iterator it = nameToSpawnNumber.begin();
       it != nameToSpawnNumber.end(); ++it)
  {
    jMap.spawnTypes.push_back(nameToSpawnType[it->first]);
  }

  return jMap;
}
