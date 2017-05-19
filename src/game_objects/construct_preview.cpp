#include "construct_preview.h"
#include "construct.h"

void ConstructPreview::set_construct_type(ConstructType type) {
    // Remove visibility of previous preview, if any
    remove_child(curr_preview);

    // Call make_preview if never made preview before
    if (cached_previews.find(type) == cached_previews.end()) {
        switch (type) {
            case CHEST:
                cached_previews[type] = make_preview<Chest>();
                break;
            default:
                cached_previews[type] = make_preview<Chest>();
                break;
        }
    }

    // Set curr preview to be visibile
    curr_preview = cached_previews[type];
    add_child(curr_preview);
}

template <typename T>
T* ConstructPreview::make_preview() {
    T *construct_preview = new T(0, 0, -1);
    construct_preview->setup_model();
    construct_preview->set_filter_color(Color::GREEN);
    construct_preview->set_filter_alpha(0.5f);
    return construct_preview;
}
