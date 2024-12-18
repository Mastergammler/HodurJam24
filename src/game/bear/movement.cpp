#include "../internal.h"
#include "../map.h"

void EvaluatePosition(v2 direction, NodeItem prevNode, v2 target)
{
    // initial open nodes
    NodeItem* forEval = &Nodes.items[Nodes.count];
    v2 localStart = prevNode.position;
    v2 nextPosition = localStart + direction;

    Tile tile = TileAt(nextPosition);

    if (tile.is_walkable)
    {
        assert(Nodes.count < Nodes.max_size);

        Nodes.count++;

        forEval->is_closed = false;
        forEval->position = nextPosition;
        forEval->g_value = walk_distance(localStart, nextPosition) +
                           prevNode.g_value;
        forEval->h_value = walk_distance(nextPosition, target);
        forEval->f_value = forEval->g_value + forEval->h_value;
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
        // unless it's the destination, then we don't need to
        // but it therefore is irrelevant for the algorithm?
        smallestItem->is_closed = true;
    }
    return smallestItem;
}

/**
 * Finds the next neighbour path tile
 * TODO: wouldn't it be more optimal to sort, for closed nodes & also
 * sort those by smallest f and g or something? Wouldn't this work?
 */
NodeItem* NextPathNode(NodeItem* neighbour)
{
    NodeItem* next = NULL;

    for (int i = 0; i < Nodes.count; i++)
    {
        NodeItem cur = Nodes.items[i];
        if (!cur.is_closed) continue;

        int distance = walk_distance(neighbour->position, cur.position);
        if (distance == 1)
        {
            if (!next || next->f_value > cur.f_value)
            {
                next = &Nodes.items[i];
            }
        }
    }

    // there must be a next node, else the algorithm did fail
    // if there is no path, this function should not be called
    assert(next);
    return next;
}

// TODO: handle node duplicates /
// collisions
/**
 * A* algorithm for finding the next
 * tile to move to
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

    if (noPathPossible) return INIT;

    NodeItem* nextNode = &prevNode;
    while (nextNode->g_value > 1)
    {
        nextNode = NextPathNode(nextNode);
    }

    if (nextNode->h_value == 1)
    {
        Log("Bear proximity aleart!!!");
    }

    v2 nextMove = nextNode->position - startPosition;
    return nextMove;
}

void PlayFootstepAudio(TileType type, float delay)
{
    if (Audio.bear_fx_mapping.find(type) != Audio.bear_fx_mapping.end())
    {
        FxInfo fxInfo = Audio.bear_fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {&Bear.body};
        SchedulePlayback(audio, playback, delay);
    }
}

void Bear_MoveTowardsPlayer()
{
    Clock pathClock = {};
    Measure_Start(pathClock);
    v2 direction = DetermineNextPosition(Bear.position, Player.position);
    float elapsed = Measure_Elapsed(pathClock);

    Bear.position = Bear.position + direction;
    Logf("Next bear position is (%i,%i) took %.2f",
         Bear.position.x,
         Bear.position.y,
         elapsed);

    if (Bear.position == Player.position)
    {
        Player.inputs_locked = true;
        PlaybackSettings playback = {&GlobalStereo};
        playback.volume = 1.5f;
        PlayAudio(&Audio.FailSound, playback);
    }
    else
    {
        Tile bearTile = TileAt(Bear.position);
        PlayFootstepAudio(bearTile.type, 1);
        PlayFootstepAudio(bearTile.type, 1.2);
        PlayFootstepAudio(bearTile.type, 1.5);
        PlayFootstepAudio(bearTile.type, 1.7);
    }
}
