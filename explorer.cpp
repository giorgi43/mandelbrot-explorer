#include "explorer.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <stdexcept>

Explorer::Explorer(const std::string &window_title, const Size2i& window_size, const Size2i& output_image_size, unsigned iterations)
    : m_window(sf::VideoMode(window_size.width, window_size.height), window_title, sf::Style::Titlebar | sf::Style::Close),
      m_window_size(window_size), m_out_image_size(output_image_size),
      m_iterations{(iterations >= min_iterations && iterations <= max_iterations) ? iterations : throw std::runtime_error("Iterations value out of bounds")} {

    m_window.setVerticalSyncEnabled(true);
    m_window.setPosition(sf::Vector2i(400,400));
    m_pixels = sf::VertexArray(sf::Points, m_window_size.width*m_window_size.height);
    initPixels(m_pixels, m_window_size);

    m_out_pixels = sf::VertexArray(sf::Points, m_out_image_size.width*m_out_image_size.height);
    initPixels(m_out_pixels, m_out_image_size);
    if (!m_out_texture.create(m_out_image_size.width, m_out_image_size.height)) {
       throw std::runtime_error("Texture creation error");
    }

    m_select.setPosition(0,0);
    m_select.setOutlineThickness(2.0f);
    m_select.setFillColor(sf::Color(0,0,0,0));
    m_select.setOutlineColor(sf::Color(255,255,255,255));
}

void Explorer::handleInput() {
    sf::Event e{};
    while (m_window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            m_window.close();
        }
        handleKeyboard(e);
        handleMouse(e);
    }
}

void Explorer::update() {
    if (!m_update) return;
    m_update = false;
    render(m_pixels, m_window_size);
}

void Explorer::render(sf::VertexArray& pixels, const Size2i& size) {
    #pragma omp parallel for schedule(dynamic)
    for (unsigned x = 0; x < size.width; x++) {
        for (unsigned y = 0; y < size.height; y++) {
            std::complex<double> c(m_re_start + (x / static_cast<double>(size.width)) * (m_re_end - m_re_start),
                                   m_im_start + (y / static_cast<double>(size.height)) * (m_im_end - m_im_start));
            auto iter = calculateIterations(c, m_iterations);
            int h = 255 * iter / m_iterations;
            float s = 255.0f;
            float v = (iter < m_iterations) ? 255.0f : 0.0f;
            pixels[x*size.height + y].color = hsv_to_rgb(h,s,v);
        }
    }
}

void Explorer::display() {
    m_window.clear();
    m_window.draw(m_pixels);
    if (m_mouse_pressed) {
        m_window.draw(m_select);
    }
    m_window.display();
}

void Explorer::setSelectBounds() {
    m_select.setOrigin(0,0);
    auto x0 = m_select.getPosition().x;
    auto y0 = m_select.getPosition().y;
    auto select_width = m_select.getSize().x;
    auto select_height = m_select.getSize().y;
    if (select_height < 0 || select_width < 0) {
        if (select_height < 0)
            y0 = y0 - std::abs(select_height);
        if (select_width < 0)
            x0 = x0 - std::abs(select_width);
    }

    const double re_factor = (m_re_end - m_re_start) / (m_window_size.width-1);
    const double im_factor = (m_im_end - m_im_start) / (m_window_size.height-1);
    const double new_re_start = m_re_start + (re_factor * x0);
    const double new_re_end = m_re_start + (re_factor * (x0 + std::abs(select_width)));
    const double new_im_start = m_im_start + (im_factor * y0);
    const double new_im_end = m_im_start + (im_factor * (y0 + std::abs(select_height)));

    m_step = (m_step*(new_im_end-new_im_start))/(m_im_end - m_im_start);

    m_re_start = new_re_start;
    m_re_end = new_re_end;
    m_im_start = new_im_start;
    m_im_end = new_im_end;
}

void Explorer::initPixels(sf::VertexArray& pixels, const Size2i& size) {
    for (unsigned int x = 0; x < size.width; x++) {
        for (unsigned int y = 0; y < size.height; y++) {
            pixels[x*size.height + y].position = sf::Vector2f(x,y);
        }
    }
}

double Explorer::calculateIterations(const std::complex<double> &c, unsigned max_iterations) const {
    std::complex<double> z = 0;
    unsigned iterations = 0;
    for (; iterations < max_iterations; ++iterations) {
        // faster than std::abs(z) :/
        if (std::sqrt(std::norm(z)) > Explorer::escape_radius) break;
        z = z*z + c;
    }
    if (iterations == m_iterations) {
        return m_iterations;
    }
    return iterations + 1 - std::log(std::log2(std::abs(z)));
}

void Explorer::makeZoom(double zoom) {
    const double old_im_start = m_im_start;
    const double old_im_end = m_im_end;
    const double w = m_re_end - m_re_start;
    const double h = m_im_end - m_im_start;
    const double ww = (w - w*zoom)/2.0;
    const double hh = (h - h*zoom)/2.0;
    m_re_start += ww;
    m_re_end -= ww;
    m_im_start += hh;
    m_im_end -= hh;
    m_step = (m_step*(m_im_end-m_im_start))/(old_im_end - old_im_start);
}

