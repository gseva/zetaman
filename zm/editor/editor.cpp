#include "editor.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

#define IMAGE_PLAYER "/zm/editor/images/editor/spawns/player.png"

#define IMAGEN_BLANCO "/zm/editor/images/tiles/void.png"
#define IMAGEN_ESCALERA "/zm/editor/images/tiles/ladder_mid.png"

#define IMAGEN_BUMBY "/zm/editor/images/editor/spawns/bumby.png"
#define IMAGEN_MET "/zm/editor/images/editor/spawns/met.png"
#define IMAGEN_SNIPER "/zm/editor/images/editor/spawns/sniper.png"
#define IMAGE_JUMPING_SNIPER "/zm/editor/images/editor/spawns/jumpingsniper.png"

#define IMAGE_BOSS_FIREMAN "/zm/editor/images/editor/spawns/fireman.png"
#define IMAGE_BOSS_SPARKMAN "/zm/editor/images/editor/spawns/sparkman.png"
#define IMAGE_BOSS_MAGNETMAN "/zm/editor/images/editor/spawns/magnetman.png"
#define IMAGE_BOSS_BOMBMAN "/zm/editor/images/editor/spawns/bombman.png"
#define IMAGE_BOSS_RINGMAN "/zm/editor/images/editor/spawns/ringman.png"

#define IMAGE_GRASS "/zm/editor/images/tiles/grass.png"
#define IMAGE_TILE_GRASS_HALF "/zm/editor/images/tiles/grass_half.png"
#define IMAGE_TILE_GRASS_MID "/zm/editor/images/tiles/grass_mid.png"
#define IMAGE_TILE_BEAM "/zm/editor/images/tiles/beam.png"
#define IMAGE_TILE_BEAM_HOLES "/zm/editor/images/tiles/beam_holes.png"
#define IMAGE_TILE_BOX "/zm/editor/images/tiles/box.png"
#define IMAGE_TILE_BROWN "/zm/editor/images/tiles/brown.png"
#define IMAGE_TILE_BROWN_BOX "/zm/editor/images/tiles/brown_box.png"
#define IMAGE_TILE_DIRT "/zm/editor/images/tiles/dirt.png"
#define IMAGE_TILE_ICE "/zm/editor/images/tiles/ice.png"
#define IMAGE_TILE_METAL "/zm/editor/images/tiles/metal_mid.png"
#define IMAGE_TILE_SAND "/zm/editor/images/tiles/sand.png"
#define IMAGE_TILE_SNOW "/zm/editor/images/tiles/snow.png"
#define IMAGE_TILE_STONE_MID "/zm/editor/images/tiles/stone_mid.png"
#define IMAGE_TILE_TUNDRA_MID "/zm/editor/images/tiles/tundra_mid.png"

#define TILE_PREFIX "/zm/editor/images/tiles/"
#define SPAWN_PREFIX "/zm/editor/images/editor/spawns/"

void Editor::on_buttonCrearJugador_clicked()
{
  pComboBoxEnemy->set_active(0);
  pComboBoxTile->set_active(0);
  std::cout << "Crear jugador seleccionado" << std::endl;
  imagenSeleccionada = IMAGE_PLAYER;
  pSelectedImage->set_from_resource(imagenSeleccionada);
}

void Editor::on_buttonCrearEscalera_clicked()
{
  pComboBoxEnemy->set_active(0);
  pComboBoxTile->set_active(0);
  std::cout << "Crear escalera seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_ESCALERA;
  pSelectedImage->set_from_resource(imagenSeleccionada);
}

void Editor::on_buttonBorrarTile_clicked()
{
  pComboBoxEnemy->set_active(0);
  pComboBoxTile->set_active(0);
  std::cout << "Borrar tile seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_BLANCO;
  pSelectedImage->set_from_resource(imagenSeleccionada);
}

void Editor::on_buttonSaveMap_clicked()
{
  std::cout << "Guardando mapa" << std::endl;
  exportCreatedMap();
}

void Editor::on_ddlEnemy_changed()
{
  pComboBoxTile->set_active(0);
  imagenSeleccionada = ddlToName[pComboBoxEnemy->get_active_text()];
  pSelectedImage->set_from_resource(imagenSeleccionada);
}

