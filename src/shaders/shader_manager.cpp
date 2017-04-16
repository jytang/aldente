#include "shader_manager.h"
#include "skybox_shader.h"
#include "basic_shader.h"
#include "shadow_shader.h"

std::map<const char*, Shader*> ShaderManager::shaders;
Shader * ShaderManager::default_shader;

void ShaderManager::destroy()
{
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
        delete(it->second);
}

void ShaderManager::create_shader_program(const char *type)
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_file_path = "shaders/" + std::string(type) + "/vert.glsl";
    std::string fragment_file_path = "shaders/" + std::string(type) + "/frag.glsl";

    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Check to make sure the file exists and you passed in the right filepath!\n", type);
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling vertex shader: %s/vert.glsl\n", type);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling fragment shader: %s/frag.glsl\n", type);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    printf("Compiled and linked shader: %s\n", type);

    Shader *s;
    std::string name = std::string(type);
    if (name == "basic")
        s = new BasicShader(ProgramID);
    else if (name == "skybox")
        s = new SkyboxShader(ProgramID);
    else if (name == "shadow")
        s = new ShadowShader(ProgramID);
    else if (name == "debug_shadow")
        s = new Shader(ProgramID);
    else {
        printf("Unregistered shader: %s\n", type);
        return;
    }
    shaders[type] = s;
}

Shader* ShaderManager::get_shader_program(const char *type)
{
    if (shaders.find(type) == shaders.end())
    {
        fprintf(stderr, "could not get shader program: %s!", type);
        return 0;
    }
    return shaders[type];
}

void ShaderManager::set_default(const char *type)
{
    default_shader = get_shader_program(type);
}

Shader* ShaderManager::get_default()
{
    return default_shader;
}
