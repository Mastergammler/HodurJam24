#include "../events.h"
#include "../internal.h"
#include "../map.h"
#include "../systems.h"

// TODO: these are tweaking values, these should live in a config file
//  -> because then i also could HOT RELOAD them !
const float BREATHING_BASE_VOLUME = 1.1;
const float BREATHING_FALLOFF_VALUE = 1.5;
const float AFTER_PLAYER_DELAY = 1;
const float FOOTSTEP_BASE_DELAY = 0.35;
const float FOOTSTEP_VOLUME_FACTOR = 1.5;

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
             cur->g_value < smallestItem->g_value))
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

    // we want to know the position, the bear has after the current move!
    if (noPathPossible)
    {
        Logf("ERROR: No path found from (%i,%i) towards (%i,%i)",
             startPosition.x,
             startPosition.y,
             targetPosition.x,
             targetPosition.y);
        return INIT;
    }

    NodeItem* nextNode = &prevNode;
    while (nextNode && nextNode->g_value > 1)
    {
        nextNode = NextPathNode(nextNode);
    }

    v2 nextMove = nextNode->position - startPosition;
    Bear.distance_in_steps = prevNode.g_value - 1;

    return nextMove;
}

void PlayTileAudio(TileType type, float delay, PlaybackSettings settings)
{
    if (Audio.bear_fx_mapping.find(type) != Audio.bear_fx_mapping.end())
    {
        FxInfo fxInfo = Audio.bear_fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        SchedulePlayback(audio, settings, delay);
    }
}

float GetVolumeByDistance(v2 playerPosition,
                          v2 bearPosition,
                          float startingVolume = 1,
                          float falloffFactor = 0.75)
{
    v2 distanceVector = {bearPosition.x - playerPosition.x,
                         playerPosition.y - bearPosition.y};

    float distance = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));

    // max volume
    if (distance <= 0) return startingVolume;

    float newVolume = startingVolume / pow(distance, falloffFactor);

    /*Logf("Volume is %.2f for distance %.2f (starting at %.2f)",
         newVolume,
         distance,
         startingVolume);*/

    return newVolume;
}

float GetPan(v2 playerPosition, v2 bearPosition)
{
    if (playerPosition == bearPosition) return 0;

    // NOTE: Due to the top down perspective and the notion that NORTH is
    // upwards, the problem is, that world space and screen space are not the
    // same or rother the world space from the map, is different from the world
    // space of the player
    //
    //  we have to switch left and right, because the player is in fact turned
    //  180 Deg relative to the map orientation (where y + 1 = downward)
    v2 distance = {bearPosition.x - playerPosition.x,
                   playerPosition.y - bearPosition.y};

    // we're only looking at q1, but also rotate the coordinate system by 90 deg
    // (by flipping x and y)
    // NOTE: I thought it should have been x / y for 90 degree rotation,
    // but apparently it still has to be y/x, not sure why
    // but this sounds correct -> the thing that flips everything, is just the
    // angle calculation
    double alphaTrans = atan2(abs(distance.y), abs(distance.x));
    // since we rotated by 90, we need to calculate
    // the complementary angle instead
    double alpha = M_PI_2 - alphaTrans;

    // since we used the abs of the values (for only q1)
    // we now need to actually hanlde left and right
    if (distance.x < 0) alpha = -alpha;

    float normalizedPan = alpha * 2 / M_PI;

    /*Logf("Pan for (%i,%i) :  %.2f - %.2f",
         distance.x,
         distance.y,
         alpha,
         normalizedPan);*/

    return normalizedPan;
}

double degreeToRadians(float angle)
{
    return angle * M_PI / 180;
}

double radiansToDegree(double radians)
{
    return radians * 180 / M_PI;
}