void Editor::on_ddlBoss_changed()
{
  selectedBoss = ddlToName[pComboBoxBoss->get_active_text()];
  pSelectedBoss->set_from_resource(selectedBoss);
}

void Editor::on_ddlTile_changed()
{
  pComboBoxEnemy->set_active(0);
  imagenSeleccionada = ddlToName[pComboBoxTile->get_active_text()];
  pSelectedImage->set_from_resource(imagenSeleccionada);
}

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton,
                                      Gtk::Image* imagen, int col, int row)
{
  imagen->set_from_resource(imagenSeleccionada);
  imageNamesCurrent[col][row] = imagenSeleccionada;
  return true;
}

Editor::Editor(Glib::RefPtr<Gtk::Application> appl, unsigned int len,
  std::string mapName): mapLen(len),app(appl)
{
  mapPath = "build/maps/custom/" + mapName + ".json";

  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  /* Elementos de la ventana del editor*/
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("btnCrearEscalera", pBtnCrearEscalera);
  builder->get_widget("btnBorrarTile", pBtnBorrarTile);
  builder->get_widget("btnSaveMap", pBtnSaveMap);
  builder->get_widget("applicationwindow1", pWindowEditor);
  builder->get_widget("grid1", pGrid);
  builder->get_widget("viewport1", pViewPort);
  builder->get_widget("scrolledwindow1", pScrolledWindow);
  builder->get_widget("ddlEnemy", pComboBoxEnemy);
  builder->get_widget("imgSelected", pSelectedImage);
  builder->get_widget("ddlBoss", pComboBoxBoss);
  builder->get_widget("ddlTile", pComboBoxTile);
  builder->get_widget("imgBossSelected", pSelectedBoss);

  pWindowEditor->set_default_size(1024, 768);
  pScrolledWindow->set_size_request(1024,768);

  initializeComboBoxes();

  connectButtonsWithSignals();

  eventBoxMatrix = new Gtk::EventBox*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    eventBoxMatrix[i] = new Gtk::EventBox[ALTO];

  imageMatrix = new Gtk::Image*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    imageMatrix[i] = new Gtk::Image[ALTO];

  imageNamesCurrent = new std::string*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    imageNamesCurrent[i] = new std::string[ALTO];

  createEmptyGrid();

  initializeRelationships();

  imagenSeleccionada = IMAGE_PLAYER;
  pSelectedImage->set_from_resource(imagenSeleccionada);
  selectedBoss = ddlToName[pComboBoxBoss->get_active_text()];
  pSelectedBoss->set_from_resource(selectedBoss);
}

Editor::Editor(Glib::RefPtr<Gtk::Application> appl,
 std::string path): app(appl), mapPath(path)
{
  Glib::RefPtr<Gtk::Builder> builder =
    Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  /* Elementos de la ventana del editor*/
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("btnCrearEscalera", pBtnCrearEscalera);
  builder->get_widget("btnBorrarTile", pBtnBorrarTile);
  builder->get_widget("btnSaveMap", pBtnSaveMap);
  builder->get_widget("applicationwindow1", pWindowEditor);
  builder->get_widget("grid1", pGrid);
  builder->get_widget("viewport1", pViewPort);
  builder->get_widget("scrolledwindow1", pScrolledWindow);
  builder->get_widget("ddlEnemy", pComboBoxEnemy);
  builder->get_widget("imgSelected", pSelectedImage);
  builder->get_widget("ddlBoss", pComboBoxBoss);
  builder->get_widget("ddlTile", pComboBoxTile);
  builder->get_widget("imgBossSelected", pSelectedBoss);

  pWindowEditor->set_default_size(1024, 768);
  pScrolledWindow->set_size_request(1024,768);

  initializeComboBoxes();
  connectButtonsWithSignals();
  initializeRelationships();

  imagenSeleccionada = IMAGE_PLAYER;
  pSelectedImage->set_from_resource(imagenSeleccionada);
  selectedBoss = ddlToName[pComboBoxBoss->get_active_text()];
  pSelectedBoss->set_from_resource(selectedBoss);

  importExistingMap(path);
}

