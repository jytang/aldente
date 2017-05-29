#pragma once

#include "aldente.h"
#include "aldente_client.h"
#include "aldente_server.h"
#include "asset_loader.h"
#include "bt_debug.h"
//#include "catch.h"
#include "debug_input.h"
//#include "events.h"
#include "physics.h"
#include "render/render.h"
#include "scene_manager.h"
#include "render/shadows.h"
#include "timer.h"
#include "components/transform.h"
#include "window.h"

#include "animation/animation.h"
#include "animation/animation_player.h"

#include "audio/audio_manager.h"

#include "game/construct_types.h"
#include "game/context.h"
#include "game/direction.h"
#include "game/game_state.h"
#include "game/player_stats.h"
#include "game/collectibles/collectible.h"
#include "game/collectibles/gold.h"
#include "game/collectibles/nothing.h"
#include "game/phase/build.h"
#include "game/phase/dungeon.h"
#include "game/phase/menu.h"
#include "game/phase/phase.h"
#include "game/minigame/minigame.h"
#include "game/minigame/mg_penguin.h"

#include "game_objects/construct.h"
#include "game_objects/construct_preview.h"
#include "game_objects/game_object.h"
#include "game_objects/grid.h"
#include "game_objects/player.h"
#include "game_objects/tile.h"
#include "game_objects/minigame/penguin.h"

#include "input/axis_combiner.h"
#include "input/conceptual_translator.h"
#include "input/raw_maps/debug.h"
#include "input/raw_maps/matricom.h"
#include "input/raw_maps/raw_map.h"
#include "input/raw_maps/xbox.h"

#include "model/geometry.h"
#include "model/geometry_generator.h"
#include "model/material.h"
#include "model/mesh.h"
#include "model/model.h"
#include "model/model_filter.h"
#include "model/plane.h"
#include "model/skeleton.h"

#include "net/connection.h"
#include "net/network_client.h"
#include "net/network_manager.h"
#include "net/network_server.h"
#include "net/threadsafe_queue.h"

#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "poll/poller.h"

//#include "proto/net.pb.h"

#include "scene/camera.h"
#include "scene/main_scene.h"
#include "scene/scene.h"
#include "scene/scene_info.h"
#include "scene/start_scene.h"
#include "scene/light/directional_light.h"
#include "scene/light/light.h"
#include "scene/light/point_light.h"
#include "scene/light/pulse_point_light.h"
#include "scene/light/spot_light.h"
#include "scene/minigame_scenes/mgscene_penguin.h"
#include "scene/minigame_scenes/mgscene_penguin.cpp"

#include "shaders/basic_shader.h"
#include "shaders/debug_shadow_shader.h"
#include "shaders/shader.h"
#include "shaders/shader_manager.h"
#include "shaders/shadow_shader.h"
#include "shaders/skybox_shader.h"
#include "shaders/text_shader.h"
#include "shaders/ui_shader.h"

#include "ui/build_ui.h"
#include "ui/clock_ui.h"
#include "ui/debug_ui.h"
#include "ui/render2d.h"
#include "ui/ui.h"
#include "ui/ui_container.h"
#include "ui/ui_element.h"
#include "ui/ui_grid.h"
#include "ui/ui_halo_container.h"
#include "ui/ui_image_node.h"
#include "ui/ui_manager.h"
#include "ui/ui_rectangle.h"
#include "ui/ui_text_node.h"

#include "util/color.h"
#include "util/config.h"
#include "util/util.h"
#include "util/util_bt.h"