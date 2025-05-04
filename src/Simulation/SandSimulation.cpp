//
// Created by Harry on 02/05/2025.
//

#include "SandSimulation.h"
#include <iostream>
#include <math.h>

SandSimulation::SandSimulation(sf::RenderWindow& game_window): window(game_window) {
}

SandSimulation::~SandSimulation() {}



bool SandSimulation::init() {
  std::cout << "INIT SIM" << std::endl;
  // Set the grid vars up correctly
  window_size = static_cast<sf::Vector2f>(window.getSize());
  cell_amt_x = window.getSize().x / sand_size;
  cell_amt_y = window.getSize().y / sand_size;

  // Initialisation of the grid
  sand_grid.resize(cell_amt_x, std::vector<int>(cell_amt_y, 0)); // Empty grid by default

  // Set up vertex array for rendering
  sand_verts.setPrimitiveType(sf::Quads);
  sand_verts.resize(cell_amt_x * cell_amt_y * 4);

  // Setup Background
  background.setSize(window_size);

  std::cout << "Sand Sim: \nImage: " << use_image_colour << "\n Solid Colour: " << use_solid_colour << "\n Rainbow: " << use_rainbow << std::endl;

  if (use_image_colour)
  {
    loadImageForSand(image_path);
    setBackgroundColour(getAverageColour(album)); // Average color of the image as background
  }
  else if (use_solid_colour && !use_rainbow)
  {
    setBackgroundColour(background_colour);
  }
  else
  {
    setBackgroundColour(sf::Color::Black); // Black background for rainbow mode
  }

  return true;
}

void SandSimulation::update(float dt)
{
  auto new_grid = sand_grid;
  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = cell_amt_y - 2; j >= 0; j--) {

      auto grid_current = sand_grid[i][j];

      if (grid_current == 1) {
        int below_current = (within_rows(j + 1)) ? sand_grid[i][j + 1] : -1;

        int dir = (rand() % 2 == 0) ? 1 : -1;
        int below_a = -1;
        int below_b = -1;

        if (within_cols(i + dir) && within_rows(j + 1)) {
          below_a = sand_grid[i + dir][j + 1];
        }

        if (within_cols(i - dir) && within_rows(j + 1)) {
          below_b = sand_grid[i - dir][j + 1];
        }

        if (below_current == 0) {
          new_grid[i][j + 1] = grid_current;
          new_grid[i][j] = 0;
        }
        else if (below_a == 0) {
          new_grid[i + dir][j + 1] = grid_current;
          new_grid[i][j] = 0;
        }
        else if (below_b == 0) {
          new_grid[i - dir][j + 1] = grid_current;
          new_grid[i][j] = 0;
        }
      }
    }
  }

  sand_grid = new_grid;

  // Handle rainbow effect if enabled
  if (use_rainbow) {
    int timeFactor = static_cast<int>(std::time(0) % 255); // Time-based color change
    sand_colour = sf::Color(timeFactor, 255 - timeFactor, (timeFactor + 128) % 255);
  }
}

void SandSimulation::draw(sf::RenderWindow &window) {
  window.draw(background);

  sand_verts.clear();
  sand_verts.setPrimitiveType(sf::Quads);

  for (int i = 0; i < cell_amt_x; i++) {
    for (int j = 0; j < cell_amt_y; j++) {
      if (sand_grid[i][j] > 0) {
        float x = i * sand_size;
        float y = j * sand_size;

        sf::Color draw_colour = sand_colour;

        // If using image colors, override with image color
        if (use_image_colour) {
          draw_colour = colour_data_image.getPixel(i, j);
        }
        // If rainbow mode is enabled, use the rainbow color
        else if (use_rainbow) {
          int timeFactor = static_cast<int>(std::time(0) % 255);
          draw_colour = sf::Color(timeFactor, 255 - timeFactor, (timeFactor + 128) % 255);
        }

        // If solid color mode is active, use the set sand color
        if (!use_solid_colour && !use_image_colour && !use_rainbow) {
          draw_colour = sand_colour;
        }

        // Create vertices for drawing the sand particle
        sf::Vertex v0(sf::Vector2f(x, y), draw_colour);                         // top-left
        sf::Vertex v1(sf::Vector2f(x + sand_size, y), draw_colour);            // top-right
        sf::Vertex v2(sf::Vector2f(x + sand_size, y + sand_size), draw_colour); // bottom-right
        sf::Vertex v3(sf::Vector2f(x, y + sand_size), draw_colour);             // bottom-left

        sand_verts.append(v0);
        sand_verts.append(v1);
        sand_verts.append(v2);
        sand_verts.append(v3);
      }
    }
  }

  window.draw(sand_verts);
}


