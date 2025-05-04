#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include "../thirdparty/TinyFileDialogs/tinyfiledialogs.h"
#include "gui-widgets/Button.hpp"
#include "gui-widgets/CheckBox.hpp"
#include "gui-widgets/ColourPicker.hpp"
#include "gui-widgets/HeadingWithLine.hpp"
#include "gui-widgets/HorizontalSeperator.hpp"
#include "gui-widgets/RadioButton.hpp"
#include "gui-widgets/TextElement.hpp"
#include "gui-widgets/VerticalSeparator.hpp"

#include <SFML/Graphics.hpp>

class SettingsWindow
{
 public:

 const static int TARGET_FPS = 144;

  SettingsWindow(sf::RenderWindow& window);
  ~SettingsWindow();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void mouseScroll(sf::Event event);
  void mouseDragged(sf::Event event);

  bool showSimulation() const { return show_simulation; }

  void openImageFileDialog();
  bool pickColour(sf::Color& out_colour, std::string& hex_colour);
  void calcFPS();

 sf::Vector2f sim_window_size = {800, 800};

 private:
  sf::RenderWindow& window;
  bool show_simulation = false;
  sf::Color button_colour = sf::Color(15, 89, 0, 255);
  sf::Color button_hover = sf::Color(109, 179, 95, 255);

  // Main Header
  sf::Texture icon_texture;
  sf::Sprite icon_sprite;

  sf::Text title_text;
  sf::Text subtitle_text;
  HorizontalSeparator hoz_sep_main_header;

  // Sim Button
  Button btn_start_simulation;


  // Source
  HeadingWithLine hdn_source;

  RadioButtonGroup source_button_group;
  RadioButton* rb_image;
  RadioButton* rb_solid_colour;

  VerticalSeparator ver_sep_source;

  // Image - Source
  TextElement file_label;
  TextElement file_name;
  Button btn_choose_file;
  std::string full_file_path;
  std::string file_path_name = "null";

  // Image - Colour
  TextElement colour_label;
  ColourPicker* colour_picker;
  CheckBox* rainbow_selection;

  sf::Color picked_colour = sf::Color(237, 209, 130, 255);
  std::string picked_colour_hex = "";







  // Options
  bool use_image = true;
  bool use_solid_colour = false;
  bool use_rainbow = false;

  sf::Color disabled_colour = sf::Color(89, 89, 89, 255);





 // Info
 sf::Font font;
 sf::Text fps_text;
 sf::Text particle_text;
 sf::Text spawn_text;
 bool draw_info = false;

 // Mouse drag
 bool is_left_mouse_down = false;
 sf::Vector2f prev_mouse_pos;

 // FPS
 sf::Clock fpsClock;
 int frameCount = 0;
 float fps = 0.f;

 // Shader
 sf::Shader sand_shader;
 bool shader_loaded = false;
 sf::Clock clock;





};

#endif // SPACEINVADERS_GAME_H