Editor::~Editor()
{
  for (int i = 0; i < ALTO; i++) {
    delete[] eventBoxMatrix[i];
  }
  delete[] eventBoxMatrix;

  for (int i = 0; i < ALTO; i++) {
    delete[] imageMatrix[i];
  }
  delete[] imageMatrix;

  for (int i = 0; i < ALTO; i++) {
    delete[] imageNamesCurrent[i];
  }
  delete[] imageNamesCurrent;
}

void Editor::initializeComboBoxes()
{
  /*Enemigos*/
  pComboBoxEnemy->append("Elegir enemigo");
  pComboBoxEnemy->append("Bumby");
  pComboBoxEnemy->append("Met");
  pComboBoxEnemy->append("Sniper");
  pComboBoxEnemy->append("Jumping sniper");
  pComboBoxEnemy->set_active(0);
  /*Bosses*/
  pComboBoxBoss->append("Sparkman");
  pComboBoxBoss->append("Fireman");
  pComboBoxBoss->append("Magnetman");
  pComboBoxBoss->append("Ringman");
  pComboBoxBoss->append("Bombman");
  pComboBoxBoss->set_active(0);
  /*Tiles*/
  pComboBoxTile->append("Elegir terreno");
  pComboBoxTile->append("Pasto");
  pComboBoxTile->append("Pasto redondeado");
  pComboBoxTile->append("Pasto reducido");
  pComboBoxTile->append("Vigas");
  pComboBoxTile->append("Vigas agujereadas");
  pComboBoxTile->append("Caja");
  pComboBoxTile->append("Caja marron");
  pComboBoxTile->append("Nieve");
  pComboBoxTile->append("Arena");
  pComboBoxTile->append("Marron");
  pComboBoxTile->append("Tundra");
  pComboBoxTile->append("Piedra");
  pComboBoxTile->append("Metal");
  pComboBoxTile->append("Hielo");
  pComboBoxTile->append("Tierra");
  pComboBoxTile->set_active(0);
}

