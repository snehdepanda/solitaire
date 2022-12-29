#pragma once
#include <ge211.hxx>

class Card
{
public:

    Card(char, int, std::string);


    bool is_red() const;
    bool is_black() const;
    char get_suit() const;
    int get_value() const;
    std::string get_filename() const;
    std::string filename;
    void set_card(Card);

private:
    char suit_;
    int value_;

};
