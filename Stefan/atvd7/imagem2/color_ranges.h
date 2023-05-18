#include <vector>
#include <tuple>

#ifndef COLOR_RANGES_H
#define COLOR_RANGES_H

// tennis -> (hMin = 30 , sMin = 130, vMin = 170), (hMax = 41 , sMax = 150, vMax = 245)
// basketball -> (hMin = 7 , sMin = 138, vMin = 208), (hMax = 12 , sMax = 208, vMax = 255)
// volley -> (hMin = 18 , sMin = 148, vMin = 180), (hMax = 26 , sMax = 200, vMax = 255)
// soccer -> (hMin = 42 , sMin = 0, vMin = 87), (hMax = 150 , sMax = 150, vMax = 235)
// baseball -> (hMin = 0 , sMin = 40, vMin = 190), (hMax = 6 , sMax = 240, vMax = 233)

static std::tuple<int, int, int> tennis_min = {30, 130, 170};
static std::tuple<int, int, int> tennis_max = {41, 150, 245};

static std::tuple<int, int, int> basket_min = {7, 138, 208};
static std::tuple<int, int, int> basket_max = {12, 208, 255};

static std::tuple<int, int, int> volley_min = {18, 148, 180};
static std::tuple<int, int, int> volley_max = {26, 200, 255};

static std::tuple<int, int, int> soccer_min = {42, 0, 87};
static std::tuple<int, int, int> soccer_max = {150, 150, 235};

static std::tuple<int, int, int> baseball_min = {0, 40, 190};
static std::tuple<int, int, int> baseball_max = {6, 240, 233};

static std::vector<std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>>> balls_color_ranges = {
    make_pair(tennis_min, tennis_max),
    make_pair(basket_min, basket_max),
    make_pair(volley_min, volley_max),
    make_pair(soccer_min, soccer_max),
    make_pair(baseball_min, baseball_max)
};

#endif