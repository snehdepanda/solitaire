#pragma once

#include "model.hxx"
#include <stack>
static const int model_size_width = 60; // cell size for grids on board (rows)
static const int model_size_height = 100; // cell size for grids on board (cols)

class View
{
public:
    using Dimensions = ge211::Dims<int>;

    using Position = ge211::Posn<int>;

    explicit View(Model const& model);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    void draw(ge211::Sprite_set&, Position);

    Position
    board_to_screen(int, int) const;

    Position
    screen_to_board(int, int) const;

private:
    Model const& model_;
    // sprite to populate grids on board
    ge211::Rectangle_sprite grid_sprite_;
    Card_Container card_sprites_[6][7];
    Card_Container card_dragged_;
    ge211::Font sans12_{"sans.ttf", 12};
    ge211::Text_sprite turns_sprite_;

};
