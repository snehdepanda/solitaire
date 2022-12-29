#include "model.hxx"
#include <catch.hxx>

using namespace ge211;
using Position = ge211::Posn<int>;

struct Test_access
{
    Model& model;

    // Constructs a `Test_access` with a reference to the Model under test.
    explicit Test_access(Model&);
};

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate your functional requirements.
// //

Position zero0 = {10,40};
Position zero1 = {70,40};
Position zero2 = {130,40};
Position zero3 = {190,40};
Position zero4 = {250,40};
Position zero5 = {310,40};
Position zero6 = {370, 40};
Position one0 = {10,140};
Position one1 = {70,140};
Position one2 = {130,140};
Position one3 = {190,140};
Position one4 = {250,140};
Position one5 = {310,140};
Position one6 = {370,140};
Position two0 = {10,240};
Position two1 = {70,240};
Position two2 = {130,240};
Position two3 = {190,240};
Position two4 = {250,240};
Position two5 = {310,240};
Position two6 = {370,240};
Position three0 = {10,340};
Position three1 = {70,340};
Position three2 = {130,340};
Position three3 = {190,340};
Position three4 = {250,340};
Position three5 = {310,340};
Position three6 = {370,340};
Position four0 = {10,440};
Position four1 = {70,440};
Position four2 = {130,440};
Position four3 = {190,440};
Position four4 = {250,440};
Position four5 = {310,440};
Position four6 = {370,440};
Position five0 = {10,540};
Position five1 = {70,540};
Position five2 = {130,540};
Position five3 = {190,540};
Position five4 = {250,540};
Position five5 = {310,540};
Position five6 = {370,540};
// Screen positions for board
Position screen_pos[6][7] = {{zero0,
                             zero1,
                             zero2,
                             zero3,
                             zero4,
                             zero5,
                             zero6},
                             {one0,
                              one1,
                             one2,
                             one3,
                             one4,
                             one5,
                             one6},
                             {two0,
                             two1,
                             two2,
                             two3,
                             two4,
                             two5,
                             two6},
                             {three0,
                             three1,
                             three2,
                             three3,
                             three4,
                             three5,
                             three6},
                             {four0,
                             four1,
                             four2,
                             four3,
                             four4,
                             four5,
                             four6},
                             {five0,
                             five1,
                             five2,
                             five3,
                             five4,
                             five5,
                             five6}};

TEST_CASE("Click on remaining deck")
{
    Model model;
    Test_access access(model);
    // remaining decks are in position board_[0][6] and the screen position
    // given below corresponds within the area of the board
    Position board_pos = model.mod_scr_to_board(zero6.x, zero6.y);
    // users clicks on some position in grid: board_[0][6]
    model.play_move_click(board_pos);
    // check that the card is moved over to board_[0][5] as desired and that
    // there is one less card in board_[0][6] which is the remaining deck
    CHECK(model.board_[0][5].stack_.size() == 1);
    CHECK(model.board_[0][6].stack_.size() == 13);
    CHECK(model.is_winner() == false);

}

TEST_CASE("Invalid Move - Click on every empty grid") {
    Model model;
    Test_access access(model);
    // Click on every empty board position
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (model.board_[i][j].stack_.empty()) {
                model.play_move_click(screen_pos[i][j]);
            }
        }
    }
    // Check that there is no card at that position still and other positions
    // are not affected
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);
    CHECK(model.is_winner() == false);
}

TEST_CASE("Invalid Move - Drag a card to every empty grid") {
    Model model;
    Test_access access(model);
    model.board_[0][6].stack_.top().set_card(Card('h',2,"2_h.png"));
    // Drags top card in remaining deck to every empty grid position
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (model.board_[i][j].stack_.empty()) {
                model.play_move_drag(screen_pos[0][6],screen_pos[i][j]);
            }
        }
    }
    // Check that there is no card at that position still and other positions
    // are not affected
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);
    CHECK(model.is_winner() == false);
}

TEST_CASE("Check starting state of board") {
        Model model;
        Test_access access(model);
        // Board must have 14 cards on the top right grid, which are the
        // remaining cards the user can flip through
        // The first starting position which is at [1][1] on the grid must
        // have 1 card, the second starting position at [1][3] must have 1
        // card showing and 1 card hidden underneath, the third starting
        // position at [1][5] must have 1 card showing and 2 cards underneath.
        // These positions are based off the rules of solitaire.
        CHECK(model.board_[0][0].stack_.size() == 0);
        CHECK(model.board_[0][5].stack_.size() == 0);
        CHECK(model.board_[0][6].stack_.size() == 14);
        CHECK(model.board_[1][1].stack_.size() == 1);
        CHECK(model.board_[1][3].stack_.size() == 2);
        CHECK(model.board_[1][5].stack_.size() == 3);
    CHECK(model.is_winner() == false);
}

