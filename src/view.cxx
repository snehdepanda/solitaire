#include "view.hxx"

View::View(Model const& model)
        : model_(model),
        // model_size minus two makes space between grid
          grid_sprite_({model_size_width-2,model_size_height-2},
                       ge211::Color::medium_green()),
          card_sprites_{{Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()},
                        {Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()},
                        {Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()},
                        {Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()},
                        {Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()},
                        {Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container(),
                                Card_Container()}},
          card_dragged_(Card_Container())
{
}

View::Dimensions
View::initial_window_dimensions() const {
    return {model_size_width*model_.width,
            model_size_height*model_.height};
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Solitaire Lite";
}

void
View::draw(ge211::Sprite_set& set, Position mouse_pos)
{
    // card dim: 50 x 72 px
    for (int i = 0; i < model_.width; i++) {
        for (int j = 0; j < model_.height; j++) {
            set.add_sprite(grid_sprite_, {i * model_size_width,
                                          j * model_size_height}, 0);
        }
    }

    for(int i = 0; i < model_.height; ++i) {
        for (int j = 0; j < model_.width; ++j) {
            if (!model_.board_[i][j].stack_.empty()) {
                Card stack_top = model_.board_[i][j].stack_.top();
                std::string img_filename = stack_top.filename;
                ge211::Image_sprite img(img_filename);
                card_sprites_[i][j].image_stack_.push(img);
                set.add_sprite(card_sprites_[i][j].image_stack_.top(),
                               board_to_screen(j, i),
                               1);
            }
        }
    }
    if (model_.is_dragging && model_.current_dragged.get_filename() != "") {
        std::string img_filename = model_.current_dragged.get_filename();
        ge211::Image_sprite img(img_filename);
        card_dragged_.image_stack_.push(img);
        set.add_sprite(card_dragged_.image_stack_.top(),mouse_pos,
                       2);
    }

    ge211::Text_sprite::Builder turns_builder(sans12_);
    turns_builder << "Turns: " + std::to_string(model_.get_turn());
    turns_sprite_.reconfigure(turns_builder);
    set.add_sprite(turns_sprite_, board_to_screen(6,5),2);

}

// 5px is offset to center in x-direction
// 14 px is offset to center in y-direction

View::Position
View::board_to_screen(int x, int y) const {
    return {x*60+5,y*100+14};
}

View::Position
View::screen_to_board(int x, int y) const
{
    return {(x-5)/60,(y-14)/100};
}



