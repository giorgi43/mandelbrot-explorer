#ifndef EXPLORER_H
#define EXPLORER_H
#include <SFML/Graphics.hpp>
#include <complex>

class Explorer {
public:
    static constexpr int escape_radius = 2;
    static constexpr unsigned max_iterations = 4000;
    static constexpr unsigned min_iterations = 100;

    struct Size2i {
        public:
            unsigned width, height;
            Size2i(unsigned w, unsigned h) : width(verify(w)), height(verify(h)) {};
        private:
            static unsigned verify(unsigned n) {
                if (n <= 0) throw std::runtime_error("Width and height must be positive integers");
                return n;
            }
    };

    Explorer(const Explorer&) = delete;
    Explorer& operator==(const Explorer&) = delete;
    Explorer(const std::string& window_title, const Size2i& window_size = {1280, 720}, const Size2i& output_image_size = {1920, 1080}, unsigned iterations=100, const std::string& output_dir="./");
    ~Explorer() = default;
    bool windowIsOpen() const { return m_window.isOpen(); };
    void handleInput();
    void update();
    void display();

private:
    sf::RenderWindow m_window;
    const Size2i m_window_size;
    unsigned m_iterations;
    sf::VertexArray m_pixels;

    const Size2i m_out_image_size;
    sf::RenderTexture m_out_texture;
    sf::VertexArray m_out_pixels;

    const std::string m_out_dir;

    // complex plane bounds make 16:9 ration
    double m_re_start = -2.0;
    double m_re_end = 1.56;
    double m_im_start = -1.0;
    double m_im_end = 1.0;
    double m_step = 0.05;
    double m_zoom = 0.9;
    bool m_update = true;
    bool m_mouse_pressed = false;

    sf::RectangleShape m_select;

    static sf::Color hsv_to_rgb(int hue, float sat, float val);
    static std::string generateFilename(const std::string& start = "");
    double calculateIterations(const std::complex<double>& c, unsigned iterations) const;
    void initPixels(sf::VertexArray& pixels, const Size2i& size);
    void setSelectBounds(); // set new complex plane bounds based on selection box
    void makeZoom(double zoom);
    inline void render(sf::VertexArray& pixels, const Size2i& size);
    void saveToFile(const std::string& filename);
    void handleMouse(const sf::Event& e);
    void handleKeyboard(const sf::Event& e);
};

#endif
