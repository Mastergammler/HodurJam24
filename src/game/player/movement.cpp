#include "../bear.h"
#include "../internal.h"
#include "../loading.h"
#include "../map.h"
#include "../player.h"
#include "../systems.h"

const float STEP_PAN = 0.2;
const float SEC_STEP_VOL = 0.42;
const float STEP_BACK_LP_CO = 0.35;
const float BACK_LP_CO = 0.45;
const float BUMP_PAN = 0.65;

const float STEP_ANIM_TIME = .45f;

v2 GetMovementDirection()
{
    if (GameInputs.MoveForward.pressed) return NORTH;
    if (GameInputs.MoveBackward.pressed) return SOUTH;
    if (GameInputs.MoveLeft.pressed) return WEST;
    if (GameInputs.MoveRight.pressed) return EAST;
    return INIT;
}

void PlayFootstepAudio(TileType type, bool isFirst, v2 direction)
{
    if (Audio.fx_mapping.find(type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {};

        // TODO: also pretty ugly
        if (isFirst && direction == NORTH)
        {
            playback.pan = -STEP_PAN;
            playback.settings = &Player.left_foot;
        }
        else if ((isFirst && direction == EAST) ||
                 (isFirst && direction == SOUTH))
        {
            playback.settings = &Player.right_foot;
            playback.pan = STEP_PAN;
        }
        else if (isFirst)
        {
            playback.pan = -STEP_PAN;
            playback.settings = &Player.left_foot;
        }
        else
        {
            playback.settings = &Player.right_foot;
            playback.pan = STEP_PAN;
        }

        if (!isFirst)
        {
            playback.volume = SEC_STEP_VOL;
        }
        if (direction == SOUTH)
        {
            playback.lowpass_filter = STEP_BACK_LP_CO;
        }

        PlayAudio(audio, playback);
    }
}

void PlayTileAudio(TileType type, v2 direction, float volume = 1)
{
    if (Audio.fx_mapping.find(type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {};
        playback.settings = &Player.body;
        playback.volume = volume;

        if (direction == WEST)
        {
            playback.pan = -BUMP_PAN;
        }
        else if (direction == EAST)
        {
            playback.pan = BUMP_PAN;
        }
        else if (direction == SOUTH)
        {
            playback.lowpass_filter = BACK_LP_CO;
        }
        PlayAudio(audio, playback);
    }
}

// TODO: not sure if this is the best solutions it's quit of akward
//  also if the player continues immediately after stopping then i have overlap
//  that's not great
void HandleFootstepAnimation()
{
    Player.time_since_anim_start += Timer.sim_time;

    if (Player.time_since_anim_start >= STEP_ANIM_TIME)
    {
        Player.time_since_anim_start = 0;
        Player.in_walk_anim = false;
        if (Level.current_tile.is_walkable)
        {
            PlayFootstepAudio(Level.current_tile.type,
                              false,
                              Player.orientation);
        }
    }
}

void ExecuteAction(TileType interactionType)
{
    switch (interactionType)
    {
        case CHEST:
        {
            if (Level.player_has_key)
            {
                PlayTileAudio(HUH, INIT);
            }
            else
            {
                Level.player_has_key = true;
                PlayAudio(&Audio.OpenChest, {&GlobalStereo});
                PlayAudio(&Audio.ObtainKeys, {&GlobalStereo, false});
            }
        }
        break;

        case DOOR:
        {
            if (Level.player_has_key)
            {
                Player.inputs_locked = true;
                PlayAudio(&Audio.UnlockDoor, {&GlobalStereo});
                PlayAudio(&Audio.SuccessSound, {&GlobalStereo, false});
                ScheduleExecution(10, LoadNextLevel);
            }
            else
            {
                PlayTileAudio(HUH, INIT);
            }
        }
        break;
        default: assert("Missing enum definition!");
    }
}

void HandleActions()
{
    if (GameInputs.Action.pressed)
    {
        v2 lookPosition = Player.position + Player.orientation;
        Tile forwardTile = TileAt(lookPosition);

        // TODO: actually i need to check the objects state
        //  -> becaues it's something specific most of the time
        if (forwardTile.is_interactable)
        {
            ExecuteAction(forwardTile.type);
        }
    }
}

void HandleMovement()
{
    v2 direction = GetMovementDirection();
    if (Player.in_walk_anim)
    {
        HandleFootstepAnimation();

        // giving back control next frame
        return;
    }

    if (direction == INIT) return;

    v2 nextPosition = Player.position + direction;
    Tile nextTile = TileAt(nextPosition);

    if (nextTile.is_walkable)
    {
        Player.orientation = direction;
        Player.position = nextPosition;
        Player.in_walk_anim = true;
        Level.current_tile = nextTile;

        PlayFootstepAudio(nextTile.type, true, direction);
        if (Level.player_has_key)
        {
            // TODO: delay for grass sounds, because else it feels strange
            PlayTileAudio(POCKET, INIT, 0.75);
        }

        Bear_MoveTowardsPlayer();
    }
    else
    {
        Player.orientation = direction;
        PlayTileAudio(nextTile.type, direction);
    }
}
