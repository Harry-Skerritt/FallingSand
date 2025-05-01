#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
 public:
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


 void spawnNewSand();
 sf::RectangleShape drawCell(sf::Vector2i pos, bool isSand = false);

 bool within_cols(int x) const;

 bool within_rows(int y) const;


 private:
  sf::RenderWindow& window;

  sf::Vector2f window_size;

  std::vector<std::vector<int>> grid;

  float sand_size = 5;
  int cell_amt_x, cell_amt_y;

 // Mouse drag
 bool is_left_mouse_down = false;
 sf::Vector2f prev_mouse_pos;


};

#endif // SPACEINVADERS_GAME_H
