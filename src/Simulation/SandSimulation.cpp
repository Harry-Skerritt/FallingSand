#include "SandSimulation.h"
#include <iostream>
#include <cmath>
#include <ctime>

SandSimulation::SandSimulation(sf::RenderWindow& game_window)
    : window(game_window) {}

SandSimulation::~SandSimulation() {}

bool SandSimulation::init() {
    std::cout << "INIT SIM" << std::endl;

    window_size = static_cast<sf::Vector2f>(window.getSize());
    cell_amt_x = window.getSize().x / sand_size;
    cell_amt_y = window.getSize().y / sand_size;

    sand_grid.resize(cell_amt_x, std::vector<int>(cell_amt_y, 0));
    sand_verts.setPrimitiveType(sf::Quads);
    sand_verts.resize(cell_amt_x * cell_amt_y * 4);

    background.setSize(window_size);

    std::cout << "Sand Sim: \nImage: " << use_image_colour
              << "\n Solid Colour: " << use_solid_colour
              << "\n Rainbow: " << use_rainbow << std::endl;

    if (use_image_colour) {
        loadImageForSand(image_path);
        setBackgroundColour(getAverageColour(album));
    } else if (use_solid_colour && !use_rainbow) {
        setBackgroundColour(background_colour);
    } else {
        setBackgroundColour(sf::Color::Black);
    }

    return true;
}

void SandSimulation::update(float dt) {
    auto new_grid = sand_grid;

    for (int i = 0; i < cell_amt_x; ++i) {
        for (int j = cell_amt_y - 2; j >= 0; --j) {
            if (sand_grid[i][j] == 1) {
                int dir = (rand() % 2 == 0) ? 1 : -1;
                if (within_rows(j + 1)) {
                    if (sand_grid[i][j + 1] == 0) {
                        new_grid[i][j + 1] = 1;
                        new_grid[i][j] = 0;
                    } else if (within_cols(i + dir) && sand_grid[i + dir][j + 1] == 0) {
                        new_grid[i + dir][j + 1] = 1;
                        new_grid[i][j] = 0;
                    } else if (within_cols(i - dir) && sand_grid[i - dir][j + 1] == 0) {
                        new_grid[i - dir][j + 1] = 1;
                        new_grid[i][j] = 0;
                    }
                }
            }
        }
    }

    sand_grid = new_grid;

    if (use_rainbow) {
        int t = static_cast<int>(std::time(nullptr) % 255);
        sand_colour = sf::Color(t, 255 - t, (t + 128) % 255);
    }
}

void SandSimulation::draw(sf::RenderWindow& window) {
    window.draw(background);

    sand_verts.clear();
    for (int i = 0; i < cell_amt_x; ++i) {
        for (int j = 0; j < cell_amt_y; ++j) {
            if (sand_grid[i][j] == 1) {
                float x = i * sand_size;
                float y = j * sand_size;

                sf::Color draw_colour = sand_colour;

                if (use_image_colour) {
                    draw_colour = colour_data_image.getPixel(i, j);
                } else if (use_rainbow) {
                    int t = static_cast<int>(std::time(nullptr) % 255);
                    draw_colour = sf::Color(t, 255 - t, (t + 128) % 255);
                }

                sand_verts.append({sf::Vector2f(x, y), draw_colour});
                sand_verts.append({sf::Vector2f(x + sand_size, y), draw_colour});
                sand_verts.append({sf::Vector2f(x + sand_size, y + sand_size), draw_colour});
                sand_verts.append({sf::Vector2f(x, y + sand_size), draw_colour});
            }
        }
    }

    window.draw(sand_verts);
}

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

void SandSimulation::resetGrid() {
    for (auto& col : sand_grid)
        std::fill(col.begin(), col.end(), 0);
}

float SandSimulation::getFilledPercentage() const {
    int filled = getSandParticleCount();
    return 100.f * filled / (cell_amt_x * cell_amt_y);
}

int SandSimulation::getSandParticleCount() const {
    int count = 0;
    for (const auto& col : sand_grid)
        for (int cell : col)
            if (cell == 1) ++count;
    return count;
}

void SandSimulation::setBackgroundColour(sf::Color colour) {
    background_colour = colour;
    background.setFillColor(colour);
}

void SandSimulation::setSandColour(sf::Color colour) {
    sand_colour = colour;
}

bool SandSimulation::loadImageForSand(const std::string& path) {
    if (!album.loadFromFile(path)) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return false;
    }
    cachePixels();
    return true;
}

void SandSimulation::cachePixels() {
    sf::Vector2u img_size = album.getSize();
    if (img_size.x == 0 || img_size.y == 0) return;

    colour_data_image.create(cell_amt_x, cell_amt_y);
    for (int x = 0; x < cell_amt_x; ++x) {
        for (int y = 0; y < cell_amt_y; ++y) {
            unsigned int src_x = static_cast<unsigned int>((x / static_cast<float>(cell_amt_x)) * img_size.x);
            unsigned int src_y = static_cast<unsigned int>((y / static_cast<float>(cell_amt_y)) * img_size.y);
            colour_data_image.setPixel(x, y, album.getPixel(std::min(src_x, img_size.x - 1), std::min(src_y, img_size.y - 1)));
        }
    }
    colour_data_texture.loadFromImage(colour_data_image);
    colour_data_texture.setSmooth(false);
    std::cout << "Cached Pixels" << std::endl;
}

sf::Color SandSimulation::getAverageColour(const sf::Image& image) {
    uint64_t r = 0, g = 0, b = 0;
    unsigned w = image.getSize().x, h = image.getSize().y;

    for (unsigned x = 0; x < w; ++x)
        for (unsigned y = 0; y < h; ++y) {
            sf::Color c = image.getPixel(x, y);
            r += c.r; g += c.g; b += c.b;
        }

    uint64_t total = w * h;
    return sf::Color(static_cast<sf::Uint8>(r / total),
                     static_cast<sf::Uint8>(g / total),
                     static_cast<sf::Uint8>(b / total));
}

bool SandSimulation::within_cols(int x) const { return x >= 0 && x < cell_amt_x; }
bool SandSimulation::within_rows(int y) const { return y >= 0 && y < cell_amt_y; }
