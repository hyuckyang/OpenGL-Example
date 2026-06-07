#include "Parabola2D.h"

Parabola2D::Parabola2D()
{
    vertShaderFile = "Example/Parabola/shader.vert";
    fragShaderFile = "Example/Parabola/shader.frag";
}

void Parabola2D::InitFunc(GLFWwindow* window)
{
    Framework_ShaderLoader::InitFunc(window);

    // Depth 등 초기 설정( State ) 
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0.0F, 1.0F);
    glClearDepthf(1.0F);

    glEnable(GL_CULL_FACE); // 뒷면 렌더링 무시 (최적화)
    glFrontFace(GL_CCW);   // 반시계 방향이 앞면
    glCullFace(GL_BACK);

    playerPos = glm::vec2(50.0f, 100.0f); // 초반 위치 
    angle = 45.0f;
    power = 800.0f;
    gravity = -500.0f;
    groundY = 50.0f; // 바닥 기준

    // 
    prediction = CalcTrajectory(playerPos, angle, power, gravity, groundY, obstacles);

    // Buffer Setup
    auto BufferLamda = [](GLuint& vao, GLuint& vbo, const vector<PVertex>& data, GLenum usage)
        {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // 데이터가 존재 할 때만 그래픽 메모리에 저장
            if (!data.empty()) 
            {
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(PVertex), data.data(), usage);
            }

            // 위치
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PVertex), (void*)offsetof(PVertex, pos));
            glEnableVertexAttribArray(0);

            // 색
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PVertex), (void*)offsetof(PVertex, color));
            glEnableVertexAttribArray(1);

            // 바인딩 닫기 
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        };


    // 바닥 설정
    PVertex leftPoint  = { glm::vec2(0.0f, groundY), glm::vec4(0.3f, 0.8f, 0.3f, 1.0f) };
    PVertex rightPoint = { glm::vec2(800.0f, groundY), glm::vec4(0.3f, 0.8f, 0.3f, 1.0f) };

    groundData.push_back(leftPoint);
    groundData.push_back(rightPoint);

    BufferLamda(groundVAO, groundVBO, groundData, GL_STATIC_DRAW);

    // 장애물 설정
    obstacles.push_back(Obstacle(glm::vec2(400.0f, 200.0f), glm::vec2(20.0f, 75.0f)));
    obstacles.push_back(Obstacle(glm::vec2(700.0f, 300.0f), glm::vec2(20.0f, 125.0f)));

    // 장애물 그리기 위한 점점 데이터-> 삼각형 2개의 데이터 생성
    for (const auto& obs : obstacles) 
    {
        
        PVertex v1 = { glm::vec2(obs.xMin, obs.yMin), obs.color };
        PVertex v2 = { glm::vec2(obs.xMax, obs.yMin), obs.color };
        PVertex v3 = { glm::vec2(obs.xMax, obs.yMax), obs.color };
                                  
        PVertex v4 = { glm::vec2(obs.xMin, obs.yMin), obs.color };
        PVertex v5 = { glm::vec2(obs.xMax, obs.yMax), obs.color };
        PVertex v6 = { glm::vec2(obs.xMin, obs.yMax), obs.color };

        obsData.push_back(v1); obsData.push_back(v2); obsData.push_back(v3);
        obsData.push_back(v4); obsData.push_back(v5); obsData.push_back(v6);
    }

    BufferLamda(obsVAO, obsVBO, obsData, GL_STATIC_DRAW);

    // 궤적
    // 미리 계산 된 포인트(prediction) 을 기준으로 궤적 데이터 저장
    for (const auto& p : prediction.path)
    {
        PVertex v;
        v.pos = p;
        v.color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        trajData.push_back(v);
    }

    BufferLamda(trajVAO, trajVBO, trajData, GL_DYNAMIC_DRAW);

    // 발사체
    BufferLamda(projVAO, projVBO, projData, GL_DYNAMIC_DRAW);

    lastTime = glfwGetTime();
}

