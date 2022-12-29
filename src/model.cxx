#include "model.hxx"
#include "card.hxx"
#include "card_container.hxx"
#include <cstdlib>
#include <time.h>

Model::Model()
        : card_deck_{Card('h', 1, "1_h.png"),
                     Card('h', 2,"2_h.png"),
                     Card('h', 3,"3_h.png"),
                     Card('h', 4,"4_h.png"),
                     Card('h', 5,"5_h.png"),
                     Card('d', 1,"1_d.png"),
                     Card('d', 2,"2_d.png"),
                     Card('d', 3,"3_d.png"),
                     Card('d', 4,"4_d.png"),
                     Card('d', 5, "5_d.png"),
                     Card('s', 1, "1_s.png"),
                     Card('s', 2, "2_s.png"),
                     Card('s', 3, "3_s.png"),
                     Card('s', 4, "4_s.png"),
                     Card('s', 5, "5_s.png"),
                     Card('c', 1, "1_c.png"),
                     Card('c', 2, "2_c.png"),
                     Card('c', 3, "3_c.png"),
                     Card('c', 4, "4_c.png"),
                     Card('c', 5, "5_c.png")},
          board_{{Card_Container(),
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
          is_dragging(false),
          current_dragged(Card(' ',0,"")),
          turns_(0),
          winner_(false)
{
    shuffle_deck();
    // Stack cards to top right
    for(int i = 0; i < 14; ++i){
        board_[0][6].stack_.push(card_deck_[i]);
    }
    // Stack cards in first solitaire column
    board_[1][1].stack_.push(card_deck_[14]);

    // Stack cards in second solitaire column
    board_[1][3].stack_.push(card_deck_[15]);
    board_[1][3].stack_.push(card_deck_[16]);

    // Stack cards in third solitaire column
    board_[1][5].stack_.push(card_deck_[17]);
    board_[1][5].stack_.push(card_deck_[18]);
    board_[1][5].stack_.push(card_deck_[19]);
}


void
Model::play_move_click(Model::Position board_pos)

{
    Position top_right = {6,0};
    Position next_to_top_right = {5,0};

    if (board_pos == top_right) {

        if (!board_[0][6].stack_.empty()) {
            Card last_card = board_[0][6].stack_.top();
            board_[0][5].stack_.push(last_card); // pushes the top
            // card of (0,6) onto (0,5) while removing from stack at (0,6)
            board_[0][6].stack_.pop();
            turns_ += 1;
        }

    } else if (board_pos == next_to_top_right) {

        // !!! THIS CODE SHOULD ONLY RUN IF OUR DISCARD PILE AT (0,6) IS EMPTY,
        // OTHERWISE NOTHING SHOULD HAPPEN WHEN WE CLICK AND NO

        // DRAG ON THIS BOARD POSITION !!!

        if (board_[0][6].stack_.empty()) {
            int init_size = board_[0][5].stack_.size();
            for (int i = 0; i < init_size; ++i) {
                Card top_card = board_[0][5].stack_.top();
                // pushes the
                // top card of (0,5) onto (0,6) however many times the stack
                // is large
                board_[0][6].stack_.push(top_card);
                // e.g 6 times if size is 6 (this is the equivalent of resetting
                // the discard pile)
                board_[0][5].stack_.pop();
            }
            turns_ += 1;
        }
    }
    current_dragged = Card(' ', 0, "");
}


void
Model::play_move_drag(Model::Position start_board_pos, Model::Position
end_board_pos) {
    Position end_card_pos = mod_scr_to_board(end_board_pos.x,
                                             end_board_pos.y);
    Position start_card_pos = mod_scr_to_board(start_board_pos.x,
                                               start_board_pos.y);
    end_card_pos = {end_card_pos.y, end_card_pos.x};
    start_card_pos = {start_card_pos.y, start_card_pos.x};
    if (board_[start_card_pos.x][start_card_pos.y].stack_.empty()) {
        return;
    }
    board_[start_card_pos.x][start_card_pos.y].stack_.pop();

    if (end_card_pos.x == 0 && is_valid_move(start_card_pos, end_card_pos)) {
        board_[end_card_pos.x][end_card_pos.y].stack_.push(current_dragged);
        turns_ += 1;
    } else if (current_dragged.get_value() == 5 && end_card_pos.x == 1 &&
               is_valid_move(start_card_pos, end_card_pos) &&
               start_card_pos.x == 0) {
        board_[end_card_pos.x][end_card_pos.y].stack_.push(current_dragged);
        turns_ += 1;
    } else if (is_valid_move(start_card_pos, end_card_pos)) {
        int end = 6 - start_card_pos.x;
        if (start_card_pos.x == 0) {
            board_[end_card_pos.x+1][end_card_pos.y].stack_.push
                    (current_dragged);
            turns_ += 1;
        }else if (start_card_pos.x != 0 && current_dragged.get_value() != 5) {
            board_[end_card_pos.x + 1][end_card_pos.y].stack_
                                                      .push((current_dragged));
            for (int i = 1; i < end; ++i) {
                if (!board_[start_card_pos.x + i][start_card_pos.y].stack_
                                                                   .empty()) {
                    board_[end_card_pos.x + i + 1][end_card_pos.y].stack_.push
                            (board_[start_card_pos.x +
                                    i][start_card_pos.y].stack_.top());
                    board_[start_card_pos.x + i][start_card_pos.y].stack_.pop();
                }
            }
            turns_ += 1;
        } else if (start_card_pos.x != 0 && current_dragged.get_value() == 5) {
            board_[end_card_pos.x][end_card_pos.y].stack_
                                                  .push(current_dragged);
            for (int i = 1; i < end; ++i) {
                if (!board_[start_card_pos.x + i][start_card_pos.y].stack_
                                                                   .empty()) {
                    board_[end_card_pos.x + i][end_card_pos.y].stack_.push
                            (board_[start_card_pos.x +
                                    i][start_card_pos.y].stack_.top());
                    board_[start_card_pos.x + i][start_card_pos.y].stack_.pop();
                }
            }
            turns_ += 1;
        }
    } else {
        board_[start_card_pos.x][start_card_pos.y].stack_.push
                (current_dragged);
    }
    current_dragged = Card(' ', 0, "");
    set_game_over();

}

// remove card from top of stack when dragging, then add it to the
// next stack if valid, if not valid then add back to the previous
// stack

bool
Model::is_valid_move(Model::Position start_pos, Model::Position
end_pos)
{
    Position end0_6 = {0,6};
    Position end0_5 = {0,5};
    Position end_1 = {0,0};
    Position end_2 = {0,1};
    Position end_3 = {0,2};
    Position end_4 = {0,3};
    Position start_pos_1 = {1,1};
    Position start_pos_2 = {1,3};
    Position start_pos_3 = {1,5};
    // Valid moves for an ace
    if (winner_) {
        return false;
    }
    if (current_dragged.get_value() == 1) {
        return ((end_pos == end_1 || end_pos == end_2 ||
                 end_pos
                 == end_3 || end_pos == end_4) && board_[end_pos
                .x][end_pos.y].stack_.empty());
    } else if (current_dragged.get_value() == 5) {
        if ((end_pos == start_pos_1 || end_pos == start_pos_2 ||
             end_pos == start_pos_3) && board_[end_pos
                .x][end_pos.y].stack_.empty()) {
            return true;
        } else if (!board_[end_pos.x][end_pos.y].stack_.empty()){
            if ((end_pos == end_1 || end_pos == end_2 ||
                 end_pos == end_3 || end_pos == end_4) &&
                board_[end_pos.x][end_pos.y].stack_.top().get_suit() ==
                current_dragged.get_suit() && (board_[end_pos.x][end_pos
                    .y].stack_.top().get_value()+1) == current_dragged
                                                      .get_value() && check_bottom(start_pos)) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        // CHANGE THIS
        if (!board_[end_pos.x][end_pos.y].stack_.empty()) {
            if (end_pos == end0_6 || end_pos == end0_5) {
                return false;
            } else if ((end_pos == end_1 || end_pos == end_2 || end_pos
                                                                == end_3 || end_pos == end_4) && check_bottom(start_pos) &&
                       current_dragged.get_suit() == board_[end_pos.x][end_pos.y].stack_
                                                                                 .top().get_suit() && board_[end_pos.x][end_pos.y].stack_.top()
                                                                                                                                  .get_value() + 1 == current_dragged.get_value()) {
                return true;
            } else if ((board_[end_pos.x][end_pos.y].stack_.top().is_black() &&
                        current_dragged.is_red()) && current_dragged.get_value()
                                                     + 1 == board_[end_pos.x][end_pos.y].stack_.top()
                                                                                        .get_value()) {
                return true;
            } else if ((board_[end_pos.x][end_pos.y].stack_.top().is_red() &&
                        current_dragged.is_black()) &&
                       current_dragged.get_value() + 1 == board_[end_pos
                               .x][end_pos.y]
                               .stack_.top().get_value()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

void
Model::shuffle_deck()
{
    int random_num;
    for(int x = 0; x < 100; ++x) {
        srand(time(0));
        for (int i = 0; i < 20; ++i) {
            Card curr_card = card_deck_[i];
            random_num = rand() % 20;
            card_deck_[i] = card_deck_[random_num];
            card_deck_[random_num] = curr_card;
        }
    }
}

int
Model::get_turn() const {
    return turns_;
}

Model::Position
Model::mod_scr_to_board(int x, int y) const {
    return {(x-5)/60, (y-14)/100};
}

void
Model::set_current_dragged(Position pos) {
    Position card_pos = mod_scr_to_board(pos.x,pos.y);
    if (!board_[card_pos.y][card_pos.x].stack_.empty()) {
        current_dragged = board_[card_pos.y][card_pos.x].stack_.top();
    }
}

bool
Model::check_bottom(Position pos) {
    return (board_[pos.x+1][pos.y].stack_.empty() && pos.x != 0) || pos.x == 0;
}

void
Model::set_game_over()
{


    if (board_[1][1].stack_.empty() && board_[1][3].stack_.empty() &&
        board_[1][5].stack_.empty() && board_[0][5].stack_.empty() &&
        board_[0][6].stack_.empty() && board_[0][0].stack_.size() == 5 &&
        board_[0][1].stack_.size() == 5 && board_[0][2].stack_.size() == 5 &&
        board_[0][3].stack_.size() == 5) {
        winner_ = true;
    }

}

bool
Model::is_winner() const
{
    return winner_;
}

void
Model::set_card_at_board(Card c, Model::Position start, Model::Position end)
{
    end = mod_scr_to_board(end.x, end.y);
    end = {end.y, end.x};
    start = mod_scr_to_board(start.x, start.y);
    start = {start.y, start.x};
    if (is_valid_move(start, end)) {
        board_[end.x][end.y].stack_.push(c);
        board_[start.x][start.y].stack_.pop();
    }
}
