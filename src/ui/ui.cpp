#include "ui.h"

#include "shaders/shader_manager.h"
#include "ui_rectangle.h"
#include "ui_text_node.h"
#include "ui_image_node.h"

#include <iostream>

// get click info from event callback
void click_callback(events::MouseData d)
{
    fprintf(stderr,
            "MouseEvent:\n"
            "  x: %d\n"
            "  y: %d\n",
            d.x, d.y);
    // which button was clicked? send event.
}

UI::UI(int width, int height)
{
    renderer_2d = new Render2D();
    renderer_2d->setup(width, height);

    // Setup some test UI.
    UI::ID id2 = create_rectangle(0, 100, 100, 50, 50, false, glm::vec3(1.f, 1.f, 1.f));
    UI::ID id4 = create_text_node(0, 500.f, 500.f, false, glm::vec3(1.f, 1.f, 1.f), "GOODBYE!", 1.f);
    UI::ID id5 = create_text_node(0, 100.f, 100.f, false, glm::vec3(0.7f, 0.8f, 0.9f), "middle World!", 1.f);
    UI::ID id3 = create_text_node(0, 25.f, 25.f, false, glm::vec3(0.5f, 0.5f, 0.5f), "Hello World!", 1.f);
    UI::ID id6 = create_text_node(0, 300.f, 300.f, false, glm::vec3(1.f, 0.f, 0.f), "red means mad!", 1.f);
    UI::ID id1 = create_rectangle(0, 50, 50, 50, 50, false, glm::vec3(1.f, 1.f, 1.f));
    UI::ID id7 = create_rectangle(0, 150, 150, 50, 50, false, glm::vec3(1.f, 1.f, 1.f));

    events::MouseEvent::subscribe(&click_callback);
}

void UI::update()
{
    // Loop through zbuffers.
    for (unsigned int i = 0; i <= max_z_depth; ++i)
    {
        // Check if any elements at this depth.
        auto it = z_buffers.find(i);
        if (it == z_buffers.end()) continue;

        auto list = it->second;
        for (auto list_it = list.begin(); list_it != list.end(); ++list_it)
        {
            UIElement *element = *list_it;
            element->draw(renderer_2d);
        }
    }
}

void UI::set_enabled(UI::ID element_id, bool enable)
{
    auto it = id_lookup.find(element_id);
    if (it != id_lookup.end())
    {
        if (enable)
            it->second->enable();
        else
            it->second->disable();
    }
}

// Create a UIRectangle at position relative to parent, as specified by parent ID.
// No parent if ID is 0. Returns ID of the newly created element.
UI::ID UI::create_rectangle(UI::ID parent_id, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color)
{
    // Calculate position of the new rectangle based off of the parent, if any.
    UIElement* parent = get_element_by_id(parent_id);
    UIElement::UIPosition pos = derive_position_from_parent(parent, x_offset, y_offset);

    // Create the new element!
    UIElement *new_element = new UIRectangle(parent, pos, { width, height }, interactable, color);

    // Adjust links and lookup tables as necessary. Returns ID of this element.
    return finalize_element_addition(new_element, parent, pos.z, interactable);
}

// Create a UITextNode with given text and scale at position relative to parent, as specified by parent ID.
// No parent if ID is 0. Returns ID of the newly created element.
UI::ID UI::create_text_node(UI::ID parent_id, int x_offset, int y_offset, bool interactable, glm::vec3 color, std::string text, float scale)
{
    // Calculate position of the new rectangle based off of the parent, if any.
    UIElement* parent = get_element_by_id(parent_id);
    UIElement::UIPosition pos = derive_position_from_parent(parent, x_offset, y_offset);

    // Create the new element!
    UIElement *new_element = new UITextNode(parent, pos, interactable, color, text, scale);

    // Adjust links and lookup tables as necessary. Returns ID of this element.
    return finalize_element_addition(new_element, parent, pos.z, interactable);
}

// Create a UIImageNode with given texture at position relative to parent, as specified by parent ID.
// No parent if ID is 0. Returns ID of the newly created element.
UI::ID UI::create_image_node(UI::ID parent_id, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color, GLuint textureID)
{
    // Calculate position of the new rectangle based off of the parent, if any.
    UIElement* parent = get_element_by_id(parent_id);
    UIElement::UIPosition pos = derive_position_from_parent(parent, x_offset, y_offset);

    // Create the new element!
    UIElement *new_element = new UIImageNode(parent, pos, { width, height }, interactable, color, textureID);

    // Adjust links and lookup tables as necessary. Returns ID of this element.
    return finalize_element_addition(new_element, parent, pos.z, interactable);
}

// Retrieves pointer to element referred to by ID, else null pointer is returned.
UIElement* UI::get_element_by_id(UI::ID id)
{
    auto it = id_lookup.find(id);
    if (it == id_lookup.end())
        return NULL;
    else
        return it->second;
}

// Calculates and returns the position (x,y,z) of a new element.
// Position is based off of the parent, as specified by parent ID.
UIElement::UIPosition UI::derive_position_from_parent(UIElement *parent, int x_offset, int y_offset)
{
    int x_pos = x_offset;
    int y_pos = y_offset;
    int z_depth = 0;

    if (parent != NULL)
    {
        // Calculate x and y relative to parent. z depth is parent's + 1
        x_pos += parent->pos.x;
        y_pos += parent->pos.y;
        z_depth = 1 + parent->pos.z;

        // Adjust max z depth as necessary
        max_z_depth = z_depth > max_z_depth ? z_depth : max_z_depth;
    }

    return {x_pos, y_pos, z_depth};
}

// Attaches new element to parent and updates tables (z_buffer, id, interactables) as necessary.
UI::ID UI::finalize_element_addition(UIElement *new_element, UIElement *parent, int z_depth, bool interactable)
{
    if (parent != 0)
        parent->add_child(new_element);

    // Add to lookup tables as necessary
    auto zbuf_it = z_buffers.find(z_depth);
    if (zbuf_it == z_buffers.end())
        z_buffers[z_depth] = std::vector<UIElement *>();
    z_buffers[z_depth].push_back(new_element);

    id_lookup[next_id] = new_element;

    if (interactable)
        interactables.push_back(new_element);

    // Return ID of this element, and increment.
    return next_id++;
}
