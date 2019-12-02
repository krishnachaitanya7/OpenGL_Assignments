//
// Created by shivababa on 12/2/19.
//

#ifndef OPENGL_ASSIGNMENTS_SHADER_H
#define OPENGL_ASSIGNMENTS_SHADER_H
class Shader{
public:
    char* ReadText(char *file);
    void PrintShaderLog(int obj,char* file);
    void PrintProgramLog(int obj);
    int CreateShader(GLenum type,char* file);
    int CreateShaderProg(char* VertFile,char* FragFile);
};
#endif //OPENGL_ASSIGNMENTS_SHADER_H