void Editor::initializeRelationships()
{
  /*Relacion entre nombre de la imagen con numero de spawn*/
  nameToSpawnNumber.insert({IMAGE_PLAYER,0});
  nameToSpawnNumber.insert({IMAGEN_BUMBY,1});
  nameToSpawnNumber.insert({IMAGEN_MET,2});
  nameToSpawnNumber.insert({IMAGEN_SNIPER,3});
  nameToSpawnNumber.insert({IMAGE_BOSS_RINGMAN,4});
  nameToSpawnNumber.insert({IMAGE_BOSS_MAGNETMAN,5});
  nameToSpawnNumber.insert({IMAGE_BOSS_FIREMAN,6});
  nameToSpawnNumber.insert({IMAGE_BOSS_BOMBMAN,7});
  nameToSpawnNumber.insert({IMAGE_BOSS_SPARKMAN,8});
  nameToSpawnNumber.insert({IMAGE_JUMPING_SNIPER,9});
  /*Vector con todos los tipos*/
  spawnTypes.push_back("player");
  spawnTypes.push_back("bumby");
  spawnTypes.push_back("met");
  spawnTypes.push_back("sniper");
  spawnTypes.push_back("ringman");
  spawnTypes.push_back("magnetman");
  spawnTypes.push_back("fireman");
  spawnTypes.push_back("bombman");
  spawnTypes.push_back("sparkman");
  spawnTypes.push_back("jumpingsniper");
  /*Relacion entre nombre de la imagen con la fisica*/
  nameToPhysics.insert({IMAGE_GRASS,"solid"});
  nameToPhysics.insert({IMAGE_TILE_GRASS_MID,"solid"});
  nameToPhysics.insert({IMAGE_TILE_GRASS_HALF,"solid"});
  nameToPhysics.insert({IMAGE_TILE_BEAM,"solid"});
  nameToPhysics.insert({IMAGE_TILE_BOX,"solid"});
  nameToPhysics.insert({IMAGE_TILE_BEAM_HOLES,"solid"});
  nameToPhysics.insert({IMAGE_TILE_SNOW,"solid"});
  nameToPhysics.insert({IMAGE_TILE_SAND,"solid"});
  nameToPhysics.insert({IMAGE_TILE_DIRT,"solid"});
  nameToPhysics.insert({IMAGE_TILE_BROWN,"solid"});
  nameToPhysics.insert({IMAGE_TILE_BROWN_BOX,"solid"});
  nameToPhysics.insert({IMAGE_TILE_METAL,"solid"});
  nameToPhysics.insert({IMAGE_TILE_ICE,"solid"});
  nameToPhysics.insert({IMAGE_TILE_TUNDRA_MID,"solid"});
  nameToPhysics.insert({IMAGE_TILE_STONE_MID,"solid"});
  nameToPhysics.insert({IMAGEN_BLANCO,"void"});
  nameToPhysics.insert({IMAGEN_ESCALERA,"stair"});
  /*Relacion entre nombre en ddl con la imagen*/
  ddlToName.insert({"Elegir enemigo", IMAGEN_BLANCO});
  ddlToName.insert({"Bumby", IMAGEN_BUMBY});
  ddlToName.insert({"Met", IMAGEN_MET});
  ddlToName.insert({"Sniper", IMAGEN_SNIPER});
  ddlToName.insert({"Jumping sniper", IMAGE_JUMPING_SNIPER});
  ddlToName.insert({"Elegir boss", IMAGEN_BLANCO});
  ddlToName.insert({"Fireman", IMAGE_BOSS_FIREMAN});
  ddlToName.insert({"Sparkman", IMAGE_BOSS_SPARKMAN});
  ddlToName.insert({"Magnetman", IMAGE_BOSS_MAGNETMAN});
  ddlToName.insert({"Ringman", IMAGE_BOSS_RINGMAN});
  ddlToName.insert({"Bombman", IMAGE_BOSS_BOMBMAN});
  ddlToName.insert({"Elegir terreno", IMAGEN_BLANCO});
  ddlToName.insert({"Pasto", IMAGE_TILE_GRASS_MID});
  ddlToName.insert({"Pasto redondeado", IMAGE_GRASS});
  ddlToName.insert({"Pasto reducido", IMAGE_TILE_GRASS_HALF});
  ddlToName.insert({"Vigas", IMAGE_TILE_BEAM});
  ddlToName.insert({"Caja", IMAGE_TILE_BOX});
  ddlToName.insert({"Vigas agujereadas", IMAGE_TILE_BEAM_HOLES});
  ddlToName.insert({"Nieve", IMAGE_TILE_SNOW});
  ddlToName.insert({"Arena", IMAGE_TILE_SAND});
  ddlToName.insert({"Tierra", IMAGE_TILE_DIRT});
  ddlToName.insert({"Marron", IMAGE_TILE_BROWN});
  ddlToName.insert({"Caja marron", IMAGE_TILE_BROWN_BOX});
  ddlToName.insert({"Metal", IMAGE_TILE_METAL});
  ddlToName.insert({"Hielo", IMAGE_TILE_ICE});
  ddlToName.insert({"Tundra", IMAGE_TILE_TUNDRA_MID});
  ddlToName.insert({"Piedra", IMAGE_TILE_STONE_MID});
}

void Editor::connectButtonsWithSignals()
{
  if (pBtnCrearJugador)
  {
    pBtnCrearJugador->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearJugador_clicked));
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

  if (pComboBoxEnemy)
  {
    pComboBoxEnemy->signal_changed().connect(
      sigc::mem_fun(this, &Editor::on_ddlEnemy_changed));
  }

  if (pComboBoxBoss)
  {
    pComboBoxBoss->signal_changed().connect(
      sigc::mem_fun(this, &Editor::on_ddlBoss_changed));
  }

  if (pComboBoxTile)
  {
    pComboBoxTile->signal_changed().connect(
      sigc::mem_fun(this, &Editor::on_ddlTile_changed));
  }
}

void Editor::createEmptyGrid()
{
  /*Agrego los event box a la grid*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          pGrid->attach(eventBoxMatrix[i][j], i, j, 1, 1);
      }
  }

  /*Agrego las imagenes a los event box*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          eventBoxMatrix[i][j].add(imageMatrix[i][j]);
      }
  }

  /*Seteo las imagenes*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
        if (j == ALTO-1)
        {
         imageMatrix[i][j].set_from_resource(IMAGE_GRASS);
        } else {
          imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
        }
      }
  }

  /*Seteo el evento en los event box*/
  for (unsigned int i=0; i<ANCHO * (mapLen-1); i++)
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
        imageNamesCurrent[i][j] = IMAGE_GRASS;
      } else {
        imageNamesCurrent[i][j] = IMAGEN_BLANCO;
      }
    }
  }

  pGrid->show_all_children();
}

