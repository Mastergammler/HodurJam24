#include "../audio.h"
#include "../bear.h"
#include "../events.h"
#include "../internal.h"
#include "../loading.h"
#include "../map.h"
#include "../player.h"
#include "../systems.h"

const float STEP_PAN = 0.2;
const float SEC_STEP_VOL = 0.42;
const float STEP_BACK_LP_CO = 0.35;
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
            playback.voice = &Player.left_foot;
        }
        else if ((isFirst && direction == EAST) ||
                 (isFirst && direction == SOUTH))
        {
            playback.voice = &Player.right_foot;
            playback.pan = STEP_PAN;
        }
        else if (isFirst)
        {
            playback.pan = -STEP_PAN;
            playback.voice = &Player.left_foot;
        }
        else
        {
            playback.voice = &Player.right_foot;
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

void PlayTileAudio(TileType type,
                   v2 direction,
                   float volume = 1,
                   float delay = 0)
{
    if (Audio.fx_mapping.find(type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = DirectionalAudio(direction);
        playback.voice = &Player.body;
        playback.volume = volume;
        SchedulePlayback(audio, playback, delay);
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
                // NOTE: did decide not to move the bear for now
                //-> this would lead to sound overlap & also doesn't feel good
                //-> loosing 2 steps immediately is quite harsh especially
                // because you mostly need to run into the chest to open it
            }
        }
        break;

        case DOOR:
        {
            if (Level.player_has_key)
            {
                Event_LevelVictory();
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

void ExecuteInteraction()
{
    v2 lookPosition = Player.position + Player.orientation;
    Tile forwardTile = TileAt(lookPosition);

    if (forwardTile.is_interactable)
    {
        ExecuteAction(forwardTile.type);
    }
}

// TODO: this is now more a handle inputs?
void HandleActions()
{
    if (GameInputs.Action.pressed)
    {
        ExecuteInteraction();
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
            float delay = 0;
            if (nextTile.type == GRASS) delay = 0.125;

            // TODO: delay for grass sounds, because else it feels strange
            PlayTileAudio(POCKET, INIT, 0.75, delay);
        }
        Bear_MoveTowardsPlayer();
    }
    else
    {
        Player.orientation = direction;
        if (nextTile.type == DOOR && Level.player_has_key)
        {
            ExecuteInteraction();
        }
        else
        {
            PlayTileAudio(nextTile.type, direction);
            if (Audio.fx_mapping.find(nextTile.type) != Audio.fx_mapping.end())
            {
                // If it makes a sound -> move bear
                Bear_MoveTowardsPlayer();
            }
        }
    }
}
