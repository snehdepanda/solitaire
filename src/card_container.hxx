#pragma once
#include <ge211.hxx>
#include <stack>
#include "card.hxx"

class Card_Container
{
public:
    Card_Container();
    std::stack<Card> stack_;
    std::stack<ge211::Image_sprite> image_stack_;
};

