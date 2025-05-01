
#include "Game.h"
#include <iostream>
#include <math.h>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::within_cols(int x) const {
  return x >= 0 && x < cell_amt_x;
}

bool Game::within_rows(int y) const {
  return y >= 0 && y < cell_amt_y;
}

bool Game::init()
{
  window_size = static_cast<sf::Vector2f>(window.getSize());

  cell_amt_x = window_size.x / sand_size;
  cell_amt_y = window_size.y / sand_size;

  grid.resize(cell_amt_x, std::vector<int>(cell_amt_y, 0));

  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = 0; j < cell_amt_y; j++) {
      grid[i][j] = 0;
    }
  }

  return true;
}

void Game::update(float dt)
{
  auto new_grid = grid;

  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = cell_amt_y - 2; j >= 0; j--) {  // Iterate from bottom to top
      auto grid_current = grid[i][j];

      if (grid_current == 1) {  // Only move sand (value 1)
        // Check the space directly below
        int below_current = (within_rows(j + 1)) ? grid[i][j + 1] : -1;

        // Randomly select left or right diagonal direction
        int dir = (rand() % 2 == 0) ? 1 : -1;
        int below_a = -1;
        int below_b = -1;

        // Check the left and right diagonal space
        if (within_cols(i + dir) && within_rows(j + 1)) {
          below_a = grid[i + dir][j + 1];
        }

        if (within_cols(i - dir) && within_rows(j + 1)) {
          below_b = grid[i - dir][j + 1];
        }

        // If the space directly below is empty, move the sand down
        if (below_current == 0) {
          new_grid[i][j + 1] = grid_current;
          new_grid[i][j] = 0;  // Clear the old position
        }
        // If the left diagonal space is empty, move the sand there
        else if (below_a == 0) {
          new_grid[i + dir][j + 1] = grid_current;
          new_grid[i][j] = 0;  // Clear the old position
        }
        // If the right diagonal space is empty, move the sand there
        else if (below_b == 0) {
          new_grid[i - dir][j + 1] = grid_current;
          new_grid[i][j] = 0;  // Clear the old position
        }
      }
    }
  }

  grid = new_grid;  // Update the grid with new positions

  // Spawn new sand if the mouse is pressed
  if (is_left_mouse_down) {
    spawnNewSand();
  }
}


void Game::spawnNewSand() {
  sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_pixel);

  float dx = mouse_world.x - prev_mouse_pos.x;
  float dy = mouse_world.y - prev_mouse_pos.y;
  float distance = std::sqrt(dx * dx + dy * dy);
  int steps = static_cast<int>(distance / (sand_size * 0.5f)) + 1;

  for (int s = 0; s <= steps; ++s) {
    float x = prev_mouse_pos.x + dx * (s / static_cast<float>(steps));
    float y = prev_mouse_pos.y + dy * (s / static_cast<float>(steps));
    int i = static_cast<int>(x / sand_size);
    int j = static_cast<int>(y / sand_size);

    if (i >= 0 && i < cell_amt_x && j >= 0 && j < cell_amt_y) {
      grid[i][j] = 1;
    }
  }

  prev_mouse_pos = mouse_world;
}

sf::RectangleShape Game::drawCell(sf::Vector2i interator, bool isSand) {
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f(sand_size, sand_size));

  if (isSand) {
    rect.setFillColor(sf::Color(209, 166, 109, 255));
  }
  else {
    rect.setFillColor(sf::Color::Transparent);
  }


  rect.setPosition(interator.x * sand_size, interator.y * sand_size);

  return rect;
}

void Game::render()
{

  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = 0; j < cell_amt_y; j++) {
      if (grid[i][j] > 0) {
        window.draw(drawCell({i, j}, true));
      } else {
        window.draw(drawCell({i, j}, false));
      }
    }
  }

}

  void Game::mouseClicked(sf::Event event)
  {
    if (event.mouseButton.button == sf::Mouse::Left) {
      is_left_mouse_down = true;
      prev_mouse_pos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
    }
  }

  void Game::mouseReleased(sf::Event event)
  {
    if (event.mouseButton.button == sf::Mouse::Left) {
      is_left_mouse_down = false;
    }
  }

void Game::keyPressed(sf::Event event)
{

  if (event.key.code == sf::Keyboard::R) {
    // Reset
    for (int i = 0; i < cell_amt_x; i++) {
      for (int j = 0; j < cell_amt_y; j++) {
        grid[i][j] = 0;
      }
    }
  }

}

void Game::keyReleased(sf::Event event)
{

}

void Game::mouseScroll(sf::Event event)
{

}

void Game::mouseDragged(sf::Event event)
{

}


