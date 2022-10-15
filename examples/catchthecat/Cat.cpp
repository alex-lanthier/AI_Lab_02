#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <queue>

Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0,5);

  auto cat = world->getCat();

  std::vector<queueEntry> visited;
  std::vector<queueEntry> queue; 
  //bootstrap the first element into the queue
  queue.push_back({cat, cat, 0, false});

  queueEntry exit = {{INT_MAX, INT_MAX}, {INT_MAX, INT_MAX}, -1, false};

  //while we have elements to be visited, visit them
  while (!queue.empty()) 
  {
    auto head = queue[0];
    queue.erase(queue.begin());

    //mark the head as visited
    visited.push_back(head);

  if (abs(head.position.x) >= (world->getWorldSideSize() / 2) ||
        abs(head.position.y) >= (world->getWorldSideSize() / 2)) {
      // head.isEdge = true;
      exit = head;
      break;
    }        

    //for each neighbor:
    for (auto neigh : World::neighbors(head.position)) {



      // - check if it is not visited yet
      bool isVisited = false; 

      for (int j = 0; j < visited.size(); j++) 
      {
        if (neigh == visited[j].position) 
        {
          isVisited = true;
        }
      }

      // - check if it is not in the queue
      bool inQueue = false;

      for (int j = 0; j < queue.size(); j++) {
        if (neigh == queue[j].position) {
          inQueue = true;
        }
      }

      // - check to make sure it isn't blocked
      bool isBlocked = false;


      if (world->getContent(neigh)) {
        isBlocked = true;
      }

      // - add to queue and add 1 to weight
      if (!isVisited && !inQueue && !isBlocked && world->isValidPosition(neigh))
      {
        //if (head.isEdge) {
            //exit = head;
        //}

        queue.push_back({neigh, head.position, head.weight + 1, false});
        // - mark the neighbor it came from as the current head

      }
    }

  }
    //code win condition and navigation process
    std::vector<Point2D> path; 

    if (exit.weight == -1) {
    //return random movement
      auto pos = world->getCat();
      switch (rand) {
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

    queueEntry e = exit;

    while (e.origin.x != cat.x || e.origin.y != cat.y) {
      for (int i = 0; i < visited.size(); i++) {
        if (visited[i].position == e.origin) {
          //path.push_back(e.position);
          e = visited[i]; 
        }
      }
    }

    // for catcher solution is path[0], for cat it is the last element of path
    return e.position;
}
