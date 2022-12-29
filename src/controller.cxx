#include "controller.hxx"
using Position = ge211::Posn<int>;
static Position mouse_ = {0,0};
static Position mouse_start_ = {0,0};

Controller::Controller()
        :view_(model_),
         mouse_moved_(false)
{

}

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, mouse_);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_mouse_down(ge211::Mouse_button button, ge211::Posn<int> pos)
{
    mouse_start_ = pos;
    mouse_moved_ = false;
    model_.is_dragging = true;
    model_.set_current_dragged(pos);
    //model_.play_move_click(view_.screen_to_board(pos.x,pos.y));
}

void
Controller::on_mouse_up(ge211::events::Mouse_button button, ge211::Posn<int>
pos) {
    if (!mouse_moved_){
        model_.play_move_click(view_.screen_to_board(pos.x,pos.y));
    } else {
        model_.play_move_drag(mouse_start_, pos);
        model_.is_dragging = false;
        mouse_moved_ = false;
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> pos) {
    mouse_ = pos;
    mouse_moved_ = true;
}