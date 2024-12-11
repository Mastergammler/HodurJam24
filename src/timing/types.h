#include "imports.h"

struct Clock
{
    int fps;
    int fps_min = 10000;
    int fps_max = 0;

    /**
      Time since the last update call
     * When update is called every frame = frame time
     * Delta time is in seconds?
     * This is the actual passed time, so all IO system based things
     * should use this time
     *
     * Unit s
     */
    float delta_time_real;

    float time_scale = 1;

    /**
     * this is the scaled delta time (simulation time)
     * it depends on time scale
     * So all entities and game systems should use this time
     *
     * Unit: s
     */
    float sim_time;

    float delta_min = 10000;
    float delta_max = 0;

    /**
     * Limiter for updating the fps text
     * Too high a frequency is hard to read
     */
    float fps_update_threshold = 0.25;
    float time_counter;
    LARGE_INTEGER frequency;
    LARGE_INTEGER last_time;
    LARGE_INTEGER start_time;
};
