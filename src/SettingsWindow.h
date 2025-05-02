#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include "../thirdparty/TinyFileDialogs/tinyfiledialogs.h"
#include <SFML/Graphics.hpp>
#include "gui-widgets/Button.h"
#include "gui-widgets/HorizontalSeperator.h"

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
 void calcFPS();

 sf::Vector2f sim_window_size = {800, 800};

 private:
  sf::RenderWindow& window;
  bool show_simulation = false;

  // Main Header
  sf::Texture icon_texture;
  sf::Sprite icon_sprite;

  sf::Text title_text;
  sf::Text subtitle_text;
  HorizontalSeparator hoz_sep_main_header;


  // Sim Button
  Button btn_start_simulation;





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
