#include "SDFsRayMarching.h"

SDFsRayMarching::SDFsRayMarching()
{
    currentTime = 0.0f;
}

void SDFsRayMarching::InitFunc(GLFWwindow* window)
{
    // 
    auto loadedFiles = Utils::loadShaderFileFromDir("Example/SDFsRayMarching/");

    for (const auto& files : loadedFiles)
    {

        ShaderInfo info;
        info.vertFile = files.vertFile;
        info.fragFile = files.fragFile;
        shaderFiles.push_back(info);
    }

    // 삼각형 2개 
    planeData = 
    {
        {{-1.0f, -1.0f}},
        {{ 1.0f, -1.0f}},
        {{-1.0f,  1.0f}},
        {{-1.0f,  1.0f}},
        {{ 1.0f, -1.0f}},
        {{ 1.0f,  1.0f}}
    };

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeData.size() * sizeof(SRMVertex), planeData.data(), GL_STATIC_DRAW);

    //
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SRMVertex), (void*)0);

    glBindVertexArray(0);

    Framework_GridShaders::InitFunc(window);
}

void SDFsRayMarching::DrawFunc(GLFWwindow* window)
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 해상도 가져오기 
    int width = WIN_W, height = WIN_H;
    glfwGetFramebufferSize(window, &width, &height);

    // 쉐이더 없으면
    int count = shaderFiles.size();
    if (count == 0)
    {
        return;
    }

    int cols = ceil(sqrt(count));
    int rows = ceil((float)count / cols);

    int cellWidth = width / cols;
    int cellHeight = height / rows;

    glBindVertexArray(planeVAO);

    // 뷰포트 쪼개서 그리기
    for (int i = 0; i < count; ++i)
    {
        ShaderInfo& info = shaderFiles[i];

        // 컴파일 성공한 것만
        if (!info.bCompiled)
        {
            continue;
        }

        glUseProgram(info.progID);

        // col , row  id
        int colIdx = i % cols;
        int rowIdx = i / cols;

        int x = colIdx * cellWidth;
        int y = (rows - 1 - rowIdx) * cellHeight;
        glViewport(x, y, cellWidth, cellHeight);

        // 기본적인 해상도 (cell 크기) 넘겨주기
        GLint resLoc = glGetUniformLocation(info.progID, "iRes");
        glUniform2f(resLoc, (float)cellWidth, (float)cellHeight);

        GLint timeLoc = glGetUniformLocation(info.progID, "iTime");
        glUniform1f(timeLoc, currentTime);

        // 실제 그리기
        glDrawArrays(GL_TRIANGLES, 0, planeData.size());
    }

    // 전체 크기로 복구
    glViewport(0, 0, width, height);
    glBindVertexArray(0);
}

void SDFsRayMarching::UpdateFunc(GLFWwindow* window)
{
    currentTime = glfwGetTime();
}
