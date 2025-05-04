
#include "SettingsWindow.h"
#include <iostream>
#include <math.h>
#include <filesystem>

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
  if (!font.loadFromFile("../Data/Fonts/CourierPrime-Regular.ttf")) {
    std::cerr << "Failed to load font" << std::endl;
    return false;
  }

  // Start the gui
  sf::Vector2f window_size = sf::Vector2f(window.getSize());

  // ### Header ###
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


  // ### SOURCE HEADING ###
  hdn_source.init(&font, "Source", {window_size.x * 0.032f, window_size.y * 0.16f}, 470);
  hdn_source.setCharacterSize(window_size.y * 0.038f);

  rb_image = new RadioButton(&source_button_group,
    {window_size.x * 0.032f, window_size.y * 0.23f},
    10, font, "Image");

  rb_solid_colour = new RadioButton(&source_button_group,
    {window_size.x * 0.032f, window_size.y * 0.29f},
    10, font, "Solid Colour");

  rb_image->setSelected(use_image);
  rb_solid_colour->setSelected(use_solid_colour);

  rb_image->onSelect = [this]()
  {
    std::cout << "Image selected\n";
    use_image = true;
    use_solid_colour = false;
  };
  rb_solid_colour->onSelect = [this]()
  {
    std::cout << "Colour selected\n";
    use_solid_colour = true;
    use_image = false;
  };

  ver_sep_source.init({window_size.x *0.35f, window_size.y * 0.22f}, 72, 2, sf::Color::White);

  // File
  file_label.init({window_size.x * 0.378f, window_size.y * 0.225f}, "File:", &font, (window_size.x * 0.03f));

  file_name.init({window_size.x * 0.476f, window_size.y * 0.225f}, file_path_name, &font, (window_size.x * 0.03f), sf::Color(0, 58, 15, 255));
  file_name.setTruncateType(TruncateType::Ellipsis);
  file_name.setMaxWidth(130);

  btn_choose_file.init({window_size.x * 0.758f, window_size.y * 0.225f},
     {window_size.x*0.212f, window_size.y*0.031f}, button_colour, "Choose", &font);
  btn_choose_file.setButtonColours(button_colour, button_hover);
  btn_choose_file.setTextColor(sf::Color::White);

  // Colour
  colour_label.init({window_size.x * 0.378f, window_size.y * 0.287f}, "Colour:", &font, (window_size.x * 0.03f));

  colour_picker = new ColourPicker({window_size.x * 0.534f, window_size.y * 0.287f}, {20, 20}, picked_colour, &font);
  picked_colour_hex = colour_picker->getColourHex();

  rainbow_selection = new CheckBox({window_size.x * 0.758f, window_size.y * 0.287f}, {20, 20}, "Rainbow", &font);










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

  // Header
  btn_start_simulation.update(mouse_world, dt);

  // Source Header
  rb_image->update(mouse_world);
  rb_solid_colour->update(mouse_world);

  if (use_image)
  {
    // Using Image
    file_label.setFillColor(sf::Color::White);
    file_name.setFillColor(sf::Color(0, 58, 15, 255));
    btn_choose_file.setButtonColours(button_colour, button_hover);
    btn_choose_file.update(mouse_world, dt);
    file_name.setText(file_path_name);

    // Disable Colour
    colour_label.setFillColor(disabled_colour);
    rainbow_selection->setColour(disabled_colour);
    colour_picker->setDisabled(true);

  }

  if (use_solid_colour)
  {
    // Using Solid Colour
    colour_label.setFillColor(sf::Color::White);
    rainbow_selection->update(mouse_world);
    use_rainbow = rainbow_selection->isChecked();

    if (!use_rainbow)
    {
      colour_picker->update(mouse_world);
      picked_colour = colour_picker->getColourRGB();
      rainbow_selection->setColour(disabled_colour);
      colour_picker->setDisabled(false);
    }
    else
    {
      rainbow_selection->setColour(sf::Color::White);
      colour_picker->setDisabled(true);
    }


    // Disable Image
    file_label.setFillColor(disabled_colour);
    file_name.setFillColor(disabled_colour);
    btn_choose_file.setButtonColours(disabled_colour, disabled_colour);
  }





}

void SettingsWindow::render()
{
  // Header
  window.draw(icon_sprite);
  window.draw(title_text);
  window.draw(subtitle_text);
  window.draw(hoz_sep_main_header);

  btn_start_simulation.draw(window);

  // Source Header
  window.draw(hdn_source);
  window.draw(*rb_image);
  window.draw(*rb_solid_colour);
  window.draw(ver_sep_source);

  window.draw(file_label);
  window.draw(file_name);
  btn_choose_file.draw(window);

  window.draw(colour_label);
  window.draw(*colour_picker);
  window.draw(*rainbow_selection);

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
    full_file_path = file;

    std::filesystem::path filepath(file);
    file_path_name = filepath.filename().string();;
  }
  else
    std::cout << "No File" << std::endl;
}

bool SettingsWindow::pickColour(sf::Color& out_colour, std::string& hex_colour)
{
  unsigned char colour[3] = { 255, 255, 255 };
  const char* result = tinyfd_colorChooser(
      "Choose a Colour",
      nullptr,
      colour,
      colour
  );

  hex_colour = result;

  if (result) {
    out_colour.r = colour[0];
    out_colour.g = colour[1];
    out_colour.b = colour[2];
    out_colour.a = 255;
    return true;
  }
  return false;
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

  // Start Sim Button
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

  // Choose file
  if (btn_choose_file.getGlobalBounds().contains(mouse_world) && use_image)
  {
    openImageFileDialog();
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
    sf::Color test;
    std::string test_str;
    bool test_bool = pickColour(test, test_str);
    std::cout << test_str << " Color: " << unsigned(test.r) << ", " << unsigned(test.g) << ", " << unsigned(test.b) << std::endl;
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


