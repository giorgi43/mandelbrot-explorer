#include "explorer.h"
#include <iostream>
using std::cout;
using std::endl;

int main() {
    Explorer explorer("Explorer", Explorer::Size2i(1280, 720), Explorer::Size2i(2560, 1440));
    while (explorer.windowIsOpen()) {
        explorer.handleInput();
        explorer.update();
        explorer.display();
    }
    return 0;
}