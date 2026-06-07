#include "FieldOfView2D.h"

FieldOfView2D::FieldOfView2D()
{
    vertShaderFile = "Example/FieldOfView/shader.vert";
    fragShaderFile = "Example/FieldOfView/shader.frag";
}

void FieldOfView2D::CreateCircle(float radius, int VertCount)
{
    circleData.clear();

    float angle = (2.0f * M_PI) / (float)VertCount;
    for (int i = 0; i < VertCount; i++)
    {
        float theta = i * angle;

        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        circleData.push_back({ glm::vec2(x, y), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
    }
}

bool FieldOfView2D::CheckFOV(glm::vec2 point)
{
    glm::vec2 pos2D = glm::vec2(playerPos.x, playerPos.y);
    glm::vec2 point2D = glm::vec2(point.x, point.y);
    glm::vec2 forward2D = glm::normalize(glm::vec2(playerForward.x, playerForward.y));
    float halfFovCos = cosf((playerFOVAngle * 0.5f) * (M_PI / 180.f));

    glm::vec2 v = point2D - pos2D;
    float d = sqrtf(glm::dot(v, v));

    float pointDistSq = glm::dot(point2D, point2D);
    float radianSq = radian * radian;

    bool bDetected = false;
    if (pointDistSq <= radianSq) // 거리가 범위 안 이라면
    {
        glm::vec2 dir2D = v / d;

        float dot = glm::dot(forward2D, dir2D);
        if (dot >= halfFovCos)
        {
            bDetected = true;
        }
    }

    return bDetected;
}

void FieldOfView2D::InitFunc(GLFWwindow* window)
{
    Framework_ShaderLoader::InitFunc(window);

    auto BufferLamda = [](GLuint& vao, GLuint& vbo, const vector<FVertex>& data, GLenum usage)
        {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            if (!data.empty())
            {
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(FVertex), data.data(), usage);
            }

            // 위치 (0번)
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, pos));
            glEnableVertexAttribArray(0);

            // 색상 (1번)
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, color));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        };

    CreateCircle(radian, 50);
    BufferLamda(circleVAO, circleVBO, circleData, GL_STATIC_DRAW);

    for (int i = -3; i <= 3; i++) // -1, 0, 1 (중앙을 기준으로 예쁘게 퍼지게 배치)
    {
        for (int j = -3; j <= 3; j++)
        {
            if (i != 0 || j != 0)
            {
                float x = 0.25f * i;
                float y = 0.25f * j;
                float distSq = x * x + y * y;
                float radianSq = radian * radian;
                if (radianSq > distSq)
                {
                    basePoints.push_back(glm::vec4(x, y, 0.0f, 1.0f));
                }
            }
        }
    }

    BufferLamda(pointsVAO, pointsVBO, pointsData, GL_DYNAMIC_DRAW);
    BufferLamda(fovVAO, fovVBO, fovData, GL_DYNAMIC_DRAW);

    // 현재 시간 정의 
    lastTime = glfwGetTime();
}

void FieldOfView2D::DrawFunc(GLFWwindow* window)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(progID);

    // 원
    if (!circleData.empty())
    {
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_LINE_LOOP, 0, circleData.size());
    }
    

    // 시야각
    if (!fovData.empty())
    {
        glBindVertexArray(fovVAO);
        glDrawArrays(GL_LINES, 0, fovData.size());
    }

    // 감지된 점 
    if (!pointsData.empty())
    {
        glPointSize(5);
        glBindVertexArray(pointsVAO);
        glDrawArrays(GL_POINTS, 0, pointsData.size());
    }

    glBindVertexArray(0);
}

void FieldOfView2D::UpdateFunc(GLFWwindow* window)
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    playerRot += inputRight * playerRotSpeed * deltaTime; // 회전 값을 더함
    float rad = playerRot * M_PI / 180.f;
    playerForward = glm::vec3(cosf(rad), sinf(rad), 0.0f);

    
    glm::vec3 pos = playerPos + playerForward * inputForward * plyaerPosSpeed * deltaTime;
    float dot = glm::dot(pos, pos);
    float radianSq = radian * radian;
    if (dot <= radianSq)
    {
        playerPos = pos;
    }

    // 시야각 
    fovData.clear();

    float r = radian;
    float fovAngle = playerFOVAngle;

    glm::vec2 pos2D = glm::vec2(playerPos.x, playerPos.y);
    glm::vec4 basedRot = glm::vec4(1.f, 0.f, 0.f, 0.f);

    glm::mat4 leftRot = glm::rotate(glm::mat4(1.f), glm::radians(playerRot + fovAngle * 0.5f), glm::vec3(0.f, 0.f, 1.f));
    glm::vec2 leftDir = glm::vec2(leftRot * basedRot);

    glm::mat4 rightRot = glm::rotate(glm::mat4(1.f), glm::radians(playerRot - fovAngle * 0.5f), glm::vec3(0.f, 0.f, 1.f));
    glm::vec2 rightDir = glm::vec2(rightRot * basedRot);

    // Lamba -> 빗변, 근의 공식 으로 반환
    auto GetInterScction = [&](glm::vec2 pos, glm::vec2 dir, float r) -> glm::vec2
        {
            float a = powf(dir.x, 2) + powf(dir.y, 2);
            float b = 2 * pos.x * dir.x + 2 * pos.y * dir.y;
            float c = powf(pos.x, 2) + powf(pos.y, 2) - powf(r, 2);

            float d = powf(b, 2) - 4 * a * c;
            if (d >= 0.f) // 0 미만은 허수
            {
                float t = (-b + sqrtf(d)) / (2.f * a);

                return glm::vec2(pos.x + t * dir.x, pos.y + t * dir.y);
            }
            return pos;
        };

    glm::vec2 leftHit = GetInterScction(pos2D, leftDir, r);
    glm::vec2 rightHit = GetInterScction(pos2D, rightDir, r);

    glm::vec4 fovColor(0.0f, 1.0f, 0.0f, 1.0f); // 초록색
    fovData.push_back({ pos2D, fovColor });
    fovData.push_back({ leftHit, fovColor });

    fovData.push_back({ pos2D, fovColor });
    fovData.push_back({ rightHit, fovColor });

    // 시야각 데이터 업데이트(그래픽 메모리)
    if (!fovData.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, fovVBO);
        glBufferData(GL_ARRAY_BUFFER, fovData.size() * sizeof(FVertex), fovData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 감지 포인트 
    pointsData.clear();
    for (const auto& point : basePoints)
    {
        // 시야각 안에 들어온다면 데이터 셋팅
        if (CheckFOV(point))
        {
            pointsData.push_back({ point, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
        }
    }

    // 감지 포인트 그래픽 메모리 업데이트
    if (!pointsData.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glBufferData(GL_ARRAY_BUFFER, pointsData.size() * sizeof(FVertex), pointsData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void FieldOfView2D::KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    Framework_Based::KeyFuncEvent(window, key, scancode, action, mode);

    switch (key)
    {
        case GLFW_KEY_UP:   if (action == GLFW_PRESS) inputForward = 1.f; if (action == GLFW_RELEASE) inputForward = 0.f; break;
        case GLFW_KEY_DOWN: if (action == GLFW_PRESS) inputForward = -1.f; if (action == GLFW_RELEASE) inputForward = 0.f; break;
        case GLFW_KEY_RIGHT:if (action == GLFW_PRESS) inputRight = -1.f; if (action == GLFW_RELEASE) inputRight = 0.f; break;
        case GLFW_KEY_LEFT: if (action == GLFW_PRESS) inputRight = 1.f; if (action == GLFW_RELEASE) inputRight = 0.f; break;
    }
}
