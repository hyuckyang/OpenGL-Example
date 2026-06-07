#include "FieldOfView2D.h"

FieldOfView2D::FieldOfView2D()
{
    vertShaderFile = "Example/FieldOfView/shader.vert";
    fragShaderFile = "Example/FieldOfView/shader.frag";
}

void FieldOfView2D::CreateCircle(float radius, int VertCount)
{
    vertexCircle.clear();

    float angle = (2.0f * M_PI) / (float)VertCount;
    for (int i = 0; i < VertCount; i++)
    {
        float theta = i * angle;

        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        vertexCircle.push_back(glm::vec4(x, y, 0.f, 1.f));
    }
}

bool FieldOfView2D::CheckFOV(glm::vec4 point)
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
        //glm::vec2 dir2D = glm::normalize(v);
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

    CreateCircle(radian, 50);

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
                    vertexPoints.push_back(glm::vec4(x, y, 0.0f, 1.0f));
                }
            }
        }
    }
}

void FieldOfView2D::DrawFunc(GLFWwindow* window)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint locPos = glGetAttribLocation(progID, "aPos");
    GLuint locColor = glGetAttribLocation(progID, "aColor");

    // 공통 세팅: 속성 활성화
    glEnableVertexAttribArray(locPos);
    if (locColor != -1) glDisableVertexAttribArray(locColor);

    // ==========================================
    // 1. 하얀색 테두리 원 그리기
    // ==========================================
    glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertexCircle[0]));
    glVertexAttrib4f(locColor, 1.0f, 1.0f, 1.0f, 1.0f); // 하얀색
    glDrawArrays(GL_LINE_LOOP, 0, vertexCircle.size());

    // ==========================================
    // 2. 초록색 시야각(FOV) 선 그리기
    // ==========================================
    // 포인터를 FOV 배열로 바꿔치기 합니다.
    glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertexFOV[0]));
    glVertexAttrib4f(locColor, 0.0f, 1.0f, 0.0f, 1.0f); // 초록색 (R:0, G:1, B:0)
    glDrawArrays(GL_LINES, 0, vertexFOV.size()); // GL_LINES 모드 사용

    glPointSize(5);
    for (auto point : vertexPoints)
    {
        if (CheckFOV(point))
        {
            glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(point));
            glDrawArrays(GL_POINTS, 0, 1); // GL_POINT 모드 사용
        }

        //// 원래 업데이트 에서 진행되어야 하나 테스트 형식으로 여기서 검사 및 정의
        //if (CheckFOV(point))
        //{
        //    glVertexAttrib4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f);
        //   
        //}
        //else
        //{
        //    glVertexAttrib4f(locColor, 0.0f, 0.0f, 0.0f, 0.0f);
        //}
        //glDrawArrays(GL_POINTS, 0, 1); // GL_POINT 모드 사용

    }
}

void FieldOfView2D::UpdateFunc(GLFWwindow* window)
{
    system_clock::time_point curTime = system_clock::now();
    milliseconds elapsedTimeMSEC = duration_cast<milliseconds>(curTime - lastTime);
    deltaTime = (elapsedTimeMSEC.count() / 1000.0f);
    lastTime = curTime; //

    playerRot += inputRight * playerRotSpeed * deltaTime; // 회전 값을 더함
    float rad = playerRot * M_PI / 180.f;
    playerForward = glm::vec3(cosf(rad), sinf(rad), 0.0f);

    // line
    glm::vec3 pos = playerPos + playerForward * inputForward * plyaerPosSpeed * deltaTime;
    float dot = glm::dot(pos, pos);
    float radianSq = radian * radian;
    if (dot <= radianSq)
    {
        playerPos = pos;
    }

    //
    vertexFOV.clear();

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

    vertexFOV.push_back(glm::vec4(pos2D.x, pos2D.y, 0.0f, 1.0f));
    vertexFOV.push_back(glm::vec4(leftHit.x, leftHit.y, 0.0f, 1.0f));

    vertexFOV.push_back(glm::vec4(pos2D.x, pos2D.y, 0.0f, 1.0f));
    vertexFOV.push_back(glm::vec4(rightHit.x, rightHit.y, 0.0f, 1.0f));

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
