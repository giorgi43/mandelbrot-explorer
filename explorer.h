#ifndef EXPLORER_H
#define EXPLORER_H
#include <SFML/Graphics.hpp>
#include <complex>

class Explorer {
public:
    Explorer() = delete;
    Explorer(const Explorer&) = delete;
    Explorer& operator==(const Explorer&) = delete;
    Explorer(const std::string& window_title, unsigned width, unsigned height, unsigned iterations=100, unsigned out_width=1280, unsigned out_height=720);
    ~Explorer() = default;
    bool windowIsOpen() const { return m_window.isOpen(); };
    void handleInput();
    void update();
    void display();
private:
    sf::RenderWindow m_window;
    const unsigned m_width, m_height;
    unsigned m_iterations;
    sf::VertexArray m_pixels;

    unsigned m_out_width, m_out_height;
    sf::RenderTexture m_out_texture;
    sf::VertexArray m_out_pixels;

    double m_re_start = -2.0;
    double m_re_end = 1.56;
    double m_im_start = -1.0;
    double m_im_end = 1.0;
    double m_step = 0.05;
    double m_zoom = 0.9;
    bool m_update = true;
    bool m_mouse_pressed = false;

    sf::RectangleShape m_select;
    static constexpr int escape_radius = 2;
    static constexpr unsigned max_iterations = 4000;
    static constexpr unsigned min_iterations = 100;

    static sf::Color hsv_to_rgb(int hue, float sat, float val);
    static std::string generateFilename(const std::string& start = "");
    double calculateIterations(const std::complex<double>& c, unsigned iterations) const;
    void initPixels(sf::VertexArray& pixels, unsigned width, unsigned height);
    void setSelectBounds(); // set new imaginary plane bounds based on selection box
    void makeZoom(double zoom);
    inline void render(sf::VertexArray& pixels, unsigned width, unsigned height);
    void handleMouse(const sf::Event& e);
    void handleKeyboard(const sf::Event& e);
};

#endif
