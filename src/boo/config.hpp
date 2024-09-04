#pragma once

#include <string>

struct Config {
    int fps = 240;
    int screenWidth = 1024;
    int screenHeight = 768;
    std::string windowTitle = "boo";
};

inline Config config;