# mandelbrot-explorer
This is interactive [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set) explorer with support of panning and zooming. You can also save favourite views as high resolution images. 
Rendering is done by CPU so utilizing all of its available cores and making them run in parallel is good idea to make processing faster.
thats quite straightforward since generating Mandelbrot set is [Embarrassingly parallel](https://en.wikipedia.org/wiki/Embarrassingly_parallel) task. 

## Build
You need C++ compiler with support of openMP. GCC provides support since version 4.2.0. Also you need [SFML](https://www.sfml-dev.org/) library installed. you can build
from sources or on Debian/Ubuntu run:
```
apt install libsfml-dev
```
clone this repository and `cd` to its directory
```
git clone https://github.com/giorgi43/mandelbrot-explorer.git
cd mandelbrot-explorer/
cmake -S . -B bin
cd bin/
make
```
## Usage


