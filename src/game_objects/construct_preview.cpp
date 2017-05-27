#include "construct_preview.h"
#include "construct.h"
#include "game_objects/traps/spikes.h"
#include "game_objects/traps/slime.h"
#include "game_objects/traps/arrow_trap.h"

void ConstructPreview::set_construct_type(ConstructType type, bool valid) {
    // Remove visibility of previous preview, if any
    remove_child(curr_preview);

    // Call make_preview if never made preview before
    if (cached_previews.find(type) == cached_previews.end()) {
        switch (type) {
            case CHEST:
                cached_previews[type] = make_preview<Chest>();
                break;
            case SPIKES:
                cached_previews[type] = make_preview<Spikes>();
                break;
            case SLIME_B:
                cached_previews[type] = make_preview<SlimeBlue>();
                break;
            case SLIME_Y:
                cached_previews[type] = make_preview<SlimeYellow>();
                break;
            case SLIME_R:
                cached_previews[type] = make_preview<SlimeRed>();
                break;
            case SLIME_G:
                cached_previews[type] = make_preview<SlimeGreen>();
                break;
            case ARROW:
                cached_previews[type] = make_preview<ArrowTrap>();
                break;
            default:
                cached_previews[type] = make_preview<Chest>();
                break;
        }
    }

    // Set curr preview to be visibile
    curr_preview = cached_previews[type];
    set_valid(valid);
    add_child(curr_preview);
}

template <typename T>
T* ConstructPreview::make_preview() {
    T *construct_preview = new T(0, 0, CLIENT_ONLY_ID);
    construct_preview->setup_model();
    construct_preview->set_filter_alpha(0.5f);
    return construct_preview;
}

void ConstructPreview::set_valid(bool valid) {
    if (curr_preview)
        curr_preview->set_filter_color(valid ? Color::GREEN : Color::RED);
}
