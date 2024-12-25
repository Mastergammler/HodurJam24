#include "../internal.h"
#include "../map.h"

static unordered_map<string, TileType> FX_MAPPING = {
                                        {"footstep-stone", STONE},
                                        {"footstep-carpet", CARPET},
                                        {"footstep-wood-hi", WOOD},
                                        {"footsteps-grass", GRASS},
                                        {"wall-bump", WALL},
                                        {"chest-bump", CHEST},
                                        {"door-bump", DOOR},
                                        {"pillar-bump", PILLAR},
                                        // TODO: I keep adding things that are
                                        // not actually tiles
                                        //  -> I have to find another solution
                                        {"nope-sound", NOOP},
                                        {"huh", HUH},
                                        {"pocket-keys", FOLEY},
};

static unordered_map<string, TileType> BEAR_FX_MAPPING = {
                                        {"footsteps-bear-stone", STONE},
                                        {"footsteps-bear-carpet", CARPET},
                                        {"footsteps-bear-wood", WOOD},
                                        {"footsteps-bear-grass", GRASS},
                                        {"foley-bear", FOLEY}

};

// TODO: mapping audio files for multiple things?
void AddMapping(string fileName, int index, FxInfo** lastInfo)
{
    for (const auto& pair : FX_MAPPING)
    {
        if (starts_with(fileName, pair.first))
        {
            FxInfo* last = *lastInfo;
            if (last != NULL && last->type == pair.second)
            {
                last->count++;
            }
            else
            {
                FxInfo newInfo = {pair.second, index, 1};
                Audio.fx_mapping.insert({newInfo.type, newInfo});
                *lastInfo = &Audio.fx_mapping[newInfo.type];
            }
            // every file is only allowed to be mapped once atm
            return;
        }
    }

    for (const auto& pair : BEAR_FX_MAPPING)
    {
        if (starts_with(fileName, pair.first))
        {
            FxInfo* last = *lastInfo;
            if (last != NULL && last->type == pair.second)
            {
                last->count++;
            }
            else
            {
                FxInfo newInfo = {pair.second, index, 1};
                Audio.bear_fx_mapping.insert({newInfo.type, newInfo});
                *lastInfo = &Audio.bear_fx_mapping[newInfo.type];
            }
            // every file is only allowed to be mapped once atm
            return;
        }
    }
}

void LoadFx()
{
    vector<FileEntry> files = directory_files("res/fx");

    Audio.fx_count = files.size();
    Audio.fx = new AudioData[Audio.fx_count]();

    FxInfo* lastInfo = NULL;
    for (int i = 0; i < Audio.fx_count; i++)
    {
        AddMapping(files[i].file_name, i, &lastInfo);
        LoadOggAsPcm(Audio.fx[i], files[i].path);
    }
}

/**
 * Currently only loads the number sounds
 */
void LoadUiSounds()
{
    vector<FileEntry> files = directory_files("res/ui");

    Audio.ui_count = files.size();
    Audio.ui = new AudioData[Audio.ui_count];

    for (int i = 0; i < Audio.ui_count; i++)
    {
        string name = files[i].file_name;
        if (starts_with(name, "num"))
        {
            // TODO: makes this sense now with the new approach?
            remove_until(name, '-');
            int num = stoi(name);
            Audio.num_mapping.insert({num, i});
        }

        // TODO: map to number
        LoadOggAsPcm(Audio.ui[i], files[i].path);
    }
}

void LoadStaticAudio()
{
    LoadOggAsPcm(Audio.DangerSound, "res/amb/s_level-start.ogg");
    LoadOggAsPcm(Audio.SuccessSound, "res/amb/s_level-success.ogg");
    LoadOggAsPcm(Audio.FailSound, "res/amb/s_level-fail.ogg");
    LoadOggAsPcm(Audio.GameSuccessSound, "res/amb/s_victory.ogg");

    LoadOggAsPcm(Audio.OpenChest, "res/amb/s_open-chest.ogg");
    LoadOggAsPcm(Audio.LockIn, "res/amb/s_locked-in.ogg");
    LoadOggAsPcm(Audio.ObtainKeys, "res/amb/s_obtain-keys.ogg");
    LoadOggAsPcm(Audio.UnlockDoor, "res/amb/s_unlock-door.ogg");
    LoadOggAsPcm(Audio.BearGrowl, "res/fx/s_bear-growl.ogg");
    LoadOggAsPcm(Audio.BearBreathingLoop, "res/fx/l_bear-breathing.ogg");
    // TODO: rename to just proximity-yellow etc
    LoadOggAsPcm(Audio.ProximityYellow, "res/amb/s_proximity_code-yellow.ogg");
    LoadOggAsPcm(Audio.ProximityRed, "res/amb/s_proximity_level-red.ogg");

    // TODO: handle this via mapping might be better?
    LoadOggAsPcm(Audio.UiDown, "res/ui/s_whoosh-001.ogg");
    LoadOggAsPcm(Audio.UiUp, "res/ui/s_whoosh-002.ogg");
    LoadOggAsPcm(Audio.UiRight, "res/ui/s_whoosh-003.ogg");
    LoadOggAsPcm(Audio.UiLeft, "res/ui/s_whoosh-004.ogg");
    LoadOggAsPcm(Audio.UiEnter, "res/ui/s_big-whoosh.ogg");
    LoadOggAsPcm(Audio.MenuAtmo, "res/amb/s_menu-ambience.ogg");

    LoadOggAsPcm(Audio.HelpInfo, "res/voice/help-info.ogg");
    LoadOggAsPcm(Audio.HelpMenu, "res/voice/help-menu.ogg");
    LoadOggAsPcm(Audio.HelpGame, "res/voice/help-game.ogg");
    LoadOggAsPcm(Audio.Intro, "res/voice/find-chest.ogg");
    LoadOggAsPcm(Audio.ThereIsABear, "res/voice/avoid-bear.ogg");
    LoadOggAsPcm(Audio.Victory, "res/voice/victory.ogg");
}
