#include "../internal.h"
#include "../map.h"

static Tile CurrentTile = {WALL, {-1, -1}, false};

v2 GetMovementDirection()
{
    if (GameInputs.MoveForward.pressed)
    {
        return {0, -1};
    }
    if (GameInputs.MoveBackward.pressed)
    {
        return {0, 1};
    }
    if (GameInputs.MoveLeft.pressed)
    {
        return {-1, 0};
    }
    if (GameInputs.MoveRight.pressed)
    {
        return {1, 0};
    }
    return {0, 0};
}

void PlayTileAudio(TileType type, bool isFirst, v2 direction)
{
    if (Audio.fx_mapping.find(type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.Fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {};

        // TODO: backwards flip also?
        // -> also pretty ugly
        if (isFirst && direction == v2{-1, 0})
        {
            playback.pan = -0.20;
            playback.settings = &Player.left_foot;
        }
        else if (isFirst && direction == v2{1, 0} ||
                 isFirst && direction == v2{0, 1})
        {
            playback.settings = &Player.right_foot;
            playback.pan = 0.20;
        }
        else if (isFirst)
        {
            playback.pan = -0.20;
            playback.settings = &Player.left_foot;
        }
        else
        {
            playback.settings = &Player.right_foot;
            playback.pan = 0.20;
        }

        if (!isFirst)
        {
            playback.volume = 0.42;
        }
        if (direction == v2{0, 1})
        {
            playback.lowpass_filter = 0.35;
        }

        PlayAudioNow(audio, playback);
    }
}

void PlayTileAudio(TileType type, v2 direction)
{
    if (Audio.fx_mapping.find(type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[type];
        int idxOffset = rand() % fxInfo.count;
        AudioData* audio = &Audio.Fx[fxInfo.start_idx + idxOffset];

        PlaybackSettings playback = {};
        playback.settings = &Player.body;

        if (direction == v2{-1, 0})
        {
            playback.pan = -.65;
        }
        else if (direction == v2{1, 0})
        {
            playback.pan = .65;
        }
        else if (direction == v2{0, 1})
        {
            playback.lowpass_filter = 0.45;
        }
        PlayAudioNow(audio, playback);
    }
}

// TODO: not sure if this is the best solutions it's quit of akward
//  also if the player continues immediately after stopping then i have overlap
//  that's not great
void HandleAnimation()
{
    Player.time_since_anim_start += Timer.sim_time;

    if (Player.time_since_anim_start >= STEP_ANIM_TIME)
    {
        Player.time_since_anim_start = 0;
        Player.in_animation = false;
        if (CurrentTile.is_walkable)
        {
            PlayTileAudio(CurrentTile.type, false, Player.orientation);
        }
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
            // TODO: this is almost 100% the logic of the other one, instead
            // which mapping to use
            //-> i need to unify those!!!
            if (Audio.interaction_mapping.find(forwardTile.type) !=
                Audio.interaction_mapping.end())
            {
                FxInfo fxInfo = Audio.interaction_mapping[forwardTile.type];
                AudioData* audio = &Audio.Fx[fxInfo.start_idx];

                PlaybackSettings playback = {};
                playback.settings = &Player.body;

                if (Player.orientation == v2{-1, 0})
                {
                    playback.pan = -.65;
                }
                else if (Player.orientation == v2{1, 0})
                {
                    playback.pan = .65;
                }
                else if (Player.orientation == v2{0, 1})
                {
                    playback.lowpass_filter = 0.45;
                }
                PlayAudioNow(audio, playback);
            }
        }
    }
}

void HandleMovement()
{
    v2 direction = GetMovementDirection();
    if (Player.in_animation)
    {
        HandleAnimation();

        // giving back control next frame
        return;
    }

    if (direction == v2{0, 0}) return;

    v2 nextPosition = Player.position + direction;
    Tile nextTile = TileAt(nextPosition);

    if (nextTile.is_walkable)
    {
        Player.orientation = direction;
        Player.position = nextPosition;
        Player.in_animation = true;
        CurrentTile = nextTile;

        PlayTileAudio(nextTile.type, true, direction);
    }
    else
    {
        Player.orientation = direction;
        PlayTileAudio(nextTile.type, direction);
    }
}
