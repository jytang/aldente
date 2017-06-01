#pragma once

#include "ui_container.h"
#include "ui_rectangle.h"
#include "ui_halo_container.h"
#include "game/direction.h"
#include "util/color.h"

#include <glm/glm.hpp>

class UIGrid : public UIContainer {
public:
    UIGrid(float start_x, float start_y,
           float grid_width, float grid_height,
           int num_elements, int columns,
           float element_width, float element_height,
           Color grid_bg_color,
           float inter_padding=0.75f,
           float selection_halo_padding=0.3f, // hardcoded defaults :(
           float grid_bg_alpha=1.f);
    ~UIGrid();
    void attach_at(int row, int col, UIElement &child);
    void detach_at(int row, int col, UIElement &child);
    void draw(Render2D &renderer_2d,
        float offset_x, float offset_y) override;
    void enable() override;
    void disable() override;
    void set_alpha(float alpha) override;
    int get_selection_index(); // returns the grid index of the current selection
    void move_selection(Direction d);
private:
    void toggle_current_selection_halo();

    float grid_width, grid_height; // size of entire grid rectangle
    int num_elements;
    int columns, rows; // grid dimensions
    float element_width, element_height;
    Color grid_bg_color;
    float inter_padding; // padding between elts
    float selection_halo_padding; // width of selection halo

    float h_border_padding; // calculated horizontal border padding
    float v_border_padding; // calculated vertical border padding
    float total_element_width, total_element_height; // elt size incl padding

    UIRectangle grid_bg;

    int selection_row, selection_col;
};