void Parabola2D::DrawFunc(GLFWwindow* window)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(progID);

    // 직교 투영 행렬 정의 
    glm::mat4 orthoMat = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    GLint matLoc = glGetUniformLocation(progID, "uMat");
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(orthoMat));

    // 바닥
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_LINES, 0, groundData.size()); 

    // 장애물
    glBindVertexArray(obsVAO);
    glDrawArrays(GL_TRIANGLES, 0, obsData.size());

    // 궤적
    glBindVertexArray(trajVAO);
    glDrawArrays(GL_LINE_STRIP, 0, trajData.size());

    // 발사체
    if (!projData.empty())
    {
        glBindVertexArray(projVAO);
        glDrawArrays(GL_TRIANGLES, 0, projData.size());
    }   
}

void Parabola2D::UpdateFunc(GLFWwindow* window)
{
    // 발사체 업데이트
    float currentTime = glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;

    // 업데이트 데이터 초기화 -> 다시 업데이트 데이터 push
    projData.clear();

    for (int i = projectiles.size() - 1; i >= 0; --i)
    {
        // 이동
        projectiles[i].velocity.y += gravity * dt;

        // 가운데 위치 및 다음 위치 계산
        glm::vec2 projCenter((projectiles[i].xMin + projectiles[i].xMax) / 2.0f, (projectiles[i].yMin + projectiles[i].yMax) / 2.0f);
        glm::vec2 nextPos = projCenter + projectiles[i].velocity * dt;

        // 충돌 Rect 업데이트
        projectiles[i].UpdatePosition(nextPos, glm::vec2(10.0f, 5.0f));

        bool isDestroyed = false;

        // 바닥 검사
        if (projectiles[i].yMin <= groundY)
        {
            isDestroyed = true;
        }
        // 화면 밖으로 나간다면 ( 800 )
        else if (projectiles[i].xMin > 800.0f || projectiles[i].xMax < 0.0f)
        {
            isDestroyed = true;
        }
        // 충돌 검사 
        else 
        {
            for (const auto& obs : obstacles) 
            {
                if (projectiles[i].IsOverlap(obs)) 
                {
                    isDestroyed = true;
                    break;
                }
            }
        }

        // 파괴 되었다면 삭제
        if (isDestroyed) 
        {
            projectiles.erase(projectiles.begin() + i);
            continue;
        }

        // 현재 방향에 따른 회전값 및 기타 셋팅
        float angleRad = atan2(projectiles[i].velocity.y, projectiles[i].velocity.x);
        glm::vec2 center((projectiles[i].xMin + projectiles[i].xMax) / 2.0f, (projectiles[i].yMin + projectiles[i].yMax) / 2.0f);
        glm::vec2 halfSize(10.0f, 5.0f);
        glm::vec4 color(1.0f, 1.0f, 0.0f, 1.0f);

        // 좌우상하 로컬좌표 지정
        glm::vec2 localCorners[4] = 
        {
            {-halfSize.x, -halfSize.y}, { halfSize.x, -halfSize.y},
            { halfSize.x,  halfSize.y}, {-halfSize.x,  halfSize.y}
        };

        PVertex verts[4];
        for (int j = 0; j < 4; ++j)
        {
            // 간단한 회전 공식 (cos, -sin, sin, cos) 로컬좌표 마다 지정
            float rx = localCorners[j].x * cos(angleRad) - localCorners[j].y * sin(angleRad);
            float ry = localCorners[j].x * sin(angleRad) + localCorners[j].y * cos(angleRad);
            verts[j] = { {center.x + rx, center.y + ry}, color };
        }

        // 삼각형 2개로 분활 및 데이터에 저장 
        projData.push_back(verts[0]); projData.push_back(verts[1]); projData.push_back(verts[2]);
        projData.push_back(verts[0]); projData.push_back(verts[2]); projData.push_back(verts[3]);
    }

    if (!projData.empty()) 
    {
        // 갱신되 데이터 그래픽 메모리에 덮어 쓰기
        glBindBuffer(GL_ARRAY_BUFFER, projVBO);
        glBufferData(GL_ARRAY_BUFFER, projData.size() * sizeof(PVertex), projData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Parabola2D::KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    Framework_Based::KeyFuncEvent(window, key, scancode, action, mode);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            glm::vec2 halfSize(10.0f, 5.0f); // 탄환 크기 (가로 10, 세로 5로 길쭉하게)
            Projectile proj(playerPos, halfSize);

            float rad = glm::radians(angle);
            proj.velocity = glm::vec2(power * cos(rad), power * sin(rad));

            projectiles.push_back(proj);
        }

        bool isChanged = false; 

        // 상하 -> 각도 , 좌우 -> 힘
        if (key == GLFW_KEY_UP)     { angle += 2.0f;    isChanged = true; }
        if (key == GLFW_KEY_DOWN)   { angle -= 2.0f;    isChanged = true; }
        if (key == GLFW_KEY_RIGHT)  { power += 50.0f;   isChanged = true; }
        if (key == GLFW_KEY_LEFT)   { power -= 50.0f;   isChanged = true; }

        // 조준점 변경시 
        // 궤적을 다시 계산 및 궤적 데이터 갱신
        if (isChanged) 
        {
            prediction = CalcTrajectory(playerPos, angle, power, gravity, groundY, obstacles);
            trajData.clear();   

            for (const auto& p : prediction.path) 
            {
                PVertex v;
                v.pos = p;
                v.color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); 
                trajData.push_back(v);
            }

            // 갱신된 데이터 덮어 쓰기 
            glBindBuffer(GL_ARRAY_BUFFER, trajVBO);
            
            glBufferData(GL_ARRAY_BUFFER, trajData.size() * sizeof(PVertex), trajData.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

PredictionResult Parabola2D::CalcTrajectory(glm::vec2 startPos, float angle, float power, float gravity, float groundY, const vector<Obstacle>& obstacles)
{
    PredictionResult result;
    result.hitObstacle = false; //

    const float dt = 0.0015f; // 프레임당 예상 시간
    float rad = glm::radians(angle);

    Projectile bullet(startPos, glm::vec2(5.f, 5.f)); // 위치, 사이즈(절반)
    bullet.velocity = glm::vec2(power * cosf(rad), power * sin(rad)); // 방향 및 회전
    
    // 최초 위치 
    glm::vec2 currentPos = startPos;
    result.path.push_back(currentPos);

    const int MAX_STEPS = 2000;

    for (int i = 0; i < MAX_STEPS; ++i) 
    {
        glm::vec2 nextPos = currentPos + bullet.velocity * dt; // 위치와 속도 갱신 
        bullet.velocity.y += gravity * dt; // 중력 적용
        bullet.UpdatePosition(nextPos, glm::vec2(5.0f, 5.0f));

        // 바닥 에 충돌(y 값이 바닥보다 낮다면) 시 종료
        if (nextPos.y <= groundY) 
        {
            result.hitPoint = glm::vec2(nextPos.x, groundY);
            result.path.push_back(result.hitPoint);
            result.hitObstacle = false;
            break;
        }

        // 장애물 충돌 검사 ( Rect 를 상속 받은 장애물과 발사체 검사)
        bool collided = false;
        for (const auto& obs : obstacles) 
        {
            if (bullet.IsOverlap(obs)) {
                result.hitPoint = currentPos;
                result.path.push_back(result.hitPoint);
                result.hitObstacle = true;
                collided = true;
                break;
            }
        }

        // 충돌 되었다면 종료
        if (collided) 
            break;

        currentPos = nextPos;

        // 경로(point) 저장
        result.path.push_back(currentPos);
    }

    if (result.path.size() == MAX_STEPS + 1)
    {
        result.hitPoint = currentPos; // 아무것도 없다면 가장 마지막 위치를 저장
    }

    return result;
}

