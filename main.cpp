/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file main.cpp
 * @brief Main file for the program
 * @details This file contains the main function for the program
 */

#include <iostream>
#include "Window.cpp"

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *WINDOW_TITLE = "Spearstake";
const std::pair<int, int> WINDOW_DIMENSIONS = std::make_pair(WINDOW_WIDTH, WINDOW_HEIGHT);

int main()
{
    // Print hello world
    std::cout << "Starting Spearstake..." << std::endl;

    // Create a Spearstake object
    Spearstake spearstake(WINDOW_DIMENSIONS, WINDOW_TITLE, "");

    // Run the Spearstake object
    spearstake.run();

    return 0;
}