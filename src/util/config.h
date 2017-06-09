#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <iterator>

const std::string str_settings_file = "cfg/settings.cfg";
const std::string str_template_settings_file = "cfg/settings_template.cfg";

class Config {

public:
    static std::string str_screen_width;
    static std::string str_screen_height;
    static std::string str_full_screen;
    static std::string str_game_name;
    static std::string str_fov;
    static std::string str_far_plane;
    static std::string str_server_ip;
    static std::string str_port;
    static std::string str_controller;
    static std::string str_num_rounds;
    static std::string str_show_dialogues;

public:
    static Config *config; // Use this static reference instead of your own call to the constructor

    bool check_if_loaded();

    void save_settings_file();

    bool load_settings_file();

    bool reload_settings_file();

    void copy_settings_template();

    void copy_missing_settings();

    bool get_value(std::string key, bool &ret);

    bool get_value(std::string key, int &ret);

    bool get_value(std::string key, float &ret);

    bool get_value(std::string key, double &ret);

    bool get_value(std::string key, std::string &ret);

    void update_value(std::string key, bool value);

    void update_value(std::string key, int value);

    void update_value(std::string key, float value);

    void update_value(std::string key, double value);

    void update_value(std::string key, std::string value);

private:
    //default parameters for a default constructor option
    Config(std::string file_name = str_settings_file, std::string template_file_name = str_template_settings_file);

    std::unordered_map<std::string, std::string> settings;
    bool settings_loaded;

    std::string file_name;
    std::string template_file_name;
};