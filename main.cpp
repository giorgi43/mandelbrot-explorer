#include "explorer.h"
#include <iostream>
#include <cstdlib>
#include "cxxopts.hpp"
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
    return 0;
}