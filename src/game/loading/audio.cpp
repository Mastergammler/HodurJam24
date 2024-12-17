#include "../internal.h"
#include "../map.h"

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
            break;
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
    LoadOggAsPcm(Audio.OpenChest, "res/amb/s_open-chest.ogg");
    LoadOggAsPcm(Audio.DangerSound, "res/amb/s_level-start.ogg");
    LoadOggAsPcm(Audio.SuccessSound, "res/amb/s_level-success.ogg");
    LoadOggAsPcm(Audio.LockIn, "res/amb/s_lock-in.ogg");
    LoadOggAsPcm(Audio.ObtainKeys, "res/amb/s_obtain-keys.ogg");
    LoadOggAsPcm(Audio.UnlockDoor, "res/amb/s_unlock-door.ogg");

    // TODO: handle this via mapping might be better?
    LoadOggAsPcm(Audio.UiDown, "res/ui/s_whoosh-001.ogg");
    LoadOggAsPcm(Audio.UiUp, "res/ui/s_whoosh-002.ogg");
    LoadOggAsPcm(Audio.UiRight, "res/ui/s_whoosh-003.ogg");
    LoadOggAsPcm(Audio.UiLeft, "res/ui/s_whoosh-004.ogg");
    LoadOggAsPcm(Audio.UiEnter, "res/ui/s_big-whoosh.ogg");
    LoadOggAsPcm(Audio.MenuAtmo, "res/amb/s_menu-ambience.ogg");
}