TEST_CASE("Check invalid move - moving non-ace to empty final position") {
    // Checks that a non-ace card cannot be moved to the final positions
    // [0][0],[0][1],[0][2], or [0][3] before an Ace is moved there. Also,
    // another condition is, is that there must be a card in the final
    // position that has value 1 less than the value of the card being moved,
    // and the same suit.
    Model model;
    Test_access access(model);
    // Check with 4 of hearts
    // set card on top of remaining deck to be 4 of hearts
    model.board_[0][6].stack_.top().set_card(Card('h', 4, "4_h.png"));
    // check that the card is in fact set to 4 of hearts
    CHECK(model.board_[0][6].stack_.top().get_value() == 4);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'h');
    // set card on top of remaining deck (4 of hearts) to move to [0][0]
    // position on the board <<-- invalid move
    model.set_card_at_board(model.board_[0][6].stack_.top(), screen_pos[0][6],
                            screen_pos[0][0]);
    // There should be no card at [0][0] on the board since the move was
    // invalid, and the card should be back at its original position on top
    // of the remaining deck at [0][6]
    CHECK(model.board_[0][0].stack_.empty());
    CHECK(model.board_[0][6].stack_.top().get_value() == 4);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'h');

    // Check with 3 of diamonds
    model.board_[0][6].stack_.top().set_card(Card('d', 3, "3_d.png"));
    CHECK(model.board_[0][6].stack_.top().get_value() == 3);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'd');

    model.set_card_at_board(model.board_[0][6].stack_.top(), screen_pos[0][6],
                            screen_pos[0][0]);
    CHECK(model.board_[0][0].stack_.empty());
    CHECK(model.board_[0][6].stack_.top().get_value() == 3);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'd');

    // Check with 2 of spades
    model.board_[0][6].stack_.top().set_card(Card('s', 2, "2_s.png"));
    CHECK(model.board_[0][6].stack_.top().get_value() == 2);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 's');

    model.set_card_at_board(model.board_[0][6].stack_.top(), screen_pos[0][6],
                            screen_pos[0][0]);
    CHECK(model.board_[0][0].stack_.empty());
    CHECK(model.board_[0][6].stack_.top().get_value() == 2);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 's');

    // Check with 5 of clubs
    model.board_[0][6].stack_.top().set_card(Card('c', 5, "5_c.png"));
    CHECK(model.board_[0][6].stack_.top().get_value() == 5);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'c');

    model.set_card_at_board(model.board_[0][6].stack_.top(), screen_pos[0][6],
                            screen_pos[0][0]);
    CHECK(model.board_[0][0].stack_.empty());
    CHECK(model.board_[0][6].stack_.top().get_value() == 5);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'c');
    CHECK(model.is_winner() == false);
}

TEST_CASE("Valid move - red 3 card on black 4 card") {
    Model model;
    Test_access access(model);
    // check starting position is correct
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    model.board_[1][1].stack_.top().set_card(Card('h', 3, "3_h.png"));
    model.board_[1][3].stack_.top().set_card(Card('s', 4, "4_s.png"));

    CHECK(model.board_[1][1].stack_.top().get_value() == 3);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 'h');
    CHECK(model.board_[1][3].stack_.top().get_value() == 4);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');

    model.set_current_dragged(screen_pos[1][1]);
    model.play_move_drag(screen_pos[1][1],screen_pos[1][3]);

    // board[1][3] should still be 4 of spades
    CHECK(model.board_[1][3].stack_.top().get_value() == 4);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    // 3 of hearts should now have moved below the 4 of clubs
    CHECK(model.board_[2][3].stack_.top().get_value() == 3);
    CHECK(model.board_[2][3].stack_.top().get_suit() == 'h');
    // where the 3 of hearts moved from should now be empty (there were no
    // cards beneath it due to its starting position)
    CHECK(model.board_[1][1].stack_.empty());
    CHECK(model.is_winner() == false);
}

