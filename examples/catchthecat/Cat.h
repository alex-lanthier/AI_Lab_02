#ifndef CAT_H
#define CAT_H

#include "Agent.h"
#include <queue>
#include <stdexcept>

class Cat : public Agent {
 public:
  explicit Cat() : Agent(){};
  Point2D Move(World*) override;
};

#endif  // CAT_H
