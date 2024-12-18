#include "../internal.h"
#include "../map.h"
#include <rpcndr.h>

void EvaluatePosition(v2 direction, NodeItem prevNode, v2 target)
{
    // initial open nodes
    NodeItem* node = &Nodes.items[Nodes.count];
    v2 localStart = prevNode.position;
    v2 nextPosition = localStart + direction;

    Tile tile = TileAt(nextPosition);

    if (tile.is_walkable)
    {
        assert(Nodes.count < Nodes.max_size);

        Nodes.count++;

        node->is_closed = false;
        node->position = nextPosition;
        node->g_value = walk_distance(localStart, nextPosition) +
                        prevNode.g_value;
        node->h_value = walk_distance(nextPosition, target);
        node->f_value = node->g_value + node->h_value;
    }
}

NodeItem* SelectNextNode()
{
    NodeItem* smallestItem = NULL;

    for (int i = 0; i < Nodes.count; i++)
    {
        NodeItem* cur = &Nodes.items[i];
        if (cur->is_closed) continue;

        if (!smallestItem || (cur->f_value < smallestItem->f_value) ||
            (cur->f_value == smallestItem->f_value &&
             cur->h_value < smallestItem->h_value))
        {
            smallestItem = cur;
        }
    }

    if (smallestItem)
    {
        // we're closing this, because we're evaluating this next
        smallestItem->is_closed = true;
    }
    return smallestItem;
}

// TODO: handle node duplicates / collisions
/**
 * A* algorithm for finding the next tile to move to
 */
v2 DetermineNextPosition(v2 startPosition, v2 targetPosition)
{
    Nodes.count = 0;

    // g value on the starting tile is 0, because we are there already
    NodeItem prevNode = {true, startPosition, 0};
    bool noPathPossible = false;

    while (prevNode.position != targetPosition)
    {
        EvaluatePosition(NORTH, prevNode, targetPosition);
        EvaluatePosition(EAST, prevNode, targetPosition);
        EvaluatePosition(SOUTH, prevNode, targetPosition);
        EvaluatePosition(WEST, prevNode, targetPosition);

        NodeItem* next = SelectNextNode();
        if (next == NULL)
        {
            // All nodes are closed, but we're not at target yet
            // -> There is no path
            noPathPossible = true;
            break;
        }

        prevNode = *next;
    }

    // go through closed nodes
    // -> lowest f value, from target towards start
    // -> From neighbour to neighbour
}

void MoveTowardsPlayer()
{
    Clock pathClock = {};
    Measure_Start(pathClock);
    v2 direction = DetermineNextPosition(Bear.position, Player.position);
    float elapsed = Measure_Elapsed(pathClock);

    Bear.position = Bear.position + direction;
    Logf("Next bear position is (%i,%i) took .2f",
         Bear.position.x,
         Bear.position.y,
         elapsed);
}
