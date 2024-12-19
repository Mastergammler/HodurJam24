#include "../internal.h"
#include "../map.h"
#include <corecrt_math_defines.h>
#include <winuser.h>

/**
 * O(n = count)
 */
NodeItem* NodeItemForPosition(v2 position)
{
    for (int i = 0; i < Nodes.count; i++)
    {
        NodeItem* cur = &Nodes.items[i];
        if (cur->position == position) return cur;
    }

    // TODO: a bit akward to set this here, and then immediately close it again
    NodeItem* newNode = &Nodes.items[Nodes.count++];
    newNode->is_closed = false;
    return newNode;
}

/*
 * O(n = count)
 */
void EvaluatePosition(v2 direction, NodeItem prevNode, v2 target)
{
    v2 localStart = prevNode.position;
    v2 nextPosition = localStart + direction;
    Tile tile = TileAt(nextPosition);

    if (tile.is_walkable)
    {
        // PERF: this is crazy, this increases alg runtime (for 7x10 map) from
        // 0.005 ms to around 8 ms each, crazy, despite only beeing 1 LOG per
        // position ...
        // Maybe my thread logging is not quite working yet???

        /*Logf("Evaluating walkable node: %i (%i,%i)",
             Nodes.count,
             nextPosition.x,
             nextPosition.y);*/
        assert(Nodes.count < Nodes.max_size);

        NodeItem* forEval = NodeItemForPosition(nextPosition);
        if (!forEval->is_closed)
        {
            forEval->is_closed = false;
            forEval->position = nextPosition;
            forEval->g_value = walk_distance(localStart, nextPosition) +
                               prevNode.g_value;
            forEval->h_value = walk_distance(nextPosition, target);
            forEval->f_value = forEval->g_value + forEval->h_value;
        }
    }
}

/**
 * O(n) / n = count
 */
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
        smallestItem->is_closed = true;
    }
    return smallestItem;
}

/**
 * Finds the next neighbour path tile
 * PERF: wouldn't it be more optimal to sort, for closed nodes & also
 * sort those by smallest f and g or something? Wouldn't this work?
 * but this would be O(n log n) -> so this is more inefficient?
 *
 * O(n) / n = count
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

/**
 * A* algorithm for finding the next
 * tile to move to
 *
 * TC: O(2 * Nodes.count * n)
 * Best: n ~ path length * 3
 * Worst: n = total map tiles
 * => Still a linear time algorithm, so pretty good
 */
v2 DetermineNextPosition(v2 startPosition, v2 targetPosition)
{
    // g value on the starting tile is 0, because we are there already
    int gValue = 0;
    int distance = walk_distance(startPosition, targetPosition);

    // NOTE:
    // this handling is a bit akward, usually the start node would be the first
    //  to put into the alg, and then be evaluated and closed
    //  but i did it a bit differently, since i reference via tha previous node
    //  since i just add it as closed node here
    Nodes.count = 1;
    Nodes.items[0] = {true, startPosition, gValue, distance, distance};
    NodeItem prevNode = Nodes.items[0];

    bool noPathPossible = false;

    // n * 5 * listSize  (n = tiles to evaluate / list size / count)
    while (prevNode.position != targetPosition)
    {
        // 4x listSize
        EvaluatePosition(NORTH, prevNode, targetPosition);
        EvaluatePosition(EAST, prevNode, targetPosition);
        EvaluatePosition(SOUTH, prevNode, targetPosition);
        EvaluatePosition(WEST, prevNode, targetPosition);

        // 1x listSize
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

    // p * listSize (p = path length)
    NodeItem* nextNode = &prevNode;
    while (nextNode && nextNode->g_value > 1)
    {
        nextNode = NextPathNode(nextNode);
    }

    // TODO: proper proximity handling
    if (nextNode->h_value == 1)
    {
        Log("Bear proximity aleart!!!");
    }

    v2 nextMove = nextNode->position - startPosition;
    return nextMove;
}

void PlayFootstepAudio(TileType type,
                       float delay,
                       float volume = 1,
                       float pan = 0)
{
    if (Audio.bear_fx_mapping.find(type) != Audio.bear_fx_mapping.end())
    {
        FxInfo fxInfo = Audio.bear_fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {&Bear.body};
        playback.volume = volume;
        playback.pan = pan;

        SchedulePlayback(audio, playback, delay);
    }
}

float GetPan(v2 playerPosition, v2 bearPosition)
{
    // NOTE: Due to the top down perspective and the notion that NORTH is
    // upwards
    //  we have to switch left and right, because the player is in fact turned
    //  180 Deg relative to the map orientation (where y + 1 = downward)
    v2 distance = {bearPosition.x - playerPosition.x,
                   playerPosition.y - bearPosition.y};

    double angleRad = atan2(distance.y, distance.x) - M_PI_2;
    float pan = angleRad * 2 / M_PI;

    Logf("Pan for (%i,%i) :  %.2f - %.2f",
         distance.x,
         distance.y,
         angleRad,
         pan);

    return pan;
}

void Bear_MoveTowardsPlayer()
{
    Clock pathClock = {};
    Measure_Start(pathClock);
    v2 direction = DetermineNextPosition(Bear.position, Player.position);
    float elapsed = Measure_Elapsed(pathClock);

    Bear.position = Bear.position + direction;
    Logf("Next bear position is (%i,%i) - %i nodes -  %.3f ms",
         Bear.position.x,
         Bear.position.y,
         Nodes.count,
         elapsed);

    if (Bear.position == Player.position)
    {
        Player.inputs_locked = true;
        PlaybackSettings playback = {&GlobalStereo};
        playback.volume = 1.5f;
        PlayAudio(&Audio.FailSound, playback);
        PlaybackSettings p2 = {&Ambience};
        p2.volume = 2.f;
        SchedulePlayback(&Audio.BearGrowl, p2, 3.25);
    }
    else
    {
        Tile newBearPosition = TileAt(Bear.position);

        // TODO: these are tweaking values, these should live in a config file
        //  -> because then i also could HOT RELOAD them !
        float afterPlayerDelay = 1;
        float stepBaseDelay = 0.35;

        float decimal1 = stepBaseDelay + ((rand() % 8) - 4) / 100.;
        float decimal2 = stepBaseDelay + ((rand() % 10) - 5) / 100.;
        float decimal3 = stepBaseDelay + ((rand() % 12) - 6) / 100.;

        float pan = GetPan(Player.position, Bear.position);

        PlayFootstepAudio(newBearPosition.type, afterPlayerDelay, 1, pan);
        PlayFootstepAudio(newBearPosition.type,
                          afterPlayerDelay + decimal1,
                          1,
                          pan);
        PlayFootstepAudio(newBearPosition.type,
                          afterPlayerDelay + decimal1 + decimal2,
                          0.85,
                          pan);
        PlayFootstepAudio(newBearPosition.type,
                          afterPlayerDelay + decimal1 + decimal2 + decimal3,
                          0.75,
                          pan);
    }
}
