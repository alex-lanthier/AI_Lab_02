#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <queue>

Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0,5);

  auto cat = world->getCat();

  std::vector<queueEntry> queue; 
  //bootstrap the first element into the queue
  queue.push_back({cat, cat, 0});

  //while we have elements to be visited, visit them
  while (!queue.empty()) 
  {
    auto head = queue[0];
    queue.erase(queue.begin());

    //mark the head as visited
    visited.push_back(head);

    //for each neighbor:
    for (int i = 0; i < World::neighbors(head.position).size(); i++) {
      // - check if it is not visited yet
      bool isVisited = false; 

      for (int j = 0; j < visited.size(); j++) 
      {
        if (World::neighbors(head.position)[i] == visited[j].position) 
        {
          isVisited = true;
        }
      }

      // - check if it is not in the queue
      bool inQueue = false;

      for (int j = 0; j < queue.size(); j++) {
        if (World::neighbors(head.position)[i] == queue[j].position) {
          inQueue = true;
        }
      }

      // - check to make sure it isn't blocked
      bool isBlocked = false;

      if (world->getContent(World::neighbors(head.position)[i])) {
        isBlocked = true;
      }

      // - add to queue and add 1 to weight
      if (!isVisited && !inQueue && !isBlocked)
      {
        queue.push_back({World::neighbors(head.position)[i], head.position, head.weight + 1});

        // - mark the neighbor it came from as the current head

      }
    }
    //code win condition and navigation process
  }

  auto pos = world->getCat();
  switch(rand){
    case 0:
      return World::NE(pos);
    case 1:
      return World::NW(pos);
    case 2:
      return World::E(pos);
    case 3:
      return World::W(pos);
    case 4:
      return World::SW(pos);
    case 5:
      return World::SE(pos);
    default:
      throw "random out of range";
  }
}
