
#include <vector>
#include <iostream>
#include "computeshader.h"




void ComputeShader::BuildShader(const std::string& cscode)
{
	m_Program = glCreateProgram();
	GLuint computeshader = glCreateShader(GL_COMPUTE_SHADER);
	const char* cschar = cscode.c_str();
	glShaderSource(computeshader, 1, &cschar, nullptr);

	glCompileShader(computeshader);

    GLint success;
    glGetShaderiv(computeshader, GL_COMPILE_STATUS, &success); // [1,4](@ref)

    if (!success) {
        // ���Ȼ�ȡ��־���ȣ��Ա㶯̬�����㹻��С�Ļ�����
        GLint maxLength = 0;
        glGetShaderiv(computeshader, GL_INFO_LOG_LENGTH, &maxLength); // [7](@ref)

        // �����ڴ����洢������־��maxLength������ֹnull�ַ���
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(computeshader, maxLength, &maxLength, &errorLog[0]); // [1,7](@ref)

        // ���������Ϣ
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &errorLog[0] << std::endl;

        // ɾ������ʧ�ܵ���ɫ�����󣬱�����Դй©
        glDeleteShader(computeshader); // [7](@ref)
        // �������...
    }



	glAttachShader(m_Program, computeshader);
	glLinkProgram(m_Program);


    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success) {

        GLint maxLength = 0;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &maxLength); // [8](@ref)
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(m_Program, maxLength, NULL, errorLog.data()); // [4,8](@ref)
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog.data() << std::endl;
    }

}