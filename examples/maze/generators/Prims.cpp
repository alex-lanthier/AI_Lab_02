#include "Prims.h"

Prims::Prims() {}

std::string Prims::GetName() { return std::string("Prim's"); }

bool Prims::Step(World* world) { 
    // check if we need to find a new starting point
  if (stack.empty()) {
    auto point = randomStartPoint(world);
    if (point.x == INT_MAX && point.y == INT_MAX)
      return false;  // no empty space no fill
    stack.push_back(point);
    world->SetNodeColor(point, Color::Red.Dark());
  }

	return false; 
}

void Prims::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D Prims::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> Prims::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  // todo: improve this
  // north
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y - 1) <= sideOver2) &&  // should be inside the board
      !visited[p.y - 1][p.x] &&       // not visited yet
      w->GetNorth({p.x, p.y - 1}))    // has wall
    visitables.emplace_back(p.x, p.y - 1);
  // east
  if ((abs(p.x + 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      !visited[p.y][p.x + 1] &&     // not visited yet
      w->GetNorth({p.x + 1, p.y}))  // has wall
    visitables.emplace_back(p.x + 1, p.y);
  // south
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y + 1) <= sideOver2) &&  // should be inside the board
      !visited[p.y + 1][p.x] &&       // not visited yet
      w->GetNorth({p.x, p.y + 1}))    // has wall
    visitables.emplace_back(p.x, p.y + 1);
  // west
  if ((abs(p.x - 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      !visited[p.y][p.x - 1] &&     // not visited yet
      w->GetNorth({p.x - 1, p.y}))  // has wall
    visitables.emplace_back(p.x - 1, p.y);

  return visitables;
}

Prims::~Prims() {}
