#pragma once

#include "ui.h"
#include "ui_controller_legend.h"

class LegendUI : public UI {
public:
    typedef std::vector<std::pair<std::string, std::string>> str_pair;

    LegendUI(float aspect, float legend_width, float entry_height, float padding);
private:
    std::vector<std::unique_ptr<UIControllerLegend>> legends;
    float legend_width;
    float entry_height;
    float padding;

    // Takes [(button_texture, text), ...]
    void set_legend(str_pair legend_spec);
};
