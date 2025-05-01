
#include "Game.h"
#include <iostream>
#include <math.h>

#include "SFML/Audio/SoundRecorder.hpp"

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

bool Game::loadAlbum(std::string loc) {
  if (!album.loadFromFile(loc)) {
    std::cerr << "Failed to load album: " << loc << std::endl;
    return false;
  }
  return true;
}

void Game::cachePixels() {
  sf::Vector2u img_size = album.getSize();
  image_colours.resize(cell_amt_x, std::vector<sf::Color>(cell_amt_y));

  colour_data_image.create(cell_amt_x, cell_amt_y);

  for (int x = 0; x < cell_amt_x; x++) {
    for (int y = 0; y < cell_amt_y; y++) {
      unsigned int src_x = static_cast<unsigned int>((x / static_cast<float>(cell_amt_x)) * img_size.x);
      unsigned int src_y = static_cast<unsigned int>((y / static_cast<float>(cell_amt_y)) * img_size.y);

      src_x = std::min(src_x, img_size.x - 1);
      src_y = std::min(src_y, img_size.y - 1);

      sf::Color col = album.getPixel(src_x, src_y);
      image_colours[x][y] = col;
      colour_data_image.setPixel(x, y, col);
    }
  }

  colour_data_texture.loadFromImage(colour_data_image);
  colour_data_texture.setSmooth(false);

}

sf::Color Game::getAverageColour(const sf::Image& image) {
  unsigned int width = image.getSize().x;
  unsigned int height = image.getSize().y;

  // Variables to store the sum of each color channel
  unsigned long long redSum = 0;
  unsigned long long greenSum = 0;
  unsigned long long blueSum = 0;

  // Loop through all pixels and accumulate color values
  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int y = 0; y < height; ++y) {
      sf::Color pixelColor = image.getPixel(x, y);
      redSum += pixelColor.r;
      greenSum += pixelColor.g;
      blueSum += pixelColor.b;
    }
  }

  // Calculate the average color
  unsigned int totalPixels = width * height;
  sf::Color avgColor(
      static_cast<sf::Uint8>(redSum / totalPixels),
      static_cast<sf::Uint8>(greenSum / totalPixels),
      static_cast<sf::Uint8>(blueSum / totalPixels)
  );

  return avgColor;
}


float Game::getFilledPercentage() const {
  int filled_cells = 0;
  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = 0; j < cell_amt_y; j++) {
      if (grid[i][j] == 1) {  // Sand cell
        filled_cells++;
      }
    }
  }

  float fill_percentage = (filled_cells / static_cast<float>(cell_amt_x * cell_amt_y)) * 100;

  return fill_percentage;
}


bool Game::init()
{
  window_size = static_cast<sf::Vector2f>(window.getSize());

  cell_amt_x = window_size.x / sand_size;
  cell_amt_y = window_size.y / sand_size;

  grid.resize(cell_amt_x, std::vector<int>(cell_amt_y, 0));
  grid_colours.resize(cell_amt_x, std::vector<sf::Color>(cell_amt_y, sf::Color::Transparent));
  image_colours.resize(cell_amt_x, std::vector<sf::Color>(cell_amt_y, sf::Color::Black));

  background.setSize(window_size);
  background.setFillColor(sf::Color::White);

  if (loadAlbum("../Data/rainbow.jpg")) {
    cachePixels();
  }
  else {
    std::cerr << "Failed to load and cache album" << std::endl;
    return false;
  }

  sand_verts.setPrimitiveType(sf::Quads);
  sand_verts.resize(cell_amt_x * cell_amt_y * 4);

  background_colour = getAverageColour(album);
  background.setFillColor(sf::Color(background_colour.r * 0.4f, background_colour.g * 0.4f, background_colour.b * 0.4f, background_colour.a));

  if (!font.loadFromFile("../Data/Fonts/CourierPrime-Bold.ttf")) {
    std::cerr << "Failed to load font" << std::endl;
    return false;
  }

  // Info
  fps_text.setFont(font);
  fps_text.setCharacterSize(20);
  fps_text.setFillColor(sf::Color::White);
  fps_text.setPosition(10, 10);
  fps_text.setString("FPS: 000");

  particle_text.setFont(font);
  particle_text.setCharacterSize(20);
  particle_text.setFillColor(sf::Color::White);
  particle_text.setPosition(10, fps_text.getPosition().y + fps_text.getGlobalBounds().height + 10);
  particle_text.setString("Sand: 000000");

  spawn_text.setFont(font);
  spawn_text.setCharacterSize(20);
  spawn_text.setFillColor(sf::Color::White);
  spawn_text.setPosition(10, particle_text.getPosition().y + particle_text.getGlobalBounds().height + 10);
  spawn_text.setString("Spawn Size: 0");


  // Shader
  if (!sand_shader.loadFromFile("../Data/Shaders/sand_shader.frag", sf::Shader::Fragment)) {
    std::cerr << "Failed to load sand_shader.frag" << std::endl;
    shader_loaded = false;
  }
  else {
    shader_loaded = true;
  }


  return true;
}

