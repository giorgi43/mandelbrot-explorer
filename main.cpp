#include "explorer.h"
#include <iostream>
#include <cstdlib>
#include "cxxopts.hpp"
#include "palette.h"
using std::cout;
using std::cerr;
using std::endl;

cxxopts::ParseResult parseArgs(cxxopts::Options parser, int argc, char** argv[]) {
    try {
        cxxopts::ParseResult res = parser.parse(argc, *argv);
        return res;
    } catch (const cxxopts::OptionParseException& e) {
        cerr << "Could not parse command line arguments\n";
        std::cerr << e.what() << std::endl;
        std::exit(0);
    }
}

int main(int argc, char* argv[]) {
    cxxopts::Options parser("explorer", "Interactive mandelbrot explorer");
    parser.add_options()
        ("i,iterations", "Bailout value", cxxopts::value<int>()->default_value("100"))
        ("o,output-dir", "Output directory path to save images", cxxopts::value<std::string>()->default_value("./"))
        ("h,help", "Print this help message and exit");
    
    auto opts = parseArgs(parser, argc, &argv);

    if (opts.count("help")) {
      cout << parser.help() << endl;
      exit(0);
    }

    int iterations = opts["iterations"].as<int>();
    if (iterations > Explorer::max_iterations || iterations < Explorer::min_iterations) {
        cout << parser.help() << endl;
        cout << "Bailout value must be between " << Explorer::min_iterations << " and " << Explorer::max_iterations << endl;
        std::exit(0);
    }

    auto out_dir = opts["output-dir"].as<std::string>();
    if (out_dir.back() != '/') {
        const std::string od = out_dir + '/';
        out_dir = od;
    }

    Explorer explorer("Explorer", Explorer::Size2i(1280, 720), Explorer::Size2i(2560, 1440), iterations, out_dir);
    while (explorer.windowIsOpen()) {
        explorer.handleInput();
        explorer.update();
        explorer.display();
    }

    //constexpr Palette<500, 6> p(test_palette);
    //p.createPalette(test_palette);
    //std::array<Color, 500>& palette = p.getPalette();

    //static_assert(test_palette[0].getR() == 168);
    //uint8_t a = 4;
    //cout << static_cast<unsigned int>(c.getR()) << endl;

    /*pl::Palette<500, 6> p(pl::test_colors);

    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
	
		for (int i = 0; i < 500; i++) {
			sf::RectangleShape v(sf::Vector2f(1, 500));
			v.setPosition(i,0);
            sf::Color c(p[i].r, p[i].g, p[i].b, p[i].a);
			v.setFillColor(c);
			window.draw(v);
		}
        
        window.display();
    }*/


    return 0;
}