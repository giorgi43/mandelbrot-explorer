#ifndef PALETTE_H
#define PALETTE_H
#include <array>
#include <cstdint>
#include <iostream>
namespace pl 
{

class Color {
public:
    constexpr Color() : r(0), g(0), b(0), a(0) {};
    constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {};
    constexpr inline uint8_t getR() const { return r; };
    constexpr inline uint8_t getG() const { return g; };
    constexpr inline uint8_t getB() const { return b; };
    constexpr inline uint8_t getA() const { return a; };
    inline sf::Color toSfmlColor() const { return sf::Color(r,g,b,a); };
    uint8_t r,g,b,a;
};

constexpr std::array<Color, 6> test_colors {Color(168,22,22,255), Color(36, 89,173,255), Color(230,123,41,255), Color(64,145,58,255), Color(54,24,105,255), Color(168, 22, 22,255)};

template <std::size_t sz, std::size_t num_colors>
class Palette {
public:
    // array must have at least 2 colors
    Palette(const std::array<Color, num_colors>& stop_colors) : m_num_segments(num_colors-1), m_segment_size(sz / (m_num_segments)) {
        createPalette(stop_colors);
    }
    Color operator[](size_t index) const {
        return m_palette[index];
    }
    ~Palette() = default;
public:
    std::size_t m_num_segments;
    std::size_t m_segment_size;
    Color m_palette[sz];

    inline size_t getStartIndex(std::size_t segment_index) const {
        return (segment_index * m_segment_size);
    }

    inline size_t getEndIndex(std::size_t segment_index) const {
        return getStartIndex(segment_index) + m_segment_size - 1;
    }

    void createGradient(const Color& lhs, const Color& rhs, std::size_t start, std::size_t end) {
        size_t size = end - start + 1;
        for (auto x = start; x <= end; x++) {
            auto p = (x-start) / static_cast<float>(size-1);
            auto r = static_cast<int>((1.0-p) * lhs.getR() + p * rhs.getR() + 0.5);
            auto g = static_cast<int>((1.0-p) * lhs.getG() + p * rhs.getG() + 0.5);
            auto b = static_cast<int>((1.0-p) * lhs.getB() + p * rhs.getB() + 0.5);
            m_palette[x] = Color(r,g,b,255);
        }
    }

    void createPalette(const std::array<Color, num_colors>& colors) {
        for (auto s = 0; s < m_num_segments; s++) {
            std::cout << getStartIndex(s) << " " << getEndIndex(s) << std::endl;
            createGradient(colors[s], colors[s+1], getStartIndex(s), getEndIndex(s));
        }
    }


};
    
}
#endif