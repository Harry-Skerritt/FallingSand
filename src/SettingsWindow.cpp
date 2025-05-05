
#include "SettingsWindow.h"
#include <iostream>
#include <math.h>
#include <filesystem>

#include "SFML/Audio/SoundRecorder.hpp"

SettingsWindow::SettingsWindow(sf::RenderWindow& game_window, SharedData* sharedData)
  : window(game_window), shared_data(sharedData)
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
    updated_window_size = false;
  };
  rb_solid_colour->onSelect = [this]()
  {
    std::cout << "Colour selected\n";
    use_solid_colour = true;
    use_image = false;
    updated_window_size = false;
  };

  ver_sep_source.init({window_size.x *0.35f, window_size.y * 0.22f}, 72, 2, sf::Color::White);

  // File
  file_label.init({window_size.x * 0.378f, window_size.y * 0.225f}, "File:", &font, (window_size.x * 0.03f));

  file_name.init({window_size.x * 0.476f, window_size.y * 0.225f}, file_path_name, &font, (window_size.x * 0.03f), sf::Color(0, 58, 15, 255));
  file_name.setTruncateType(TruncateType::Ellipsis);
  file_name.setMaxWidth(130);

  shared_data->path_to_image = full_file_path;

  btn_choose_file.init({window_size.x * 0.758f, window_size.y * 0.225f},
     {window_size.x*0.212f, window_size.y*0.031f}, button_colour, "Choose", &font);
  btn_choose_file.setButtonColours(button_colour, button_hover);
  btn_choose_file.setTextColor(sf::Color::White);

  // Colour
  colour_label.init({window_size.x * 0.378f, window_size.y * 0.287f}, "Colour:", &font, (window_size.x * 0.03f));

  colour_picker = new ColourPicker({window_size.x * 0.534f, window_size.y * 0.287f}, {20, 20}, picked_colour, &font);
  picked_colour_hex = colour_picker->getColourHex();

  rainbow_selection = new CheckBox({window_size.x * 0.758f, window_size.y * 0.287f}, {20, 20}, "Rainbow", &font);

  // Window Size
  window_size_label.init({window_size.x * 0.106f, window_size.y * 0.341f}, "Window Size:", &font, (window_size.x * 0.03f));

  width_text_entry = new TextEntryField(100, 20, sf::Color(217, 217, 217, 255), sf::Color::Black);
  width_text_entry->setFont(font);
  width_text_entry->setPosition(window_size.x * 0.39f, window_size.y * 0.338f);
  width_text_entry->setTextSize(window_size.y * 0.0238f);

  window_x_text.init({window_size.x * 0.62f, window_size.y * 0.341f}, "x", &font, (window_size.x * 0.03f));


  height_text_entry = new TextEntryField(100, 20, sf::Color(217, 217, 217, 255), sf::Color::Black);
  height_text_entry->setFont(font);
  height_text_entry->setPosition(window_size.x * 0.664f, window_size.y * 0.338f);
  height_text_entry->setTextSize(window_size.y * 0.0238f);

  width_text_entry->onTextTyped([this](const std::string& text)
  {
    if (locked_aspect_ratio)
    {
        if (!text.empty())
        {
            int newWidth = std::stoi(text);
            int newHeight = calculateAspectHeight(
                std::round(sim_window_size.x),
                std::round(sim_window_size.y),
                newWidth
            );
            height_text_entry->setString(std::to_string(newHeight));
        }
    }
  });

  return true;
}

int SettingsWindow::calculateAspectHeight(
  int originalWidth, int originalHeight, int newWidth)
{
  if (originalWidth == 0) return 0; // Prevent division by zero
  float aspectRatio = static_cast<float>(originalHeight) / static_cast<float>(originalWidth);
  return static_cast<int>(newWidth * aspectRatio);
}

void SettingsWindow::update(float dt)
{
  sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
  sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_pixel);

  // Header
  btn_start_simulation.update(mouse_world, dt);

  if (use_solid_colour || use_rainbow)
  {
    sim_window_size = default_sim_window_size;
    if (!updated_window_size)
    {
      width_text_entry->setString(std::to_string(std::round(sim_window_size.x)));
      height_text_entry->setString(std::to_string(std::round(sim_window_size.y)));
      updated_window_size = true;
    }

    locked_aspect_ratio = false;
  }
  else if (use_image)
  {
    sf::Image image;
    if (!image.loadFromFile(full_file_path))
    {
      std::cerr << "Failed to load image: " << full_file_path << std::endl;
    }

    sim_window_size = sf::Vector2f(image.getSize().x, image.getSize().y);
    if (!updated_window_size)
    {
      width_text_entry->setString(std::to_string(std::round(sim_window_size.x)));
      height_text_entry->setString(std::to_string(std::round(sim_window_size.y)));
      updated_window_size = true;
    }
    locked_aspect_ratio = true;
  }

  if (!show_simulation)
  {
    // Source Header
    rb_image->update(mouse_world);
    rb_solid_colour->update(mouse_world);
    shared_data->use_image = use_image;
    shared_data->use_colour = use_solid_colour;
    shared_data->use_rainbow = use_rainbow;

    width_text_entry->setEnabled(true);
    height_text_entry->setEnabled(true);
    window_size_label.setFillColor(sf::Color::White);
    window_x_text.setFillColor(sf::Color::White);

    if (locked_aspect_ratio && width_text_entry->isEnabled())
    {
      height_text_entry->setEnabled(false);
    }

    rb_image->setCircleColor(sf::Color::White, sf::Color::White);
    rb_solid_colour->setCircleColor(sf::Color::White, sf::Color::White);

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
      shared_data->use_rainbow = use_rainbow;

      if (!use_rainbow)
      {
        colour_picker->update(mouse_world);
        picked_colour = colour_picker->getColourRGB();
        rainbow_selection->setColour(disabled_colour);
        shared_data->selected_colour = picked_colour;
        shared_data->selected_background_color = sf::Color(picked_colour.r * 0.4f, picked_colour.g * 0.4f,picked_colour.b * 0.4f, picked_colour.a);
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
  else
  {
    // Disable everything
    // Disable Image
    file_label.setFillColor(disabled_colour);
    file_name.setFillColor(disabled_colour);
    btn_choose_file.setButtonColours(disabled_colour, disabled_colour);

    // Disable Colour
    colour_label.setFillColor(disabled_colour);
    rainbow_selection->setColour(disabled_colour);
    colour_picker->setDisabled(true);

    // Radio
    rb_image->setCircleColor(disabled_colour, disabled_colour);
    rb_solid_colour->setCircleColor(disabled_colour, disabled_colour);

    // Window Size
    width_text_entry->setEnabled(false);
    height_text_entry->setEnabled(false);
    window_size_label.setFillColor(disabled_colour);
    window_x_text.setFillColor(disabled_colour);
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

  window.draw(window_size_label);
  window.draw(*width_text_entry);
  window.draw(window_x_text);
  window.draw(*height_text_entry);

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

    shared_data->path_to_image = full_file_path;

    updated_window_size = false;

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

void SettingsWindow::handleGenericEvent(sf::Event event)
{
  width_text_entry->handleEvent(event, window);
  height_text_entry->handleEvent(event, window);
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
    shared_data->use_image = use_image;
    shared_data->use_colour = use_rainbow ? false : use_solid_colour;
    shared_data->use_rainbow = use_rainbow;
    sim_window_size = sf::Vector2f(std::stof(width_text_entry->getString()), std::stof(height_text_entry->getString()));

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