TEST_CASE("Drag ace into final position") {
    Model model;
    Test_access access(model);

    // check starting position is correct
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    // Set card on position [1][1] to Ace of Spades
    model.board_[1][1].stack_.top().set_card(Card('s', 1, "1_s.png"));
    // Check that the card is set as desired
    CHECK(model.board_[1][1].stack_.top().get_value() == 1);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 's');
    CHECK(!model.board_[1][1].stack_.empty());

    // Set the dragged card to the Ace of Spades set at position [1][1]
    model.set_current_dragged(screen_pos[1][1]);
    // Drag the card to one of the final positions which are [0][0], [0][1],
    // [0][2], and [0][3]
    model.play_move_drag(screen_pos[1][1],screen_pos[0][1]);
    // Check if the ace is actually at the dragged position
    CHECK(model.board_[0][1].stack_.top().get_value() == 1);
    CHECK(model.board_[0][1].stack_.top().get_suit() == 's');
    CHECK(model.board_[0][1].stack_.size() == 1);
    // Where the ace was moved from, its starting position, must now have no
    // cards; thus, should be empty
    CHECK(model.board_[1][1].stack_.empty());
    // Played a turn, so the turn counter should increment by 1
    CHECK(model.get_turn() == 1);
    // Since the winning criteria has not been met there should be no winner
    CHECK(model.is_winner() == false);

    // Try to move another ace card onto the same location --> will not work
    // Move it to another open final position --> will work

    // Set top card in remaining pile to Ace of Hearts
    model.board_[0][6].stack_.top().set_card(Card('h',1,"1_h.png"));
    // Check if card is set to Ace of Hearts
    CHECK(model.board_[0][6].stack_.top().get_value() == 1);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'h');
    // Drag the card onto [0][1] which is not an open final position since
    // the Ace of Spades is already there
    model.set_current_dragged(screen_pos[0][6]);
    // Drags Ace of Hearts to [0][1]
    model.play_move_drag(screen_pos[0][6],screen_pos[0][1]);
    // Check that Ace of Hearts is not at [0][1] and it is back at [0][6]
    // since that was an invalid move
    CHECK(model.board_[0][6].stack_.top().get_value() == 1);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'h');
    CHECK(model.board_[0][1].stack_.top().get_value() == 1);
    CHECK(model.board_[0][1].stack_.top().get_suit() == 's');
    // Drag the Ace of Hearts from the remaining pile [0][6] onto an OPEN
    // final position of which there are [0][0], [0][2], [0][3] remaining.
    unsigned int remaining_pile_size_before_move = model.board_[0][6].stack_
            .size();
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][0]);
    // Check that the Ace of Hearts is at [0][0] and no longer on the
    // remaining pile of cards since this was a valid move. The remaining
    // pile must also have one less card than before.
    CHECK(model.board_[0][0].stack_.top().get_value() == 1);
    CHECK(model.board_[0][0].stack_.top().get_suit() == 'h');
    CHECK(model.board_[0][6].stack_.size() != remaining_pile_size_before_move);
    // There should be no winner yet
    CHECK(model.is_winner() == false);
    // The number of turns played is 2
    CHECK(model.get_turn() == 2);
}

TEST_CASE("Adds card to grid below if valid move") {
    // Checks if when player plays a valid move (ex: 4 of Hearts to 5 of
    // Spades, then card with the lower value is placed on the grid directly
    // below the card with the higher value
    Model model;
    Test_access access(model);

    // check starting position is correct
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    // Set card on [1][3] to a 5 of Spaces to demonstrate the test
    model.board_[1][3].stack_.top().set_card(Card('s',5,"5_s.png"));
    // Set card on [1][5] to a 4 of Hearts to demonstrate the test
    model.board_[1][5].stack_.top().set_card(Card('h',4,"4_h.png"));
    // Check that the cards were set properly
    CHECK(model.board_[1][3].stack_.top().get_value() == 5);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    CHECK(model.board_[1][5].stack_.top().get_value() == 4);
    CHECK(model.board_[1][5].stack_.top().get_suit() == 'h');
    // There should be two cards on [1][3] (one under 5 of Spades) and three
    // cards on [1][5] (two under 4 of Hearts)
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    // Try moving the 4 of Hearts to the remaining pile on [0][6]
    model.set_current_dragged(screen_pos[1][5]);
    model.play_move_drag(screen_pos[1][5],screen_pos[0][6]);
    // Check that this did not work
    // 4 of Hearts is still top card on [1][5]
    CHECK(model.board_[1][5].stack_.top().get_value() == 4);
    CHECK(model.board_[1][5].stack_.top().get_suit() == 'h');
    // The size of the remaining pile is still 14 since no other cards from
    // there have been clicked on or played
    CHECK(model.board_[0][6].stack_.size() == 14);

    // Move the 4 of Hearts onto the 5 of Spades and check that it goes to
    // the grid under 5 of spades (the 4 of hearts must be dragged onto the 5 of
    // spades not on the grid under, by the user/mouse)
    model.set_current_dragged(screen_pos[1][5]);
    model.play_move_drag(screen_pos[1][5],screen_pos[1][3]);
    // Since this is a valid move, check grid below and check that the size
    // of the stack on [1][5] has reduced by 1 (from 3 to 2).
    CHECK(model.board_[1][3].stack_.top().get_value() == 5);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    CHECK(model.board_[2][3].stack_.top().get_value() == 4);
    CHECK(model.board_[2][3].stack_.top().get_suit() == 'h');
    CHECK(model.board_[1][5].stack_.size() == 2);
    // Since a turn has been played, it should be incremented
    CHECK(model.get_turn() == 1);
    // Since there is no winner, check for no winner
    CHECK(model.is_winner() == false);

}