sf::Color Explorer::hsv_to_rgb(int hue, float sat, float val) {
    hue %= 360;
    while(hue<0) hue += 360;

    if(sat<0.f) sat = 0.f;
    if(sat>1.f) sat = 1.f;

    if(val<0.f) val = 0.f;
    if(val>1.f) val = 1.f;

    int h = hue/60;
    float f = float(hue)/60-h;
    float p = val*(1.f-sat);
    float q = val*(1.f-sat*f);
    float t = val*(1.f-sat*(1-f));

    switch(h) {
        default:
        case 0:
        case 6: return sf::Color(val*255, t*255, p*255);
        case 1: return sf::Color(q*255, val*255, p*255);
        case 2: return sf::Color(p*255, val*255, t*255);
        case 3: return sf::Color(p*255, q*255, val*255);
        case 4: return sf::Color(t*255, p*255, val*255);
        case 5: return sf::Color(val*255, p*255, q*255);
    }
}

std::string Explorer::generateFilename(const std::string &start) {

    const std::time_t now = std::time(nullptr);
    const std::tm calendar_time = *std::localtime(std::addressof(now));

    const auto year = std::to_string(calendar_time.tm_year + 1900);
    const auto hour = std::to_string(calendar_time.tm_hour);
    const auto minute = std::to_string(calendar_time.tm_min);
    const auto second = std::to_string(calendar_time.tm_sec);

    // example-2021T224532
    const std::string filename = start + "-" + year + "T" + hour + minute + second;
    return filename;
}

void Explorer::saveToFile(const std::string& filename) {
    std::cout << "Saving current frame\n";
    render(m_out_pixels, m_out_image_size);
    m_out_texture.clear();
    m_out_texture.draw(m_out_pixels);
    m_out_texture.display();
    const auto texture = m_out_texture.getTexture();
    const auto image = texture.copyToImage();
    if (image.saveToFile(filename)) {
        std::cout << "Saved to file: " << filename << std::endl;
    } else {
        std::cout << "Could not save to file" << std::endl;
    }
}

void Explorer::handleMouse(const sf::Event& e) {
    switch (e.type) {
        case sf::Event::MouseButtonPressed: {
            if (e.mouseButton.button == sf::Mouse::Left) {
                m_mouse_pressed = true;
                m_select.setPosition(sf::Vector2f(e.mouseButton.x, e.mouseButton.y));
            }
            break;
        }
        case sf::Event::MouseButtonReleased: {
            if (e.mouseButton.button == sf::Mouse::Left) {
                m_mouse_pressed = false;
                if (m_select.getSize().x != 0) { // don't update on just single click
                    m_update = true;
                    setSelectBounds();
                }
                m_select.setSize(sf::Vector2f(0,0));
            }
            break;
        }
        case sf::Event::MouseMoved: {
            if (m_mouse_pressed) {
                // mouse dragged
                float w = e.mouseMove.x - m_select.getPosition().x;
                float h;
                if (e.mouseMove.y < m_select.getPosition().y) {
                    h = w * 2/3.0;
                    if (w > 0) h *= -1;
                } else {
                    h = std::abs(w * 9/16.0);
                }
                m_select.setSize(sf::Vector2f(w,h));
            }
        }
        case sf::Event::MouseWheelScrolled: {
            if (e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                m_update = true;
                makeZoom((e.mouseWheelScroll.delta > 0) ? m_zoom : 1/m_zoom);
            }
            break;
        }
    }
}

void Explorer::handleKeyboard(const sf::Event &e) {
    if (e.type == sf::Event::KeyPressed) {
        m_update = true;
        switch (e.key.code) {
            case sf::Keyboard::Equal: // zoom in
                makeZoom(m_zoom); break;
            case sf::Keyboard::Hyphen: // zoom out
                makeZoom(1/m_zoom); break;
            case sf::Keyboard::A:
                m_re_start -= m_step;
                m_re_end -= m_step;
                break;
            case sf::Keyboard::W:
                m_im_start -= m_step;
                m_im_end -= m_step;
                break;
            case sf::Keyboard::S:
                m_im_start += m_step;
                m_im_end += m_step;
                break;
            case sf::Keyboard::D:
                m_re_start += m_step;
                m_re_end += m_step;
                break;
            case sf::Keyboard::J:
                if (m_iterations >= Explorer::min_iterations+100) m_iterations -= 100;
                break;
            case sf::Keyboard::K:
                if (m_iterations <= Explorer::max_iterations-100) m_iterations += 100;
                break;
            case sf::Keyboard::P: {
                m_update = false;
                const auto filename = "../images/" + Explorer::generateFilename("explorer") + ".png";
                saveToFile(filename);
                break;
            }
            default: m_update = false; break;
        }
    }
}
