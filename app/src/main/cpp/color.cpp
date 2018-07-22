#include "color.h"

const Color& Color::Black  = {   0,   0,   0, 255 };
const Color& Color::Blue   = {   0,   0, 255, 255 };
const Color& Color::Green  = {   0, 255,   0, 255 };
const Color& Color::Cyan   = {   0, 255, 255, 255 };
const Color& Color::Red    = { 255,   0,   0, 255 };
const Color& Color::Pink   = { 255,   0, 255, 255 };
const Color& Color::Yellow = { 255, 255,   0, 255 };
const Color& Color::White  = { 255, 255, 255, 255 };

Color::Color() : r(0), g(0), b(0), a(0) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
