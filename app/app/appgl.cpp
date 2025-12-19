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


void filluniformdata(float x,float y,float (&offset)[4])
{
	for (int i = 0; i < 4; i++)
	{
		offset[i] = 0;
	}
	offset[0] = x;
    offset[1] = y;
}

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


    //std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>();
    std::shared_ptr<SubMesh> submesh = std::make_shared<SubMesh>();
    submesh->BuildDefaultBox();
    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->BuildDefaultShader();

    std::vector<std::shared_ptr<Param>> paramlist;

    for (int i=0;i<4;i++)
    {
		std::shared_ptr<Param> param = std::make_shared<Param>();
		float offset[4];
		filluniformdata(-0.3 + 0.1*i, -0.1+0.1*i, offset);
        param->CreateUniformBuffer(&offset[0], sizeof(float) * 4);
        paramlist.push_back(param);

    }

    std::shared_ptr<Pass> pass = std::make_shared<Pass>();
    for (int i = 0; i < 4; i++)
    {
        std::shared_ptr<RenderObj> renderobj = std::make_shared<RenderObj>(submesh, shader, paramlist[i]);
        pass->AddRenderObj(renderobj);
    }

    pass->SortRenderObjVec2DrawCall();

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);


        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pass->ApplyOpenglAPI();
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