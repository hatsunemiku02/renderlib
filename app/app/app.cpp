#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>


#include "renderlib/vertexbuffer.h"
#include "renderlib/shader.h"
#include "renderlib/pass.h"
#include "renderlib/shaderparam.h"
#include "renderlib/renderobj.h"
#include "resourcemgr/submesh.h"


// 窗口尺寸常量
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;





// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";


const char* vertexShaderSourceFB = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 uv;
    out vec2 texcord;
    void main() {
        texcord = uv;
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSourceFB = R"(
    #version 330 core
    in vec2 texcord;
    out vec4 FragColor;
    uniform sampler2D screentex;
    void main() {
        FragColor =texture(screentex,texcord);
    }
)";

int main()
{
    // 初始化GLFW
    glfwInit();

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 核心模式

    // 如果是Mac OS X系统，需要添加下面这行
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 注册窗口大小改变的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;



    GLuint test = 3;


    /*
    GLenum error;
    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    error = glGetError();
    glGenBuffers(1, &vbo);
    error = glGetError();

    glBindVertexArray(vao);
    error = glGetError();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    error = glGetError();

    std::vector<float> vtx = { -0.5f, -0.5f, 0.0f,  // 左下角
         0.5f, -0.5f, 0.0f,  // 右下角
         0.0f,  0.5f, 0.0f   // 顶部
    };

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // 左下角
     0.5f, -0.5f, 0.0f,  // 右下角
     0.0f,  0.5f, 0.0f   // 顶部
    };
    float* data = vtx.data();
    int size = sizeof(float) * 9;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    error = glGetError();

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);
    error = glGetError();
    glEnableVertexAttribArray(0);

    error = glGetError();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    error = glGetError();
    glBindVertexArray(0);




    float verticesFBO[] = {
     -1.0f, -1.0f, 0.0f,0.f,1.f,  // 左下角
     1.0f, -1.0f, 0.0f,1.f,1.f,  // 右下角
     1.0f,  1.0f, 0.0f,1.f,0.f,   // 右上

     -1.0f, -1.0f, 0.0f,0.f,1.f,  // 左下角
      1.0f,  1.0f, 0.0f,1.f,0.f,   // 右上
     -1.0f,  1.0f, 0.0f,0.f,0.f   //左上
    };

    GLuint vaoFB;
    GLuint vboFB;
    glGenVertexArrays(1, &vaoFB);
    glGenBuffers(1, &vboFB);

    glBindVertexArray(vaoFB);
    glBindBuffer(GL_ARRAY_BUFFER, vboFB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFBO), verticesFBO, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), nullptr);
    error = glGetError();
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    error = glGetError();
    glEnableVertexAttribArray(1);



    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 创建和编译着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 创建着色器程序并链接
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    error = glGetError();

    // 创建和编译着色器
    unsigned int vertexShaderFB = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderFB, 1, &vertexShaderSourceFB, NULL);
    glCompileShader(vertexShaderFB);

    GLint success;
    glGetShaderiv(vertexShaderFB, GL_COMPILE_STATUS, &success); // [1,4](@ref)

    if (!success) {
        // 首先获取日志长度，以便动态分配足够大小的缓冲区
        GLint maxLength = 0;
        glGetShaderiv(vertexShaderFB, GL_INFO_LOG_LENGTH, &maxLength); // [7](@ref)

        // 分配内存来存储错误日志（maxLength包含终止null字符）
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShaderFB, maxLength, &maxLength, &errorLog[0]); // [1,7](@ref)

        // 输出错误信息
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &errorLog[0] << std::endl;

        // 删除编译失败的着色器对象，避免资源泄漏
        glDeleteShader(vertexShaderFB); // [7](@ref)
        // 处理错误...
    }

    unsigned int fragmentShaderFB = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderFB, 1, &fragmentShaderSourceFB, NULL);
    glCompileShader(fragmentShaderFB);

    glGetShaderiv(fragmentShaderFB, GL_COMPILE_STATUS, &success); // [1,4](@ref)

    if (!success) {
        // 首先获取日志长度，以便动态分配足够大小的缓冲区
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderFB, GL_INFO_LOG_LENGTH, &maxLength); // [7](@ref)

        // 分配内存来存储错误日志（maxLength包含终止null字符）
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShaderFB, maxLength, &maxLength, &errorLog[0]); // [1,7](@ref)

        // 输出错误信息
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &errorLog[0] << std::endl;

        // 删除编译失败的着色器对象，避免资源泄漏
        glDeleteShader(fragmentShaderFB); // [7](@ref)
        // 处理错误...
    }

    // 创建着色器程序并链接
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

    error = glGetError();

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    error = glGetError();


    GLuint texattach;
    glGenTextures(1, &texattach);
    error = glGetError();
    glBindTexture(GL_TEXTURE_2D, texattach);
    error = glGetError();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    error = glGetError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    error = glGetError();
    glBindTexture(GL_TEXTURE_2D, 0);



    error = glGetError();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texattach, 0);
    error = glGetError();
    GLuint dsbuffer;
    glGenRenderbuffers(1, &dsbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, dsbuffer);
    error = glGetError();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    error = glGetError();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    error = glGetError();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dsbuffer);
    error = glGetError();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    error = glGetError();


    error = glGetError();*/

    //std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>();
    std::shared_ptr<SubMesh> submesh = std::make_shared<SubMesh>();
    submesh->BuildDefaultBox();
    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    std::shared_ptr<Param> param = std::make_shared<Param>();
    shader->BuildDefaultShader();

    std::shared_ptr<RenderObj> renderobj = std::make_shared<RenderObj>(submesh,shader,param);
    std::shared_ptr<Pass> pass = std::make_shared<Pass>();

    pass->AddRenderObj(renderobj);
    pass->SortRenderObjVec2DrawCall();

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);


        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pass->ApplyOpenglAPI();
        /*
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        error = glGetError();
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        // 渲染指令
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // 清除颜色缓冲

        //glEnable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        error = glGetError();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        error = glGetError();



        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    



        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        error = glGetError();
        glClearColor(1.0f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 清除颜色缓冲
        error = glGetError();
        glBindVertexArray(vaoFB);
        error = glGetError();
        glUseProgram(shaderProgramFB);
        error = glGetError();
        glActiveTexture(GL_TEXTURE0);
        error = glGetError();
        glBindTexture(GL_TEXTURE_2D, texattach);
        error = glGetError();
        glUniform1i(glGetUniformLocation(shaderProgramFB, "screentex"), 0);
        error = glGetError();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        error = glGetError();*/
        // 交换颜色缓冲
        glfwSwapBuffers(window);

        // 检查并调用事件
        glfwPollEvents();
    }

    // 清理所有资源
    glfwTerminate();
    return 0;
}

// 处理输入
void processInput(GLFWwindow* window)
{
    // 如果按下ESC键，则设置窗口应该关闭
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 当窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 设置视口大小
    glViewport(0, 0, width, height);
}