void Editor::exportCreatedMap()
{
  JsonSerializer s;

  JsonMap jMap;

  jMap = createJsonMap();

  s.exportMap(mapPath, jMap);
}

std::string Editor::getName(std::string imageFullPath)
{
  std::size_t found = imageFullPath.find_last_of("/");
  return imageFullPath.substr(found + 1);
}

JsonMap Editor::createJsonMap()
{
  JsonMap jMap;
  std::map<std::string, int> nameToNumber;
  int numeroImagen = 1;
  nameToNumber.insert({IMAGEN_BLANCO,0});
  jMap.physics.push_back(nameToPhysics[IMAGEN_BLANCO]);

  for (unsigned int i=0; i<ALTO; i++)
  {
    for (unsigned int j=0; j<ANCHO * mapLen; j++)
    {
      std::string image = imageNamesCurrent[j][i];

      if (nameToNumber.count(image) == 0 && nameToSpawnNumber.count(image)==0)
      {
        nameToNumber.insert({image,numeroImagen});
        numeroImagen++;

        jMap.imageNames.push_back(getName(image));

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
        data.row = ALTO-i-1;
        data.type = nameToSpawnNumber[image];
        jMap.spawnsData.push_back(data);
      }
    }
  }

  jMap.spawnTypes = spawnTypes;

  /*Agrego el boss en su recamara*/
  SpawnData bossData;
  bossData.column = (ANCHO * mapLen) - 2;
  bossData.row = 4;
  bossData.type = nameToSpawnNumber[selectedBoss];
  jMap.spawnsData.push_back(bossData);

  return jMap;
}

void Editor::importExistingMap(std::string path)
{
  JsonSerializer s;

  JsonMap jMap;

  jMap = s.importMap(path);

  mapLen = jMap.imageNumbers.size() / ALTO;

  mapLen = mapLen / ANCHO;

  eventBoxMatrix = new Gtk::EventBox*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    eventBoxMatrix[i] = new Gtk::EventBox[ALTO];

  imageMatrix = new Gtk::Image*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    imageMatrix[i] = new Gtk::Image[ALTO];

  imageNamesCurrent = new std::string*[ANCHO * mapLen];
  for ( size_t i = 0; i < ANCHO * mapLen; ++i )
    imageNamesCurrent[i] = new std::string[ALTO];

  createGridFromJsonMap(jMap);
}

void Editor::createGridFromJsonMap(JsonMap jm)
{
  /*Agrego los event box a la grid*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          pGrid->attach(eventBoxMatrix[i][j], i, j, 1, 1);
      }
  }

  /*Agrego las imagenes a los event box*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
          eventBoxMatrix[i][j].add(imageMatrix[i][j]);
      }
  }

  /*Seteo las imagenes*/
  for (unsigned int i = 0; i < ANCHO * (mapLen-1); i++)
  {
      for (unsigned int j = 0; j < ALTO; j++)
      {
        int imageNumber =
        jm.imageNumbers[(j+1)*ANCHO*mapLen-(ANCHO*mapLen-i)] - 1;
        if (imageNumber >=0)
        {
          imageMatrix[i][j].set_from_resource(
            TILE_PREFIX + jm.imageNames[imageNumber]);
        } else {
          imageMatrix[i][j].set_from_resource(IMAGEN_BLANCO);
        }
      }
  }

  /*Seteo el evento en los event box*/
  for (unsigned int i=0; i<ANCHO * (mapLen-1); i++)
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
      int imageNumber =
        jm.imageNumbers[(j+1)*ANCHO*mapLen-(ANCHO*mapLen-i)] - 1;
      if (imageNumber >= 0)
      {
        imageNamesCurrent[i][j] = TILE_PREFIX + jm.imageNames[imageNumber];
      } else {
        imageNamesCurrent[i][j] = IMAGEN_BLANCO;
      }
    }
  }

  /*Agrego los spawns*/
  for (unsigned int i=0; i < jm.spawnsData.size(); i++)
  {
    unsigned int col;
    unsigned int row;
    col = jm.spawnsData[i].column;
    row = ALTO - jm.spawnsData[i].row -1;
    if (col < ANCHO * (mapLen-1))
    {
      imageMatrix[col][row].set_from_resource(
        SPAWN_PREFIX + jm.spawnTypes[jm.spawnsData[i].type] + ".png");
      imageNamesCurrent[col][row] =
       SPAWN_PREFIX + jm.spawnTypes[jm.spawnsData[i].type] + ".png";
    } else {
      selectedBoss =
       SPAWN_PREFIX + jm.spawnTypes[jm.spawnsData[i].type] + ".png";
      pSelectedBoss->set_from_resource(selectedBoss);
    }
  }

  pGrid->show_all_children();
}

