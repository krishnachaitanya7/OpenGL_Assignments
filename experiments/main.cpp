/*
 * Vertex Buffer: It's an array of bytes of memory. The difference between normal arrays and vertex buffers is that it's
 * an array stored in GPU.
 * Shader: Do manipulation
 * Using GLGenBuffer you have to bind everything
 * And later work on it
 * Usually For shaders First Vertex Shader -> Frag Shader -> Output on screen
 * For each vertex vertex shader is run
 * Frag/Pixel Shader runs for each pixel that needs to rasterized or shown as output. Basically it's the coloring book
 * For a triangle vertex shader runs 3 times, but frag shader would run 10k times, basically for each pixel the triangle
 * covers.
 * */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


static void GLCheckError(){
    while(GLenum error = glGetError()){
        std::cout << "Opengl error code: "<< error << std::endl;
    }
}

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType ::NONE;
    while(getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType ::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType ::FRAGMENT;
            }
        }
        else{
                ss[(int)type] << line << std::endl;
        }
    }
    return {ss[0].str(), ss[1].str()};

}
static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        std::cout << "Shader not compiled" << std::endl;
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

//    glDeleteShader(vs);
//    glDeleteShader(fs);

    return program;
}


int main(void){
    GLFWwindow* window;
    if(!glfwInit()){
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK){
        std::cout << "Error! Glew Not Okay" << std::endl;
    }

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    float positions[6] = {
            -0.5f, -0.5f,
            -0.5f, -0.5f,
            -0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    ShaderProgramSource source = ParseShader("basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    glUseProgram(shader);
    GLCheckError();
    /*Loop until the user closes the window*/
    while( !glfwWindowShouldClose(window) ){
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;


}