TEST_CASE("Only allows bottom cards to be dragged to final position") {
    // If there are cards below a card (ex: 4 of Heart, then 3 of Spades,
    // then 2 of Diamonds), then the 4 of Hearts or the 3 of Spades should
    // not be able to be moved to a final position, only the 2 of diamonds
    // should. But when the 2 of diamonds moves to the final position, the 3
    // of spades can move to the final position, but the 4 of hearts cannot.
    // This is the criteria for a valid move.
    Model model;
    Test_access access(model);

    // check starting position is correct
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    // Set the board first
    model.board_[1][1].stack_.top().set_card(Card('h',4,"4_h.png"));
    // Card in playable position [1][3] is set to 3 of spades
    model.board_[1][3].stack_.top().set_card(Card('s',3,"3_s.png"));
    // Card in remaining pile top is set to 2 of diamonds
    model.board_[0][6].stack_.top().set_card(Card('d',2,"2_d.png"));
    // Checks that the cards have been set effectively
    CHECK(model.board_[1][1].stack_.top().get_value() == 4);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 'h');
    CHECK(model.board_[1][3].stack_.top().get_value() == 3);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    CHECK(model.board_[0][6].stack_.top().get_value() == 2);
    CHECK(model.board_[0][6].stack_.top().get_suit() == 'd');

    // Move 2 of Diamonds to 3 of Spades from [0][6] to [1][3], which is a
    // valid move
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[1][3]);
    // Check that 2 of diamonds is in the grid below 3 of spades and 4 of
    // hearts is still in the same position
    CHECK(model.board_[1][1].stack_.top().get_value() == 4);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 'h');
    CHECK(model.board_[1][3].stack_.top().get_value() == 3);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    CHECK(model.board_[2][3].stack_.top().get_value() == 2);
    CHECK(model.board_[2][3].stack_.top().get_suit() == 'd');

    // Set Ace of Spades and 2 of Spades into a final position stack, so that 3
    // of spades should be able to move into it. Set Ace of Diamonds in a final
    // position so 2 of diamonds can go into it. Set Ace of hearts, 2 of
    // hearts, 3 of hearts into a final position stack so that 4 of hearts
    // should be able to move into it. (These are all valid moves, just not
    // when they have another card below them).
    // Move Ace of Diamonds to [0][0]
    model.board_[0][6].stack_.top().set_card(Card('d',1,"1_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][0]);
    CHECK(model.board_[0][0].stack_.top().get_value() == 1);
    CHECK(model.board_[0][0].stack_.top().get_suit() == 'd');
    // Move Ace of Spades and 2 of Spades to [0][1] (2 of spades should be on
    // top of Ace of Spades)
    model.board_[0][6].stack_.top().set_card(Card('s',1,"1_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][1]);
    model.board_[0][6].stack_.top().set_card(Card('s',2,"2_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][1]);
    // Check that there are two cards in position [0][1] and 2 of spades is
    // on top
    CHECK(model.board_[0][1].stack_.top().get_value() == 2);
    CHECK(model.board_[0][1].stack_.top().get_suit() == 's');
    CHECK(model.board_[0][1].stack_.size()==2);
    CHECK(model.get_turn() == 4);
    // Move Ace of Hearts, 2 of Hearts and 3 of Hearts into a final position
    // [0][2], where 3 of hearts must be on top and the size of the stack
    // must be 3
    model.board_[0][6].stack_.top().set_card(Card('h',1,"1_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][2]);
    model.board_[0][6].stack_.top().set_card(Card('h',2,"2_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][2]);
    model.board_[0][6].stack_.top().set_card(Card('h',3,"3_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6],screen_pos[0][2]);
    // Check that 3 of hearts is on top and stack size is 3
    CHECK(model.board_[0][2].stack_.top().get_value() == 3);
    CHECK(model.board_[0][2].stack_.top().get_suit() == 'h');
    CHECK(model.board_[0][2].stack_.size()==3);
    CHECK(model.get_turn() == 7);

    // Now try to move 3 of Spades onto final position [0][1] where the
    // Spades suit is. It should not work.
    model.set_current_dragged(screen_pos[1][3]);
    model.play_move_drag(screen_pos[1][3],screen_pos[0][1]);
    // Since the move is illegal the top card in the spades final position
    // must still be 2 of spades, and 3 of spades must be the top card in its
    // starting position with 2 of diamonds below it and turn must not increment
    CHECK(model.board_[0][1].stack_.top().get_value() == 2);
    CHECK(model.board_[0][1].stack_.top().get_suit() == 's');
    CHECK(model.board_[0][1].stack_.size()==2);
    CHECK(model.board_[1][3].stack_.top().get_value() == 3);
    CHECK(model.board_[1][3].stack_.top().get_suit() == 's');
    CHECK(model.board_[2][3].stack_.top().get_value() == 2);
    CHECK(model.board_[2][3].stack_.top().get_suit() == 'd');
    CHECK(model.get_turn() == 7);

    // Move 3 of Spades to 4 of Hearts (the 2 of Diamonds must also move with
    // being moved to the grid below where 3 of spades is placed, which is
    // the grid below 4 of hearts). This is a valid move so it should work.
    model.set_current_dragged(screen_pos[1][3]);
    model.play_move_drag(screen_pos[1][3], screen_pos[1][1]);
    // Check if what was said on line 519-521 is true
    CHECK(model.board_[1][1].stack_.top().get_value() == 4);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 'h');
    CHECK(model.board_[2][1].stack_.top().get_value() == 3);
    CHECK(model.board_[2][1].stack_.top().get_suit() == 's');
    CHECK(model.board_[3][1].stack_.top().get_value() == 2);
    CHECK(model.board_[3][1].stack_.top().get_suit() == 'd');
    // Turn increases by 1 --> turns = 8
    CHECK(model.get_turn() == 8);

    // Try moving 4 of hearts to final position stack [0][2] (this move would
    // be legal is 4 of hearts was the bottom card, but since it is
    // 'attached' to 3 of spades and 2 of diamonds, this move is illegal).
    model.set_current_dragged(screen_pos[1][1]);
    model.play_move_drag(screen_pos[1][1], screen_pos[0][2]);
    // The position of the cards should not change
    CHECK(model.board_[1][1].stack_.top().get_value() == 4);
    CHECK(model.board_[1][1].stack_.top().get_suit() == 'h');
    CHECK(model.board_[2][1].stack_.top().get_value() == 3);
    CHECK(model.board_[2][1].stack_.top().get_suit() == 's');
    CHECK(model.board_[3][1].stack_.top().get_value() == 2);
    CHECK(model.board_[3][1].stack_.top().get_suit() == 'd');
    // Size of heart suit final position stack must be 3
    CHECK(model.board_[0][2].stack_.size() == 3);
    // Turn increases by 1 --> turns = 8
    CHECK(model.get_turn() == 8);
    // Move 2 of diamonds to diamond final position stack, move 3 of spades
    // to spades final position stack and move 4 of hearts to hearts final
    // position stack (all moves are legal if done in that order).
    model.set_current_dragged(screen_pos[3][1]);
    model.play_move_drag(screen_pos[3][1], screen_pos[0][0]);
    model.set_current_dragged(screen_pos[2][1]);
    model.play_move_drag(screen_pos[2][1], screen_pos[0][1]);
    model.set_current_dragged(screen_pos[1][1]);
    model.play_move_drag(screen_pos[1][1], screen_pos[0][2]);
    // Top of diamonds final position stack should be 2 of diamonds, top of
    // spades final position stack should be 3 of spades, and top of hearts
    // final position stack should be 4 of hearts
    CHECK(model.board_[0][2].stack_.top().get_value() == 4);
    CHECK(model.board_[0][2].stack_.top().get_suit() == 'h');
    // Size of hearts final position stack must be 4
    CHECK(model.board_[0][2].stack_.size() == 4);
    CHECK(model.board_[0][1].stack_.top().get_value() == 3);
    CHECK(model.board_[0][1].stack_.top().get_suit() == 's');
    // Size of spades final position stack must be 3
    CHECK(model.board_[0][1].stack_.size() == 3);
    CHECK(model.board_[0][0].stack_.top().get_value() == 2);
    CHECK(model.board_[0][0].stack_.top().get_suit() == 'd');
    // Size of diamonds final position stack must be 2
    CHECK(model.board_[0][0].stack_.size() == 2);
    // 3 turns played
    CHECK(model.get_turn() == 11);
    // No winner yet
    CHECK(model.is_winner() == false);
}

TEST_CASE("Game is won") {
    Model model;
    Test_access access(model);

    // check starting position is correct
    CHECK(model.board_[0][0].stack_.size() == 0);
    CHECK(model.board_[0][5].stack_.size() == 0);
    CHECK(model.board_[0][6].stack_.size() == 14);
    CHECK(model.board_[1][1].stack_.size() == 1);
    CHECK(model.board_[1][3].stack_.size() == 2);
    CHECK(model.board_[1][5].stack_.size() == 3);

    // Check that there is no winner
    CHECK(!model.is_winner());

    // Play through a game to check valid moves and that game can be won
    // (using an ideal, un-shuffled deck just for simplicity)
    model.board_[0][6].stack_.top().set_card(Card('h', 1, "1_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][0]);

    model.board_[0][6].stack_.top().set_card(Card('h', 2, "2_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][0]);

    model.board_[0][6].stack_.top().set_card(Card('h', 3, "3_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][0]);

    model.board_[0][6].stack_.top().set_card(Card('h', 4, "4_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][0]);

    model.board_[0][6].stack_.top().set_card(Card('h', 5, "5_h.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][0]);

    model.board_[0][6].stack_.top().set_card(Card('s', 1, "1_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][1]);

    model.board_[0][6].stack_.top().set_card(Card('s', 2, "2_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][1]);

    model.board_[0][6].stack_.top().set_card(Card('s', 3, "3_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][1]);

    model.board_[0][6].stack_.top().set_card(Card('s', 4, "4_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][1]);

    model.board_[0][6].stack_.top().set_card(Card('s', 5, "5_s.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][1]);

    model.board_[0][6].stack_.top().set_card(Card('d', 1, "1_d.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][2]);

    model.board_[0][6].stack_.top().set_card(Card('d', 2, "2_d.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][2]);

    model.board_[0][6].stack_.top().set_card(Card('d', 3, "3_d.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][2]);

    model.board_[0][6].stack_.top().set_card(Card('d', 4, "4_d.png"));
    model.set_current_dragged(screen_pos[0][6]);
    model.play_move_drag(screen_pos[0][6], screen_pos[0][2]);

    model.board_[1][1].stack_.top().set_card(Card('d', 5, "5_d.png"));
    model.set_current_dragged(screen_pos[1][1]);
    model.play_move_drag(screen_pos[1][1], screen_pos[0][2]);

    model.board_[1][3].stack_.top().set_card(Card('c', 1, "1_c.png"));
    model.set_current_dragged(screen_pos[1][3]);
    model.play_move_drag(screen_pos[1][3], screen_pos[0][3]);

    model.board_[1][3].stack_.top().set_card(Card('c', 2, "2_c.png"));
    model.set_current_dragged(screen_pos[1][3]);
    model.play_move_drag(screen_pos[1][3], screen_pos[0][3]);

    model.board_[1][5].stack_.top().set_card(Card('c', 3, "3_c.png"));
    model.set_current_dragged(screen_pos[1][5]);
    model.play_move_drag(screen_pos[1][5], screen_pos[0][3]);

    model.board_[1][5].stack_.top().set_card(Card('c', 4, "4_c.png"));
    model.set_current_dragged(screen_pos[1][5]);
    model.play_move_drag(screen_pos[1][5], screen_pos[0][3]);

    model.board_[1][5].stack_.top().set_card(Card('c', 5, "5_c.png"));
    model.set_current_dragged(screen_pos[1][5]);
    model.play_move_drag(screen_pos[1][5], screen_pos[0][3]);

    // check that the game is won
    CHECK(model.is_winner());

    // check that every position on the board is now invalid (e.g. the player
    // will not be able to play anymore moves)
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 6; ++j) {
            CHECK(!model.is_valid_move(screen_pos[0][0], screen_pos[i][j]));
        }
    }





}

// Test Access Definitions
Test_access::Test_access(Model& model)
        : model(model)
{ }



