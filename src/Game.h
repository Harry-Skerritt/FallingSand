#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
 public:

 const static int TARGET_FPS = 144;

  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void mouseScroll(sf::Event event);
  void mouseDragged(sf::Event event);



 // Sand related
 void spawnNewSand();
 sf::RectangleShape drawCell(sf::Vector2i pos, sf::Color colour);
 float getFilledPercentage() const;

 // Checks
 bool within_cols(int x) const;
 bool within_rows(int y) const;

 // Photo related
 bool loadAlbum(std::string loc);
 void cachePixels();
 sf::Color getAverageColour(const sf::Image& image);

 int getSandParticleCount() const;

 void calcFPS();

 private:
  sf::RenderWindow& window;

  sf::Vector2f window_size;

  std::vector<std::vector<int>> grid;
  std::vector<std::vector<sf::Color>> grid_colours;
  std::vector<std::vector<sf::Color>> image_colours;

  float sand_size = 2; // "resolution"
  int cell_amt_x, cell_amt_y;

 int spawn_size = 1; // "5x5 grid"
 int multiple_spawn_change = 40; // 40%
 bool spawn_multiple = true;

 // Map to picture
 sf::Image album;
 sf::Texture colour_data_texture;
 sf::Image colour_data_image;

 // Sand
 sf::VertexArray sand_verts;

 sf::Color background_colour;
 sf::RectangleShape background;

 // Info
 sf::Font font;
 sf::Text fps_text;
 sf::Text particle_text;
 sf::Text spawn_text;
 bool draw_info = true;

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
