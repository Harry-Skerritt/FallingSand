//
// Created by Harry on 02/05/2025.
//

#ifndef SANDSIMULATION_H
#define SANDSIMULATION_H

#include <SFML/Graphics.hpp>


class SandSimulation {

public:
    SandSimulation(sf::RenderWindow& game_window);
    ~SandSimulation();


    bool init();
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // Sand
    void spawnNewSand(sf::Vector2f prev_mouse_pos);
    //sf::RectangleShape drawCell(sf::Vector2i pos, sf::Color colour);

    int getSandParticleCount() const;
    float getFilledPercentage() const;

    void resetGrid();

    // Images
    bool loadAlbum(std::string loc);
    void cachePixels();
    sf::Color getAverageColour(const sf::Image& image);
    sf::Color getAverageBackgroundColour(sf::Color colour);

    // FPS
    void calcFPS();

    // Setters / Setters
    void setSandSize(float size) { sand_size = size; };
    float getSandSize() const { return sand_size;}

    void setBrushSize(float size) { spawn_size = size; }
    float getBrushSize() const { return spawn_size; }

    void setSandColour(sf::Color colour);
    sf::Color getSandColour() const { return sand_colour; }

    void setBackgroundColour(sf::Color colour);
    sf::Color getBackgroundColour() const { return background_colour; }




private:
    sf::RenderWindow& window;
    sf::Vector2f window_size;

    // Sand
    sf::VertexArray sand_verts;
    std::vector<std::vector<int>> sand_grid;
    //std::vector<std::vector<sf::Color>> grid_colours;
    //std::vector<std::vector<sf::Color>> image_colours;


    // Sand
    float sand_size = 2; // "resolution"
    int cell_amt_x, cell_amt_y;
    int spawn_size = 1; // "5x5 grid"
    int multiple_spawn_change = 40; // 40%
    bool spawn_multiple = true;

    // Solid Colours
    sf::Color sand_colour;
    bool use_solid_colour = true;
    bool use_rainbow = false;

    // Picture
    sf::Image album;
    sf::Texture colour_data_texture;
    sf::Image colour_data_image;

    // Background
    sf::RectangleShape background;
    sf::Color background_colour;



    // Helper Funcs
    bool within_cols(int x) const;
    bool within_rows(int y) const;




};



#endif //SANDSIMULATION_H
