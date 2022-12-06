#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include "Generator.h"
#include <iostream>
#include <cmath>
// do not use this one to your assignment. this is my sample generator
std::vector<Color32> Generator::Generate(int sideSize,
                                                       float displacement) {
  std::vector<Color32> colors;
  //   create your own function for noise generation
  FastNoiseLite base;
  base.SetFractalOctaves(3);
  base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
  base.SetFractalType(FastNoiseLite::FractalType_FBm);
  FastNoiseLite cellular;
  cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
  cellular.SetFractalOctaves(3);
  for (int l = 0; l < sideSize; l++) {
    for (int c = 0; c < sideSize; c++) {
      float c1 =
          ((base.GetNoise((float)c, (float)l, displacement * 50) + 1) / 2) *
          255;
      float c2 =
          ((cellular.GetNoise((float)c, (float)l, displacement * 50) + 1) / 2) *
          255;
      auto avg = (c1 + c2) / 2;

      //calculate island location
      float nx = sideSize / 2;
      float ny = sideSize / 2;

      float d = std::sqrt((nx - c) * (nx - c) + (ny - l) * (ny - l));

      avg = avg * (1 - d / (std::sqrt(2) * sideSize / 2));

      //calculate color
      if (avg < 50) {
          Color32 from = Color::Black, to = Color::Red; 
          float a = avg / 50;
          Color32 avgColor((to.r - from.r) * a, (to.g - from.g) * a, (to.b - from.b) * a);
          colors.emplace_back(avgColor);
      } 
      else if (avg < 100) {
        Color32 from = Color::Red, to = Color::Green;
        float a = (avg - 50) / 50;
        Color32 avgColor((to.r - from.r) * a, (to.g - from.g) * a, (to.b - from.b) * a);
        colors.emplace_back(avgColor);
      } 
      else if (avg < 150) {
        Color32 from = Color::Green, to = Color::Blue;
        float a = (avg - 150) / 50;
        Color32 avgColor((to.r - from.r) * a, (to.g - from.g) * a, (to.b - from.b) * a);
        colors.emplace_back(avgColor);
      } 
      else{
        Color32 from = Color::Blue, to = Color::White;
        float a = (avg - 205) / 50;
        Color32 avgColor((to.r - from.r) * a, (to.g - from.g) * a, (to.b - from.b) * a);
        colors.emplace_back(avgColor);
      }
    }
  }
  std::cout << colors.size() << std::endl;
  return colors;
}
std::string Generator::GetName() { return "Alex"; }
