#include "../module.h"

struct UiState
{
    bool is_active;

    int current_level;
    int level_count;

    string* level_names;
};
