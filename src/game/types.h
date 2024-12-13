#include "module.h"

struct HodurInputState : InputState
{
    HodurInputState(int inputCount, KeyInput* inputs)
        : InputState{inputs, inputCount}, Exit(key_states[0]),
          Action(key_states[1]), MoveForward(key_states[2]),
          MoveBackward(key_states[3]), MoveLeft(key_states[4]),
          MoveRight(key_states[5]), TurnLeft(key_states[6]),
          TurnRight(key_states[7]), Log(key_states[8]), Debug(key_states[9]),
          Mixer(key_states[10]), Fire(key_states[11]), Manual(key_states[12]),
          Skip(key_states[13])
    {
    }

    KeyInput& Exit;
    KeyInput& Action;
    KeyInput& MoveForward;
    KeyInput& MoveBackward;
    KeyInput& MoveLeft;
    KeyInput& MoveRight;
    KeyInput& TurnLeft;
    KeyInput& TurnRight;
    KeyInput& Log;
    KeyInput& Debug;
    KeyInput& Mixer;
    KeyInput& Fire;
    KeyInput& Manual;
    KeyInput& Skip;
};

struct LoadedAudio
{
    AudioData TestAudio;

    AudioData* Fx;
    int fx_count;
};
