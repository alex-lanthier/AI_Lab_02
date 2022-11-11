#include "Prims.h"
#include "Random.h"

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

  // visit the current element
  auto current = stack.back();
  visited[current.y][current.x] = true;
  world->SetNodeColor(current, Color::Red.Dark());

  // check if we should go deeper
  std::vector<Point2D> visitables = getVisitables(world, current);

  if (!visitables.empty()) {
      //get random visitable
    auto next = visitables[Random::Range(0, visitables.size() - 1)];

    //delete random wall between next and a previously visited node
    auto visitedNeighbors = getVisitedNeighbors(world, next);
    if (visitedNeighbors.empty())
      return false;  // this should never happen. if we are in this state, the code is wrong

    auto visitedPoint = visitedNeighbors[Random::Range(0, visitedNeighbors.size() - 1)];
    auto delta = visitedPoint - next;

    // remove walls
    if (delta.y == -1)  // north
      world->SetNorth(next, false);
    else if (delta.x == 1)  // east
      world->SetEast(next, false);
    else if (delta.y == 1)  // south
      world->SetSouth(next, false);
    else if (delta.x == -1)  // west
      world->SetWest(next, false);
    else
      return false;  // this should never happen;

    stack.push_back(next);
  }

	return true; 
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

std::vector<Point2D> Prims::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  for (auto delta : deltas) {
    auto neigh = p + delta;
    if ((abs(neigh.x) <= sideOver2 &&
         abs(neigh.y) <= sideOver2) &&  // should be inside the board
        visited[neigh.y][neigh.x])      // visited
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
