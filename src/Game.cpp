
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



bool Game::init()
{
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

  //
  calcFPS();
  fps_text.setString("FPS: " + std::to_string(static_cast<float>(static_cast<int>(fps * 10.)) / 10.));
  fps_text.setPosition(10, 10);
  particle_text.setString("Sand: " + std::to_string(000));
  particle_text.setPosition(10, fps_text.getPosition().y + fps_text.getGlobalBounds().height + 10);
  spawn_text.setString("Spawn Size: " + std::to_string(0));
  spawn_text.setPosition(10, particle_text.getPosition().y + particle_text.getGlobalBounds().height + 10);
}

void Game::render()
{

}

void Game::openImageFileDialog() {
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

void Game::calcFPS() {
  frameCount++;

  if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
    fps = frameCount / fpsClock.getElapsedTime().asSeconds();
    frameCount = 0;
    fpsClock.restart();
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
  if (event.key.code == sf::Keyboard::L) {
    openImageFileDialog();
  }

  if (event.key.code == sf::Keyboard::S) {
    show_simulation = !show_simulation;
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


