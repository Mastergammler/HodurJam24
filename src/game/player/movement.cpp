#include "../internal.h"
#include "../map.h"

static TileType CurrentTileType = WALL;

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

void HandleMovement()
{
    v2 direction = GetMovementDirection();

    if (direction == v2{0, 0}) return;

    v2 nextPosition = Player.position + direction;
    Tile nextTile = TileAt(nextPosition);

    if (nextTile.is_walkable)
    {
        Player.position = nextPosition;
        // play 2 sounds etc & lock player etc
    }

    if (Audio.fx_mapping.find(nextTile.type) != Audio.fx_mapping.end())
    {
        FxInfo fxInfo = Audio.fx_mapping[nextTile.type];
        int idxOffset = rand() % fxInfo.count;
        PlayNewAudio(&Audio.Fx[fxInfo.start_idx + idxOffset]);
    }
}