float GetLowpass(v2 playerPosition, v2 bearPosition)
{
    v2 distance = {bearPosition.x - playerPosition.x,
                   playerPosition.y - bearPosition.y};

    double alpha = atan2(abs(distance.y), abs(distance.x));
    // double alpha = M_PI_2 - alphaTrans;

    double alphaDegrees = radiansToDegree(alpha);

    if (alphaDegrees >= Player.behind_cone_max_angle && distance.y < 0)
    {
        return Player.lp_max_ratio;
    }
    else if (alphaDegrees >= Player.behind_cone_angle && distance.y < 0)
    {
        // adding more of the low pass the closer to the back a sound is
        float angleRange = Player.behind_cone_max_angle -
                           Player.behind_cone_angle;

        float addRange = alphaDegrees - Player.behind_cone_angle;
        float toMaxPercentage = addRange / angleRange;
        // TODO: whats high and low is flipped, it's confusing
        float relativeRatio = Player.lp_start_ratio - Player.lp_max_ratio;

        float ratioAdd = toMaxPercentage * relativeRatio;
        float lpValue = Player.lp_start_ratio - ratioAdd;

        if (lpValue < Player.lp_max_ratio)
        {
            Logf("Ratio is actually too low: %.2f", lpValue);
        }
        // - preventing audio errors just in case
        if (lpValue < 0)
        {
            Logf("Fatal lp calculation error: value was below 0: %.2f",
                 lpValue);
            lpValue = 0;
        }
        return lpValue;
    }

    // TODO: make this clear what is lp on and off etc ...
    // -> LP off should be value 0! -> zero as initalization
    // default value, 0 lowpass
    return 1;
}

// TODO: refactor, this now does multiple things ....
// and the params are only for the breathing ...
// -> This is super akward
void SetProximityLevels(float pan, float lpRatio, float volume)
{
    // TODO: it looks like the proximity handling is something different
    //  -> should maybe live somewhere else
    //  Logf("Bear distance in steps: %i", Bear.distance_in_steps);
    PlaybackSettings codeYellow = {&ProximityYellow, true, true};
    PlaybackSettings codeRed = {&ProximityRed, true, true};
    PlaybackSettings breathing = {&Bear.breathing};
    breathing.pan = pan;
    breathing.lowpass_filter = lpRatio;
    breathing.volume = volume;

    v2 distance = Player.position - Bear.position;
    v2 delta = {abs(distance.x), abs(distance.y)};

    bool isDirectNeighbour = false;
    bool isNextToDirect = false;

    if (delta <= 1)
        isDirectNeighbour = true;
    else if (delta.x == 2 || delta.y == 2)
        isNextToDirect = true;

    // does not include corners 2/2 etc, because these are distance 4
    // but all other 2s basically
    if (isNextToDirect &&
        (Bear.distance_in_steps == 2 || Bear.distance_in_steps == 3))
    {
        codeYellow.volume = 0.55;
        codeRed.volume = 0;
    }
    // includes the corners of 1/1 etc
    else if (isDirectNeighbour &&
             (Bear.distance_in_steps == 1 || Bear.distance_in_steps == 2))
    {
        codeYellow.volume = 0.55;
        codeRed.volume = 0.55;
    }
    else
    {
        codeYellow.volume = 0;
        codeRed.volume = 0;
    }
    UpdateCurrentPlayback(codeYellow);
    UpdateCurrentPlayback(codeRed);
    UpdateCurrentPlayback(breathing);
}

void Debug_LogClosedNodes(v2 direction)
{
    // TODO: IF DEBUG
    if (direction == INIT)
    {
        // this should never happen, unless the bear is trapped somewhere
        Log("Warning: Bear is not moving ...");

        for (int i = 0; i < Nodes.count; i++)
        {
            NodeItem* cur = &Nodes.items[i];
            if (cur->is_closed)
            {
                Logf("C %i %i - %i + %i = %i",
                     cur->position.x,
                     cur->position.y,
                     cur->g_value,
                     cur->h_value,
                     cur->f_value);
            }
        }
    }
}

/*
 * The 'moveBear' gives possibility of doing a dry run, for setting the audio
 * levels at startup
 */
