#include "Cat.h"
#include "World.h"

Point2D Cat::Move(World* world) {
  ///
  /// gets the cat object from the world class
  ///

  auto cat = world->getCat();

  ///
  /// creating two queues: one for the points that have already been visited by
  /// the algorithm and one for points that need to be checked
  ///

  std::vector<queueEntry> visited;
  std::vector<queueEntry> queue;

  ///
  /// add the cat's starting point into the queue
  ///

  queue.push_back({cat, cat, 0});

  ///
  /// setting up a dummy variable so that the queue can mark an exit point
  ///

  queueEntry exit = {{INT_MAX, INT_MAX}, {INT_MAX, INT_MAX}, -1};

  ///
  /// this loop finds the path with the least amount of steps to the exit
  ///

  while (!queue.empty()) {
    ///
    /// create a variable marking the location of the current point that
    /// the algorithm is checking, then erase it so that the next time the loop
    /// is run the next point is used as the starting point, then add the
    /// current point to the queue of visited points
    ///

    auto head = queue[0];
    queue.erase(queue.begin());
    visited.push_back(head);

    ///
    /// checking to see if the next point is out of range, and marking the
    /// current point as an exit if it is
    ///

    if (abs(head.position.x) >= (world->getWorldSideSize() / 2) ||
        abs(head.position.y) >= (world->getWorldSideSize() / 2)) {
      exit = head;
      break;
    }

    ///
    /// for each neighboring point to the current point:
    ///

    for (auto neigh : World::neighbors(head.position)) {
      ///
      /// check if the algorithm has visited it
      ///

      bool isVisited = false;

      for (int j = 0; j < visited.size(); j++) {
        if (neigh == visited[j].position) {
          isVisited = true;
        }
      }

      ///
      /// check if it is in the queue
      ///

      bool inQueue = false;

      for (int j = 0; j < queue.size(); j++) {
        if (neigh == queue[j].position) {
          inQueue = true;
        }
      }

      ///
      /// check to make sure it isn't blocked
      ///

      bool isBlocked = false;

      if (world->getContent(neigh)) {
        isBlocked = true;
      }

      ///
      /// under the correct conditions, add the point to queue, add 1 to
      /// 'weight' (weight counts how many spaces are in the current queue), and
      /// mark the neighbor it came from as the current head
      ///

      if (!isVisited && !inQueue && !isBlocked &&
          world->isValidPosition(neigh)) {
        queue.push_back({neigh, head.position, head.weight + 1});
      }
    }
  }

  ///
  /// if there is no exit, perform a random movement
  ///

  if (exit.weight == -1) {
    auto rand = Random::Range(0, 5);
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

  ///
  /// creating a dummy variable so that exit does not get changed unnecesarily
  ///

  queueEntry e = exit;

  ///
  /// cycles backwards through the visited points of the final queue to find the
  /// next point the cat should move to
  ///

  while (e.origin.x != cat.x || e.origin.y != cat.y) {
    for (int i = 0; i < visited.size(); i++) {
      if (visited[i].position == e.origin) {
        e = visited[i];
      }
    }
  }

  return e.position;
}
