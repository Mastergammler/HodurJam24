#include "../types.h"
#include "../vector.h"

struct Bear
{
    bool is_present;

    v2 position;
    VoiceSettings body;

    /**
     * Distance in steps from the player
     * The amount of steps the bear needs to make in ordder to reach the player
     */
    int distance_in_steps;
};

struct NodeItem
{
    bool is_closed;

    v2 position;

    /**
     * walk value from start to this node
     */
    int g_value;

    /**
     * heuristic value - distance to target
     */
    int h_value;

    /**
     * h + g value
     */
    int f_value;
};

struct NodeList
{
    NodeItem* items;
    int max_size;
    int count;
};
