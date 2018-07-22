#pragma once

struct Color {
    static const Color& Black;
    static const Color& Blue;
    static const Color& Green;
    static const Color& Cyan;
    static const Color& Red;
    static const Color& Pink;
    static const Color& Yellow;
    static const Color& White;

    unsigned char r, g, b, a;

    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};