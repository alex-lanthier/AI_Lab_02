#pragma once
#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include "../World.h"
#include <map>
#include <vector>

class Prims : public MazeGeneratorBase {
 private:
  std::vector<Point2D> stack;
  Point2D randomStartPoint(World* world);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);

 public:
  Prims();
  std::string GetName() override;
  // Step should return true if it made changes in the world
  bool Step(World* world) override;
  // Clears and resets all data from the generator
  void Clear(World* world) override;
  std::vector<Point2D> getVisitedNeighbors(World* w, const Point2D& p);
  ~Prims() override;

  std::vector<Point2D> visitables;
};