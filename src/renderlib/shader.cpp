#include "shader.h"
#include <vector>
#include <iostream>



std::string DEFAULT_VS = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 uv;
    out vec2 texcord;
    uniform mat4 u_WorldMatrix;
    uniform mat4 u_ProjMatrix;
    uniform mat4 u_ViewMatrix;
    void main() {
        texcord = uv;
        gl_Position =u_ProjMatrix*u_ViewMatrix*u_WorldMatrix*vec4(aPos.x,aPos.y,aPos.z,1.0);
    }
)";

std::string DEFAULT_PS = R"(
    #version 330 core
    in vec2 texcord;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.4,0.4,0.7,1.0);
    }
)";

void Shader::BuildDefaultShader()
{
    BuildShader(DEFAULT_VS, DEFAULT_PS);
}

void Shader::BuildShader(const std::string& vs_source, const std::string& fs_source)
{
    unsigned int vertexShaderFB = glCreateShader(GL_VERTEX_SHADER);
    const char* vschar = vs_source.c_str();
    glShaderSource(vertexShaderFB, 1, &vschar, NULL);
    glCompileShader(vertexShaderFB);

    GLint success;
    glGetShaderiv(vertexShaderFB, GL_COMPILE_STATUS, &success); // [1,4](@ref)

    if (!success) {
        // ���Ȼ�ȡ��־���ȣ��Ա㶯̬�����㹻��С�Ļ�����
        GLint maxLength = 0;
        glGetShaderiv(vertexShaderFB, GL_INFO_LOG_LENGTH, &maxLength); // [7](@ref)

        // �����ڴ����洢������־��maxLength������ֹnull�ַ���
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShaderFB, maxLength, &maxLength, &errorLog[0]); // [1,7](@ref)

        // ���������Ϣ
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &errorLog[0] << std::endl;

        // ɾ������ʧ�ܵ���ɫ�����󣬱�����Դй©
        glDeleteShader(vertexShaderFB); // [7](@ref)
        // �������...
    }

    unsigned int fragmentShaderFB = glCreateShader(GL_FRAGMENT_SHADER);
    const char* pschar = fs_source.c_str();
    glShaderSource(fragmentShaderFB, 1, &pschar, NULL);
    glCompileShader(fragmentShaderFB);

    glGetShaderiv(fragmentShaderFB, GL_COMPILE_STATUS, &success); // [1,4](@ref)

    if (!success) {
        // ���Ȼ�ȡ��־���ȣ��Ա㶯̬�����㹻��С�Ļ�����
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderFB, GL_INFO_LOG_LENGTH, &maxLength); // [7](@ref)

        // �����ڴ����洢������־��maxLength������ֹnull�ַ���
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShaderFB, maxLength, &maxLength, &errorLog[0]); // [1,7](@ref)

        // ���������Ϣ
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &errorLog[0] << std::endl;

        // ɾ������ʧ�ܵ���ɫ�����󣬱�����Դй©
        glDeleteShader(fragmentShaderFB); // [7](@ref)
        // �������...
    }

    // ������ɫ����������
    unsigned int shaderProgramFB = glCreateProgram();
    glAttachShader(shaderProgramFB, vertexShaderFB);
    glAttachShader(shaderProgramFB, fragmentShaderFB);
    glLinkProgram(shaderProgramFB);

    glGetProgramiv(shaderProgramFB, GL_LINK_STATUS, &success);
    if (!success) {

        GLint maxLength = 0;
        glGetProgramiv(shaderProgramFB, GL_INFO_LOG_LENGTH, &maxLength); // [8](@ref)
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(shaderProgramFB, maxLength, NULL, errorLog.data()); // [4,8](@ref)
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog.data() << std::endl;
    }

    glDeleteShader(vertexShaderFB);
    glDeleteShader(fragmentShaderFB);

    GLuint error = glGetError();
}