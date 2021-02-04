#include "explorer.h"

int main() {
    Explorer explorer("Explorer", 1280, 720, 100, 2560, 1440);
    while (explorer.windowIsOpen()) {
        explorer.handleInput();
        explorer.update();
        explorer.display();
    }
    return 0;
}