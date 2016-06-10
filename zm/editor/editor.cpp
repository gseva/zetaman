#include "editor.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

#define IMAGEN_JUGADOR "/zm/editor/images/player/megaman.png"
#define IMAGEN_TERRENO "/zm/editor/images/grass_mid.png"
#define IMAGEN_BUMPY "/zm/editor/images/enemies/bumpy/1.png"
#define IMAGEN_BLANCO "/zm/editor/images/void.png"
#define IMAGEN_ESCALERA "/zm/editor/images/tiles/ladder_mid.png"
#define IMAGEN_MET "/zm/editor/images/enemies/met/unguarded.png"
#define IMAGEN_SNIPER "/zm/editor/images/enemies/sniper/guarded.png"

void Editor::on_buttonCrearJugador_clicked()
{
  pComboBoxEnemy->set_active(0);
  std::cout << "Crear jugador seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_JUGADOR;
}

void Editor::on_buttonCrearTerreno_clicked()
{
  pComboBoxEnemy->set_active(0);
  std::cout << "Crear terreno seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_TERRENO;
}

void Editor::on_buttonCrearEscalera_clicked()
{
  pComboBoxEnemy->set_active(0);
  std::cout << "Crear escalera seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_ESCALERA;
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
  if (pEntryExportMapName->get_text_length()>0)
  {
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

void Editor::on_ddlEnemy_changed()
{
  imagenSeleccionada = ddlToName[pComboBoxEnemy->get_active_text()];
}

void Editor::on_windowEditor_hidden()
{
  std::cout << "Cierro editor" << std::endl;

  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    delete [] eventBoxMatrix[i];
  delete [] eventBoxMatrix;

  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    delete [] imageMatrix[i];
  delete [] imageMatrix;

  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    delete [] imageNamesCurrent[i];
  delete [] imageNamesCurrent;
}

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton,
                                      Gtk::Image* imagen, int col, int row)
{
  imagen->set_from_resource(imagenSeleccionada);
  imageNamesCurrent[col][row] = imagenSeleccionada;
  return true;
}

Editor::Editor(Glib::RefPtr<Gtk::Application> appl, unsigned int len): 
mapLen(len), app(appl)
{
  eventBoxMatrix = new Gtk::EventBox*[ANCHO * len];
  for ( size_t i = 0; i < ANCHO * len; ++i )
    eventBoxMatrix[i] = new Gtk::EventBox[ALTO];

  imageMatrix = new Gtk::Image*[ANCHO * len];
  for ( size_t i = 0; i < ANCHO * len; ++i )
    imageMatrix[i] = new Gtk::Image[ALTO];

  imageNamesCurrent = new std::string*[ANCHO * len];
  for ( size_t i = 0; i < ANCHO * len; ++i )
    imageNamesCurrent[i] = new std::string[ALTO];

  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  /* Elementos de la ventana del editor*/
  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("btnCrearEscalera", pBtnCrearEscalera);
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

  /* Elementos del menu */
  builder->get_widget("windowMenu", pWindowMenu);
  builder->get_widget("btnCreateLevel", pBtnCreateLevel);
  builder->get_widget("btnEditLevel", pBtnEditLevel);

  pWindowEditor->set_default_size(1024, 768);
  pScrolledWindow->set_size_request(768,768);

  pComboBoxEnemy->append("Elegir enemigo");
  pComboBoxEnemy->append("Bumpy");
  pComboBoxEnemy->append("Met");
  pComboBoxEnemy->append("Sniper");
  pComboBoxEnemy->set_active(0);

  connectButtonsWithSignals();

  createEmptyGrid();

  initializeRelationships();


  imagenSeleccionada = IMAGEN_TERRENO;
}

Editor::~Editor()
{
}

void Editor::initializeRelationships()
{
  /*Relacion entre nombre de la imagen con numero de spawn*/
  nameToSpawnNumber.insert({IMAGEN_JUGADOR,0});
  nameToSpawnNumber.insert({IMAGEN_BUMPY,1});
  nameToSpawnNumber.insert({IMAGEN_MET,2});
  nameToSpawnNumber.insert({IMAGEN_SNIPER,3});
  /*Relacion entre nombre de la imagen con tipo de spawn*/
  nameToSpawnType.insert({IMAGEN_BUMPY,"enemy"});
  nameToSpawnType.insert({IMAGEN_MET,"enemy"});
  nameToSpawnType.insert({IMAGEN_SNIPER,"enemy"});
  nameToSpawnType.insert({IMAGEN_JUGADOR,"player"});
  /*Relacion entre nombre de la imagen con la fisica*/
  nameToPhysics.insert({IMAGEN_TERRENO,"solid"});
  nameToPhysics.insert({IMAGEN_BLANCO,"void"});
  nameToPhysics.insert({IMAGEN_JUGADOR,"void"});
  nameToPhysics.insert({IMAGEN_BUMPY,"void"});
  nameToPhysics.insert({IMAGEN_ESCALERA,"stair"});
  /*Relacion entre nombre en ddl con la imagen*/
  ddlToName.insert({"Bumpy", IMAGEN_BUMPY});
  ddlToName.insert({"Met", IMAGEN_MET});
  ddlToName.insert({"Sniper", IMAGEN_SNIPER});
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

  if (pBtnCrearEscalera)
  {
    pBtnCrearEscalera->signal_clicked().connect(
      sigc::mem_fun(this,&Editor::on_buttonCrearEscalera_clicked));
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

  if (pComboBoxEnemy)
  {
    pComboBoxEnemy->signal_changed().connect(
      sigc::mem_fun(this, &Editor::on_ddlEnemy_changed));
  }

  if (pWindowEditor)
  {
    pWindowEditor->signal_hide().connect(
      sigc::mem_fun(this, &Editor::on_windowEditor_hidden));
  }
}

void Editor::createEmptyGrid()
{
  /*Agrego los event box a la grid*/
  for (unsigned int i = 0; i < ANCHO * mapLen; i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          pGrid->attach(eventBoxMatrix[i][j], i, j, 1, 1);
      }
  }

  /*Agrego las imagenes a los event box*/
  for (unsigned int i = 0; i < ANCHO * mapLen; i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          eventBoxMatrix[i][j].add(imageMatrix[i][j]);
      }
  }

  /*Seteo las imagenes*/
  for (unsigned int i = 0; i < ANCHO * mapLen; i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
        if (j == ALTO-1)
        {
         imageMatrix[i][j].set_from_resource(IMAGEN_TERRENO); 
        } else {
          imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
        }
      }
  }

  /*Seteo el evento en los event box*/
  for (unsigned int i=0; i<ANCHO * mapLen; i++)
  {
    for (unsigned int j=0; j<ALTO; j++)
    {
      eventBoxMatrix[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
      eventBoxMatrix[i][j].signal_button_press_event().connect(
        sigc::bind<Gtk::Image*>(sigc::mem_fun(
            this,&Editor::on_eventbox_button_press), &imageMatrix[i][j], i, j));
    }
  }

  /*Seteo datos de esta pantalla*/
  for (unsigned int i=0; i<ANCHO * mapLen; i++)
  {
    for (unsigned int j=0; j<ALTO; j++)
    {
      if (j==ALTO-1)
      {
        imageNamesCurrent[i][j] = IMAGEN_TERRENO;  
      } else {
        imageNamesCurrent[i][j] = IMAGEN_BLANCO;  
      }
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

  for (unsigned int i=0; i<ALTO; i++)
  {
    for (unsigned int j=0; j<ANCHO * mapLen; j++)
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

void Editor::importExistingMap(std::string path)
{
  JsonSerializer s;
  
  JsonMap jMap;

  jMap = s.importMap(path);
}
