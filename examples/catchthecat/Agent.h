#ifndef AGENT_H
#define AGENT_H
#include "Point2D.h"
#include <vector>
#include <map>
#include <algorithm>

class World;
struct queueEntry;

class Agent {
public:
  explicit Agent()= default;
  virtual Point2D Move(World*)=0;

  std::vector<queueEntry> visited;
};

struct queueEntry
{
  Point2D position;
  Point2D origin;
  int weight;
};

#endif  // AGENT_H
