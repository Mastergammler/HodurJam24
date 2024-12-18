#include "../types.h"
#include "../vector.h"

struct Bear
{
    bool is_present;

    v2 position;

    VoiceSettings body;

    // TODO: do i need an orientation?
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