void Bear_MoveTowardsPlayer(bool moveBear = true)
{
    if (!Bear.is_present) return;

    Clock pathClock = {};
    Measure_Start(pathClock);
    v2 direction = DetermineNextPosition(Bear.position, Player.position);
    float elapsed = Measure_Elapsed(pathClock);

    TileType playerTile = TileAt(Player.position).type;
    bool bearIsMoving = direction != INIT && playerTile != GRASS && moveBear;

    Debug_LogClosedNodes(direction);

    if (bearIsMoving)
    {
        Bear.position = Bear.position + direction;
        Logf("New bear position is (%i,%i), moving towards (%i,%i) - %i nodes "
             "-  %.3f ms",
             Bear.position.x,
             Bear.position.y,
             Player.position.x,
             Player.position.y,
             Nodes.count,
             elapsed);
    }
    else
    {
        // bear didn't actually move, but the value is set by the alg,
        // the the algorithm determines distance based on the just evaluated
        // position
        Bear.distance_in_steps += 1;
    }

    float pan = GetPan(Player.position, Bear.position);
    float lpRatio = GetLowpass(Player.position, Bear.position);
    float volume = GetVolumeByDistance(Player.position,
                                       Bear.position,
                                       BREATHING_BASE_VOLUME,
                                       BREATHING_FALLOFF_VALUE);
    SetProximityLevels(pan, lpRatio, volume);

    float walkingDelay = 0;
    if (bearIsMoving)
    {
        Tile newBearPosition = TileAt(Bear.position);

        float foot2 = FOOTSTEP_BASE_DELAY + ((rand() % 8) - 4) / 100.;
        float foot3 = FOOTSTEP_BASE_DELAY + ((rand() % 10) - 5) / 100.;
        float foot4 = FOOTSTEP_BASE_DELAY + ((rand() % 12) - 6) / 100.;

        float f1Delay = AFTER_PLAYER_DELAY;
        float f2Delay = f1Delay + foot2;
        float f3Delay = f2Delay + foot3;
        float f4Delay = f3Delay + foot4;

        walkingDelay = f4Delay + FOOTSTEP_BASE_DELAY;

        PlaybackSettings s1 = {&Bear.body};
        s1.pan = pan;
        s1.lowpass_filter = lpRatio;
        s1.volume = GetVolumeByDistance(Player.position,
                                        Bear.position,
                                        1 * FOOTSTEP_VOLUME_FACTOR);

        PlaybackSettings s2 = {&Bear.body};
        s2.pan = pan;
        s2.lowpass_filter = lpRatio;
        s2.volume = GetVolumeByDistance(Player.position,
                                        Bear.position,
                                        1 * FOOTSTEP_VOLUME_FACTOR);
        PlaybackSettings s3 = {&Bear.body};
        s3.pan = pan;
        s3.lowpass_filter = lpRatio;
        s3.volume = GetVolumeByDistance(Player.position,
                                        Bear.position,
                                        0.85 * FOOTSTEP_VOLUME_FACTOR);
        PlaybackSettings s4 = {&Bear.body};
        s4.pan = pan;
        s4.lowpass_filter = lpRatio;
        s4.volume = GetVolumeByDistance(Player.position,
                                        Bear.position,
                                        0.75 * FOOTSTEP_VOLUME_FACTOR);

        PlaybackSettings foley = {&Bear.foley};
        foley.pan = pan;
        foley.lowpass_filter = lpRatio;
        foley.volume = GetVolumeByDistance(Player.position,
                                           Bear.position,
                                           1.5 * FOOTSTEP_VOLUME_FACTOR);

        PlayTileAudio(newBearPosition.type, f1Delay, s1);
        PlayTileAudio(newBearPosition.type, f2Delay, s2);
        PlayTileAudio(newBearPosition.type, f3Delay, s3);
        PlayTileAudio(newBearPosition.type, f4Delay, s4);
        // not playing those, because they are inaudable anyway when the bear is
        // close enough beacause of the proximity sounds
        /*PlayTileAudio(FOLEY, f1Delay, foley);
        PlayTileAudio(FOLEY, f2Delay, foley);
        PlayTileAudio(FOLEY, f3Delay, foley);
        PlayTileAudio(FOLEY, f4Delay, foley);*/
    }

    if (Bear.position == Player.position)
    {
        Player.inputs_locked = true;
        ScheduleExecution(walkingDelay, Event_LevelFailure);
    }
}
