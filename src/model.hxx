#pragma once

#include <ge211.hxx>
#include <card.hxx>
#include "card_container.hxx"
#include <list>

class Model
{
public:
    Model();

    using Position = ge211::Posn<int>;

    const int width = 7; // grid width (rows)
    const int height = 6; // grid height (cols)

    // plays the move when user clicks mouse
    void play_move_click(Position);

    // plays the move when user clicks and drags
    void play_move_drag(Position, Position);

    // checks if game is complete
    void set_game_over();

    void shuffle_deck();

    void set_card_at_board(Card, Position, Position);

    Card card_deck_[20];

    int get_turn() const;

    bool is_valid_move(Position, Position);

    Card_Container board_[6][7];

    //Position board_to_screen(int, int)const;
    //Position screen_to_board(int, int)const;
    Position
    mod_scr_to_board(int, int) const;

    void set_current_dragged(Position);

    bool is_winner() const;

    bool check_bottom(Position);

    bool is_dragging;
    Card current_dragged;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif
private:
    int turns_;
    bool winner_;
};
