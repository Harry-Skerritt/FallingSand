
#include "SettingsWindow.h"
#include <iostream>
#include <math.h>

#include "SFML/Audio/SoundRecorder.hpp"

SettingsWindow::SettingsWindow(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

SettingsWindow::~SettingsWindow()
{

}



bool SettingsWindow::init()
{
  if (!font.loadFromFile("../Data/Fonts/CourierPrime-Bold.ttf")) {
    std::cerr << "Failed to load font" << std::endl;
    return false;
  }

  // Start the gui
  sf::Vector2f window_size = sf::Vector2f(window.getSize());
  sf::Color button_colour = sf::Color(15, 89, 0, 255);
  sf::Color button_hover = sf::Color(109, 179, 95, 255);

  // Header
  if (!icon_texture.loadFromFile("../Data/icon.png"))
  {
    std::cout << "Failed to load icon texture" << std::endl;
    return false;
  }
  icon_sprite.setTexture(icon_texture);
  icon_sprite.setScale(0.25f, 0.25f);
  icon_sprite.setPosition(window_size.x * 0.032f, window_size.y * 0.02f);

  title_text.setFont(font);
  title_text.setString("Falling Sand Simulator");
  title_text.setFillColor(sf::Color::White);
  title_text.setCharacterSize(window_size.y * 0.038f);
  title_text.setStyle(sf::Text::Bold);
  title_text.setPosition(window_size.x * 0.18f, window_size.y * 0.026f);

  subtitle_text.setFont(font);
  subtitle_text.setString("by Harry Skerritt");
  subtitle_text.setFillColor(sf::Color::White);
  subtitle_text.setCharacterSize(window_size.y * 0.02f);
  subtitle_text.setPosition(window_size.x * 0.18f, window_size.y * 0.068f);

  hoz_sep_main_header.init({window_size.x * 0.18f, window_size.y * 0.11f}, 342, 2, sf::Color::White);


  // Sim Control Button
  btn_start_simulation.init({window_size.x * 0.572f, window_size.y * 0.13f},
    {window_size.x*0.4f, window_size.y*0.06f}, button_colour, "Launch Sim", &font);

  btn_start_simulation.setButtonColours(button_colour, button_hover);
  btn_start_simulation.setTextColor(sf::Color::White);


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

void SettingsWindow::update(float dt)
{
  sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_pixel);

  btn_start_simulation.update(mouse_world, dt);
}

void SettingsWindow::render()
{
  // Header
  window.draw(icon_sprite);
  window.draw(title_text);
  window.draw(subtitle_text);
  window.draw(hoz_sep_main_header);

  btn_start_simulation.draw(window);

}

void SettingsWindow::openImageFileDialog() {
  const char* filter_patters[] = {"*.png",  "*.jpg", "*.jpeg" };
  const char* file = tinyfd_openFileDialog(
    "Select an image",
    "",
    3,
    filter_patters,
    "Image Files",
    0);

  if (file) {
    std::cout << file << std::endl;
    //if (loadAlbum(file)) {
    //  resetGrid();
    //  cachePixels();
     // background_colour= getAverageColour(album);
     // background.setFillColor(setBackgroundColour(background_colour));
    //}
  }
  else
    std::cout << "No File" << std::endl;
}

void SettingsWindow::calcFPS() {
  frameCount++;

  if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
    fps = frameCount / fpsClock.getElapsedTime().asSeconds();
    frameCount = 0;
    fpsClock.restart();
  }

}



void SettingsWindow::mouseClicked(sf::Event event)
{

  sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_pixel);

  if (event.mouseButton.button == sf::Mouse::Left) {
    is_left_mouse_down = true;
    prev_mouse_pos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
  }

  if (btn_start_simulation.getGlobalBounds().contains(mouse_world))
  {
    show_simulation = !show_simulation;
    if (show_simulation)
    {
      btn_start_simulation.setString("Abort Sim");
    }
    else
    {
      btn_start_simulation.setString("Launch Sim");
    }
  }

}

void SettingsWindow::mouseReleased(sf::Event event)
  {
    if (event.mouseButton.button == sf::Mouse::Left) {
      is_left_mouse_down = false;
    }
  }

void SettingsWindow::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::L) {
    openImageFileDialog();
  }

  if (event.key.code == sf::Keyboard::S) {
    show_simulation = !show_simulation;
  }

}

void SettingsWindow::keyReleased(sf::Event event)
{

}

void SettingsWindow::mouseScroll(sf::Event event)
{

}

void SettingsWindow::mouseDragged(sf::Event event)
{

}