void Game::update(float dt)
{
    auto new_grid = grid;
    auto new_grid_colours = grid_colours;

    for (int i = 0; i < cell_amt_x; i++) {
        for (int j = cell_amt_y - 2; j >= 0; j--) {

            auto grid_current = grid[i][j];

            if (grid_current == 1) {
                int below_current = (within_rows(j + 1)) ? grid[i][j + 1] : -1;

                int dir = (rand() % 2 == 0) ? 1 : -1;
                int below_a = -1;
                int below_b = -1;

                if (within_cols(i + dir) && within_rows(j + 1)) {
                    below_a = grid[i + dir][j + 1];
                }

                if (within_cols(i - dir) && within_rows(j + 1)) {
                    below_b = grid[i - dir][j + 1];
                }

              if (below_current == 0) {
                new_grid[i][j + 1] = grid_current;
                new_grid_colours[i][j + 1] = image_colours[i][j + 1];
                new_grid[i][j] = 0;
                new_grid_colours[i][j] = sf::Color::Transparent;
              }
              else if (below_a == 0) {
                new_grid[i + dir][j + 1] = grid_current;
                new_grid_colours[i + dir][j + 1] = image_colours[i + dir][j + 1];
                new_grid[i][j] = 0;
                new_grid_colours[i][j] = sf::Color::Transparent;
              }
              else if (below_b == 0) {
                new_grid[i - dir][j + 1] = grid_current;
                new_grid_colours[i - dir][j + 1] = image_colours[i - dir][j + 1];
                new_grid[i][j] = 0;
                new_grid_colours[i][j] = sf::Color::Transparent;
              }
            }
        }
    }

    grid = new_grid;
    grid_colours = new_grid_colours;

    if (is_left_mouse_down) {
        spawnNewSand();
    }

  //
  calcFPS();
  fps_text.setString("FPS: " + std::to_string(static_cast<float>(static_cast<int>(fps * 10.)) / 10.));
  fps_text.setPosition(10, 10);
  particle_text.setString("Sand: " + std::to_string(getSandParticleCount()));
  particle_text.setPosition(10, fps_text.getPosition().y + fps_text.getGlobalBounds().height + 10);
  spawn_text.setString("Spawn Size: " + std::to_string(spawn_size));
  spawn_text.setPosition(10, particle_text.getPosition().y + particle_text.getGlobalBounds().height + 10);
}

void Game::calcFPS() {
  frameCount++;

  if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
    fps = frameCount / fpsClock.getElapsedTime().asSeconds();
    frameCount = 0;
    fpsClock.restart();
  }

}


void Game::spawnNewSand() {
  sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_pixel);

  if (spawn_multiple) {
    // Spawn Multiple
    int center_col = static_cast<int>(mouse_world.x / sand_size);
    int center_row = static_cast<int>(mouse_world.y / sand_size);
    int extent = static_cast<int>(std::floor(spawn_size / 2));

    for (int i = -extent; i <= extent; i++) {
      for (int j = -extent; j <= extent; j++) {
        if (rand() % 100 < multiple_spawn_change) {
          int col = center_col + i;
          int row = center_row + j;

          if (within_cols(col) && within_rows(row)) {
            grid[col][row] = 1;
          }
        }
      }
    }
  }
  else {
    // Spawn One
    float dx = mouse_world.x - prev_mouse_pos.x;
    float dy = mouse_world.y - prev_mouse_pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    int steps = static_cast<int>(distance / (sand_size * 0.5f)) + 1;

    for (int s = 0; s <= steps; ++s) {
      float x = prev_mouse_pos.x + dx * (s / static_cast<float>(steps));
      float y = prev_mouse_pos.y + dy * (s / static_cast<float>(steps));
      int i = static_cast<int>(x / sand_size);
      int j = static_cast<int>(y / sand_size);

      if (within_cols(i) && within_rows(j)) {
        grid[i][j] = 1;
      }
    }

    prev_mouse_pos = mouse_world;
  }
}

int Game::getSandParticleCount() const {
  int count = 0;
  for (int i = 0; i < cell_amt_x; ++i) {
    for (int j = 0; j < cell_amt_y; ++j) {
      if (grid[i][j] > 0) {
        ++count;
      }
    }
  }
  return count;
}

sf::RectangleShape Game::drawCell(sf::Vector2i interator, sf::Color colour) {
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f(sand_size, sand_size));
  rect.setFillColor(colour);
  rect.setPosition(interator.x * sand_size, interator.y * sand_size);

  return rect;
}

void Game::render()
{
  window.draw(background);

  sand_verts.clear();
  sand_verts.setPrimitiveType(sf::Quads);

  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = 0; j < cell_amt_y; j++) {
      if (grid[i][j] > 0) {
        float x = i * sand_size;
        float y = j * sand_size;

        sf::Vertex v0(sf::Vector2f(x, y), sf::Vector2f(i, j));         // top-left
        sf::Vertex v1(sf::Vector2f(x + sand_size, y), sf::Vector2f(i + 1, j)); // top-right
        sf::Vertex v2(sf::Vector2f(x + sand_size, y + sand_size), sf::Vector2f(i + 1, j + 1)); // bottom-right
        sf::Vertex v3(sf::Vector2f(x, y + sand_size), sf::Vector2f(i, j + 1)); // bottom-left

        sand_verts.append(v0);
        sand_verts.append(v1);
        sand_verts.append(v2);
        sand_verts.append(v3);
      }
    }
  }

  sf::RenderStates states;
  states.shader = &sand_shader; // <- You load this in init
  states.texture = &colour_data_texture;

  window.draw(sand_verts, states);


  if (draw_info) {
    window.draw(fps_text);
    window.draw(particle_text);
    window.draw(spawn_text);
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

  if (event.key.code == sf::Keyboard::F3) {
    draw_info = !draw_info;
  }

}

void Game::keyReleased(sf::Event event)
{

}

void Game::mouseScroll(sf::Event event)
{
  if (event.mouseWheelScroll.delta > 0) {
    spawn_size++;
  }
  else if (event.mouseWheelScroll.delta < 0) {
    if (spawn_size < 1) {
      spawn_size = 1;
    }
    else {
      spawn_size--;
    }
  }


}

void Game::mouseDragged(sf::Event event)
{

}


