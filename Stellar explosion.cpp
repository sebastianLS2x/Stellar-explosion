#include <SDL.h>
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

const int WIND_WIDTH = 900;
const int WIND_HEIGHT = 800;
const int SIZE = 1; // Change the size to 1 for higher resolution
const int ROWS = WIND_HEIGHT / SIZE;
const int COLS = WIND_WIDTH / SIZE;
const int UPPER_BOUND = 100;




// Function to convert HSV color to RGB color
void hSVtoRGB(int h, int s, int v, int& r, int& g, int& b) {
    int f = (h % 60) * 255 / 60;
    int p = (v * (255 - s)) / 255;
    int q = (v * (255 - (s * f) / 255)) / 255;
    int t = (v * (255 - (s * (255 - f)) / 255)) / 255;

    switch (h / 60) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    }
}


// Function to map the number of iterations to a color
SDL_Color mapToColor(int n) {
    if (n == UPPER_BOUND) {
        // Point is in the Mandelbrot set, color it black
        return { 0, 0, 0, 255 };
    }
    else if(n < UPPER_BOUND) {
        // Map number of iterations to a range of colors
        int hue = static_cast<int>(static_cast<double>(n) / UPPER_BOUND * 360.0);
        int saturation = 255;
        int value = 255;
        int r, g, b;
        hSVtoRGB(hue, saturation, value, r, g, b);
        return { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255 };
    }
}

int isMandelbrot(std::complex<double> c) {
    int k = 0;
    std::complex<double> z = 0.0;
    for (int i = 0; i < UPPER_BOUND; ++i) {
        z =  std::pow(z, 2) + c;
        k++;
        if (std::abs(z) > 2.0) {
            return k;
        }
    }
    return k; // Return the number of iterations
}

void updateGrid(std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> newGrid(ROWS, std::vector<int>(COLS));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            double real = (double(j) - WIND_WIDTH / 2.0) / (WIND_WIDTH/4.0);
            double imag = (double(i) - WIND_HEIGHT / 2.0) / (WIND_HEIGHT/4.0);
            std::complex<double> z(real, imag);
            int n = isMandelbrot(z);
            newGrid[i][j] = n;
        }
    }
    grid = newGrid;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIND_WIDTH, WIND_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<std::vector<int>> grid(ROWS, std::vector<int>(COLS));
    bool isRunning = true;
    SDL_Event event;

    
    


   

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

       

        updateGrid(grid);

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int n = grid[i][j];
                SDL_Rect cellRect = { j * SIZE, i * SIZE, SIZE, SIZE };
                SDL_Color color = mapToColor(n);
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }

        SDL_RenderPresent(renderer);
    

        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
