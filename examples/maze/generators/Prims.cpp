#include "Prims.h"
#include "Random.h"

Prims::Prims() {}

std::string Prims::GetName() { return std::string("Prim's"); }

bool Prims::Step(World* world) { 
    //add random start point to stack if stack is empty
    if (stack.empty()) {
      auto point = randomStartPoint(world);
      if (point.x == INT_MAX && point.y == INT_MAX)
        return false;  // no empty space no fill
      stack.push_back(point);
      world->SetNodeColor(point, Color::Red.Dark());
    }

    //get random point from the stack to use as the current node
    auto randomIndex = Random::Range(0, stack.size() - 1);
    auto current = stack[randomIndex];
    auto visitables = getVisitables(world, current);
    world->SetNodeColor(current, Color::DarkGray);

    //add neighbors to the stack
    for (int i = 0; i < visitables.size(); i++) {
      stack.push_back(visitables[i]);
    }

    world->SetNodeColor(current, Color::Black);

    //remove current from the stack
    stack.erase(stack.begin() + randomIndex);

    for each (Point2D iter in stack) {
      world->SetNodeColor(iter, Color::Red.Dark());
    }

    if (!stack.empty()) {
      auto visitedNeighbors = getVisitedNeighbors(world, current);

      if (!visitedNeighbors.empty()) {
        auto visitedPoint =
            visitedNeighbors[Random::Range(0, visitedNeighbors.size() - 1)];

        auto delta = visitedPoint - current;

        world->SetNodeColor(current, Color::Black);

        // remove walls
        if (delta.y == -1)  // north
          world->SetNorth(current, false);
        else if (delta.x == 1)  // east
          world->SetEast(current, false);
        else if (delta.y == 1)  // south
          world->SetSouth(current, false);
        else if (delta.x == -1)  // west
          world->SetWest(current, false);
        else
          return false;  // this should never happen;
      }
    }

	return true; 
}

void Prims::Clear(World* world) {
  stack.clear();
}

Point2D Prims::randomStartPoint(World* world) {
  return {-world->GetSize()/2, -world->GetSize()/2};
}

std::vector<Point2D> Prims::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  // north
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y - 1) <= sideOver2) &&  // should be inside the board
      w->GetNodeColor(p + Point2D::UP) == Color::DarkGray)  // not visited yet
    visitables.emplace_back(p.x, p.y - 1);
  // east
  if ((abs(p.x + 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&  // should be inside the board
      w->GetNodeColor(p + Point2D::RIGHT) ==
          Color::DarkGray)  // not visited yet
    visitables.emplace_back(p.x + 1, p.y);
  // south
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y + 1) <= sideOver2) &&  // should be inside the board
      w->GetNodeColor(p + Point2D::DOWN) == Color::DarkGray)  // not visited yet
    visitables.emplace_back(p.x, p.y + 1);
  // west
  if ((abs(p.x - 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&  // should be inside the board
      w->GetNodeColor(p + Point2D::LEFT) == Color::DarkGray)  // not visited yet
    visitables.emplace_back(p.x - 1, p.y);

  return visitables;
}

std::vector<Point2D> Prims::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT,
                                 Point2D::RIGHT};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  for (auto delta : deltas) {
    auto neigh = p + delta;
    if ((abs(neigh.x) <= sideOver2 &&
         abs(neigh.y) <= sideOver2) &&           // should be inside the board
        w->GetNodeColor(neigh) == Color::Black)  // visited
    {
      bool wall;
      if (delta.y == -1)  // north
        wall = w->GetNorth(p);
      else if (delta.x == 1)  // east
        wall = w->GetEast(p);
      else if (delta.y == 1)  // south
        wall = w->GetSouth(p);
      else if (delta.x == -1)  // west
        wall = w->GetWest(p);
      else
        wall = false;  // this should never happens

      if (wall)  // it should have wall
        neighbors.emplace_back(neigh);
    }
  }

  return neighbors;
}

Prims::~Prims() {}
