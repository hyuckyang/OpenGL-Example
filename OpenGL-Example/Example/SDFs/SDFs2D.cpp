#include "SDFs2D.h"

SDFs2D::SDFs2D()
{
    vertShaderFile = "Example/SDFs/shader.vert";
    fragShaderFile = "Example/SDFs/shader.frag";

    currentTime = 0.0f;
}

void SDFs2D::InitFunc(GLFWwindow* window)
{
    Framework_ShaderLoader::InitFunc(window);

    // 화면을 덮는 2개의 삼각형 생성 및 셋팅
    planeData.push_back({ glm::vec2(-1.0f, -1.0f) });
    planeData.push_back({ glm::vec2( 1.0f, -1.0f) });
    planeData.push_back({ glm::vec2(-1.0f,  1.0f) });
    
    planeData.push_back({ glm::vec2(-1.0f,  1.0f) });
    planeData.push_back({ glm::vec2( 1.0f, -1.0f) });
    planeData.push_back({ glm::vec2( 1.0f,  1.0f) });

    // 1개 이고 위치만 전송하고 람다 안씀.
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

    if (!planeData.empty())
    {
        glBufferData(GL_ARRAY_BUFFER, planeData.size() * sizeof(SVertex), planeData.data(), GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, pos));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SDFs2D::DrawFunc(GLFWwindow* window)
{
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(progID);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GLint resLoc = glGetUniformLocation(progID, "iResolution");
    glUniform2f(resLoc, (float)width, (float)height);

    GLint timeLoc = glGetUniformLocation(progID, "iTime");
    glUniform1f(timeLoc, currentTime);

    // Quad 렌더링
    if (!planeData.empty())
    {
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, planeData.size());
    }

    glBindVertexArray(0);
}

void SDFs2D::UpdateFunc(GLFWwindow* window)
{
    currentTime = glfwGetTime();
}
