#include "config.h"

using namespace std;

string Config::str_screen_width = "ScreenWidth";
string Config::str_screen_height = "ScreenHeight";
string Config::str_full_screen = "FullScreen";
string Config::str_game_name = "GameName";
string Config::str_fov = "FieldOfView";
string Config::str_far_plane = "FarPlane";
string Config::str_is_server = "IsServer";
string Config::str_server_ip = "ServerIp";
string Config::str_port = "Port";
string Config::str_controller = "Controller";

typedef pair<string, string> StringPair;
Config *Config::config = new Config();

Config::Config(string fname, string template_fname) {
    file_name = fname;
    template_file_name = template_fname;

    if (!load_settings_file())
        copy_settings_template();
    else
        copy_missing_settings();
}

//Reads and loads the settings
bool Config::load_settings_file() {
    ifstream myfile;
    myfile.open(file_name.c_str());

    if (myfile.is_open()) {
        string line;
        while (myfile.good()) {
            getline(myfile, line);
            size_t pos = line.find_first_of(':');

            if (pos > 0 && pos < line.length()) {
                string key, value;
                key = line.substr(0, pos);
                value = line.substr(pos + 2);

                settings.insert(StringPair(key, value));
            }
        }
        settings_loaded = true;
        myfile.close();
        return true;
    }

    return false;
}

//clears data read and loads from the settings file
bool Config::reload_settings_file() {
    settings.clear();
    return load_settings_file();
}

//set the default settings from the config file
//loads the config file while copying
void Config::copy_settings_template() {
    ifstream template_file;
    ofstream myfile;

    template_file.open(template_file_name.c_str());
    myfile.open(file_name.c_str());

    if (template_file.is_open()) {
        string line;
        while (template_file.good()) {
            getline(template_file, line);

            size_t pos = line.find_first_of(':');
            if (pos > 0 && pos < line.length()) {
                string key, value;
                key = line.substr(0, pos);
                value = line.substr(pos + 2);


                if (myfile.is_open())
                    myfile << line << endl;

                settings.insert(StringPair(key, value)); //loads the config file while copying
            }
        }
        settings_loaded = true;
        template_file.close();
        myfile.close();
    }
}

void Config::copy_missing_settings() {
    ifstream template_file;
    fstream myfile;

    template_file.open(template_file_name.c_str());
    myfile.open(file_name.c_str(), fstream::in | fstream::out | fstream::app);

    if (template_file.is_open()) {
        string line;
        while (template_file.good()) {
            getline(template_file, line);

            size_t pos = line.find_first_of(':');
            if (pos > 0 && pos < line.length()) {
                string key, value;
                key = line.substr(0, pos);
                value = line.substr(pos + 2);

                unordered_map<string, string>::iterator i = settings.find(key);

                if (i == settings.end()) {
                    if (myfile.is_open())
                        myfile << line << endl;
                    settings.insert(StringPair(key, value));
                }
            }
        }
        template_file.close();
        myfile.close();
    }
}

void Config::save_settings_file() {
    ofstream myfile;
    myfile.open(file_name.c_str());

    if (myfile.is_open()) {
        string line;
        unordered_map<string, string>::iterator endIter = settings.end();
        unordered_map<string, string>::iterator iter;

        for (iter = settings.begin(); iter != endIter; iter++)
            myfile << iter->first << ": " << iter->second << endl;
        myfile.close();
    }
}

bool Config::get_value(string key, string &ret) {
    unordered_map<string, string>::iterator i = settings.find(key);

    if (i != settings.end()) {
        ret = i->second;
        return true;
    }
    return false;
}

bool Config::get_value(string key, bool &ret) {
    unordered_map<string, string>::iterator i = settings.find(key);

    if (i != settings.end()) {
        if (i->second == "true")
            ret = true;
        else
            ret = false;
        return true;
    }
    return false;
}

bool Config::get_value(string key, int &ret) {
    unordered_map<string, string>::iterator i = settings.find(key);

    if (i != settings.end()) {
        ret = atoi(i->second.c_str()); //error results in 0
        return true;
    }
    return false;
}

bool Config::get_value(string key, float &ret) {
    unordered_map<string, string>::iterator i = settings.find(key);

    if (i != settings.end()) {
        ret = (float) atof(i->second.c_str()); //error results in 0
        return true;
    }
    return false;
}

bool Config::get_value(string key, double &ret) {
    unordered_map<string, string>::iterator i = settings.find(key);

    if (i != settings.end()) {
        ret = atof(i->second.c_str()); //error results in 0
        return true;
    }
    return false;
}

void Config::update_value(string key, string value) {
    settings.insert(StringPair(key, value));
}


void Config::update_value(string key, bool value) {
    if (value)
        settings.insert(StringPair(key, "true"));
    else
        settings.insert(StringPair(key, "false"));
}

void Config::update_value(string key, int value) {
    stringstream s;
    s << value;

    settings.insert(StringPair(key, s.str()));
}


void Config::update_value(string key, float value) {
    stringstream s;
    s << value;

    settings.insert(StringPair(key, s.str()));
}

void Config::update_value(string key, double value) {
    stringstream s;
    s << value;

    settings.insert(StringPair(key, s.str()));
}

//returns true if loaded the saved settings, otherwise false
bool Config::check_if_loaded() {
    return settings_loaded;
}