// Colours
void SandSimulation::setBackgroundColour(sf::Color colour) {
    if (use_solid_colour) {
        background_colour = colour;
    }
    else {
        background_colour = sf::Color::Transparent;
    }

    background.setFillColor(background_colour);
}


void SandSimulation::setSandColour(sf::Color colour) {
    if (use_solid_colour) {
        sand_colour = colour;
    }
    else {
        sand_colour = sf::Color::White;
    }
}





// Sand
void SandSimulation::spawnNewSand(sf::Vector2f prev_mouse_pos) {
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
            sand_grid[col][row] = 1;
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
        sand_grid[i][j] = 1;
      }
    }

    prev_mouse_pos = mouse_world;
  }
}

int SandSimulation::getSandParticleCount() const {
    int count = 0;
    for (int i = 0; i < cell_amt_x; ++i) {
        for (int j = 0; j < cell_amt_y; ++j) {
            if (sand_grid[i][j] > 0) {
                ++count;
            }
        }
    }
    return count;
}

void SandSimulation::resetGrid() {
    for (int i = 0; i < cell_amt_x; i++) {
        for (int j = 0; j < cell_amt_y; j++) {
            sand_grid[i][j] = 0;
        }
    }
}

float SandSimulation::getFilledPercentage() const {
    int filled_cells = getSandParticleCount();
    float fill_percentage = (filled_cells / static_cast<float>(cell_amt_x * cell_amt_y)) * 100;
    return fill_percentage;
}

// Image
bool SandSimulation::loadAlbum(std::string loc) {
    if (!album.loadFromFile(loc)) {
        std::cerr << "Failed to load album from " << loc << std::endl;
        return false;
    }
  std::cout << "Loaded image from " << loc << std::endl;
    return true;
}

bool SandSimulation::loadImageForSand(const std::string& path)
{
  if (!loadAlbum(path)) return false;
  cachePixels();
  return true;
}


void SandSimulation::cachePixels() {
  sf::Vector2u img_size = album.getSize();
  if (img_size.x == 0 || img_size.y == 0) return;

  colour_data_image.create(cell_amt_x, cell_amt_y);

  for (int x = 0; x < cell_amt_x; x++) {
    for (int y = 0; y < cell_amt_y; y++) {
      unsigned int src_x = static_cast<unsigned int>((x / static_cast<float>(cell_amt_x)) * img_size.x);
      unsigned int src_y = static_cast<unsigned int>((y / static_cast<float>(cell_amt_y)) * img_size.y);

      src_x = std::min(src_x, img_size.x - 1);
      src_y = std::min(src_y, img_size.y - 1);

      sf::Color col = album.getPixel(src_x, src_y);
      colour_data_image.setPixel(x, y, col);
    }
  }

  colour_data_texture.loadFromImage(colour_data_image);
  colour_data_texture.setSmooth(false);

  std::cout << "Cached Pixels" << std::endl;
}

sf::Color SandSimulation::getAverageColour(const sf::Image& image) {
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

sf::Color SandSimulation::getAverageBackgroundColour(sf::Color colour) {
    sf::Color colour_new = sf::Color(colour.r * 0.4f, colour.g * 0.4f, colour.b * 0.4f, colour.a);
    return colour_new;
}


// Helper Funs - PRIVATE
bool SandSimulation::within_cols(int x) const {
    return x >= 0 && x < cell_amt_x;
}
bool SandSimulation::within_rows(int y) const {
    return  y >= 0 && y < cell_amt_y;
}
