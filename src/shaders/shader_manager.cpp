#include "shader_manager.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

#define SHADER_SRC_DIR "shaders/"
#define VERT_SHADER_FILE "/vert.glsl"
#define FRAG_SHADER_FILE "/frag.glsl"

GLuint ShaderManager::create_shader_program(const char *type) {
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_file_path = SHADER_SRC_DIR + std::string(type) + VERT_SHADER_FILE;
    std::string fragment_file_path = SHADER_SRC_DIR + std::string(type) + FRAG_SHADER_FILE;

    // Read vertex shader code from file.
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
    if (vertex_shader_stream.is_open()) {
        std::string line = "";
        while (getline(vertex_shader_stream, line))
            vertex_shader_code += "\n" + line;
        vertex_shader_stream.close();
    } else {
        printf("Impossible to open %s.\n", type);
    }

    // Read the fragment shader code from file.
    std::string fragment_shader_code;
    std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
    if (fragment_shader_stream.is_open()) {
        std::string line = "";
        while (getline(fragment_shader_stream, line))
            fragment_shader_code += "\n" + line;
        fragment_shader_stream.close();
    }

    GLint result = GL_FALSE;
    int info_log_len;

    // Compile vertex shader
    printf("Compiling vertex shader: %s\n", type + VERT_SHADER_FILE);
    char const *vertex_source_pointer = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, NULL);
    glCompileShader(vertex_shader_id);

    // Verify vertex shader
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 0) {
        std::vector<char> vertex_shader_error_message(info_log_len + 1);
        glGetShaderInfoLog(vertex_shader_id, info_log_len, NULL, &vertex_shader_error_message[0]);
        printf("%s\n", &vertex_shader_error_message[0]);
    }

    // Compile fragment shader
    printf("Compiling fragment shader: %s\n", type + FRAG_SHADER_FILE);
    char const *fragment_source_pointer = fragment_shader_code.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, NULL);
    glCompileShader(fragment_shader_id);

    // Verify fragment shader
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 0) {
        std::vector<char> fragment_shader_error_message(info_log_len + 1);
        glGetShaderInfoLog(fragment_shader_id, info_log_len, NULL, &fragment_shader_error_message[0]);
        printf("%s\n", &fragment_shader_error_message[0]);
    }

    // Link the program
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 0) {
        std::vector<char> program_error_message(info_log_len + 1);
        glGetProgramInfoLog(program_id, info_log_len, NULL, &program_error_message[0]);
        printf("%s\n", &program_error_message[0]);
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    printf("Compiled and linked shader: %s\n", type);

    return program_id;
}
