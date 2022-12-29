#include "card.hxx"

Card::Card(char suit, int val, std::string filename)
        :filename(filename),
         suit_(suit),
         value_(val)
{
}

char
Card::get_suit()const
{
    return suit_;
}

int
Card::get_value()const
{
    return value_;
}

std::string
Card::get_filename() const
{
    return filename;
}

bool
Card::is_red() const
{
    return suit_ == 'h' || suit_ == 'd';
}

bool
Card::is_black() const
{
    return suit_ == 's' || suit_ == 'c';
}

void
Card::set_card(Card c)
{
    suit_ = c.get_suit();
    value_ = c.get_value();
    filename = c.get_filename();
}