void EditorMenu::on_buttonAcceptExport_clicked()
{
  if (pEntryExportMapName->get_text_length()>0)
  {
    mapName = pEntryExportMapName->get_text();
    mapLen = pSpinLength->get_value();

    pWindowNewLevel->hide();
    auto appl = Gtk::Application::create("Editor.nuevo.app");
    Editor editor(appl, mapLen, mapName);
    editor.runEditor();
  }
}

void EditorMenu::on_buttonCreateLevel_clicked()
{
  pWindowNewLevel->show();
}

void EditorMenu::on_btnFCAcceptEdit_clicked()
{
  pFCLevelToEdit->hide();
  auto appl = Gtk::Application::create("Editor.modificar.app");
  Editor editor(appl, pFCLevelToEdit -> get_filename());
  editor.runEditor();
}

void EditorMenu::on_buttonEditLevel_clicked()
{
  pFCLevelToEdit->show();
}

EditorMenu::EditorMenu(Glib::RefPtr<Gtk::Application> appl): app(appl)
{
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  /*Elementos de la ventana crear nuevo nivel*/
  builder->get_widget("windowNewLevel", pWindowNewLevel);
  builder->get_widget("btnAcceptExport", pBtnAcceptExport);
  builder->get_widget("entryExportMapName", pEntryExportMapName);
  builder->get_widget("spinLength", pSpinLength);
  pSpinLength -> set_range(4,10);
  pSpinLength -> set_increments(1,1);

  /* Elementos del menu */
  builder->get_widget("windowMenu", pWindowMenu);
  builder->get_widget("btnCreateLevel", pBtnCreateLevel);
  builder->get_widget("btnEditLevel", pBtnEditLevel);

  /*File chooser para editar un nivel existente*/
  builder->get_widget("fcEditLevel", pFCLevelToEdit);
  builder->get_widget("btnAcceptEditFC", pBtnFCAcceptEdit);

  pWindowNewLevel -> set_position(Gtk::WIN_POS_CENTER);
  pWindowMenu -> set_position(Gtk::WIN_POS_CENTER);
  pWindowMenu -> set_default_size(468,320);
  pFCLevelToEdit -> set_position(Gtk::WIN_POS_CENTER);

  connectButtonsWithSignals();
}

void EditorMenu::connectButtonsWithSignals()
{
  if (pBtnAcceptExport)
  {
    pBtnAcceptExport->signal_clicked().connect(
      sigc::mem_fun(this,&EditorMenu::on_buttonAcceptExport_clicked));
  }

  if (pBtnCreateLevel)
  {
    pBtnCreateLevel->signal_clicked().connect(
      sigc::mem_fun(this,&EditorMenu::on_buttonCreateLevel_clicked));
  }

  if (pBtnEditLevel)
  {
    pBtnEditLevel->signal_clicked().connect(
      sigc::mem_fun(this,&EditorMenu::on_buttonEditLevel_clicked));
  }

  if (pBtnFCAcceptEdit)
  {
    pBtnFCAcceptEdit->signal_clicked().connect(
      sigc::mem_fun(this,&EditorMenu::on_btnFCAcceptEdit_clicked));
  }
}

void EditorMenu::runEditorMenu()
{
  app->run(*pWindowMenu);
}

void Editor::runEditor()
{
  std::cout << "Por correr" << std::endl;
  app->run(*pWindowEditor, 0, 0);
  std::cout << "Corriendo" << std::endl;
}
