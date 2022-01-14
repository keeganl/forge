#ifndef FORGE_CAMERA_H
#define FORGE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// passing the data to imgui
struct Camera {
    float fov{45.0f};
    float speed{10.0f};
    bool orthographic = false;
    float sensitivity = 0.5f;
    glm::vec3 initialPos = glm::vec3(1.0f, 2.0f, 3.0f);
    glm::vec3 pos{glm::vec3(1.0f, 2.0f, 3.0f)};
    glm::vec3 target{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 direction{glm::normalize(glm::vec3(0.0f, 0.0f, 3.0f) - glm::vec3(0.0f, 0.0f, 0.0f))};
    glm::vec3 front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 up{glm::vec3(0.0f, 1.0f,  0.0f)};
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    Camera() {}

    Camera(float &camFov, float &camSpeed, glm::vec3 &camPos, glm::vec3 &camTarget, glm::vec3 &camDirection, glm::vec3 &camFront, glm::vec3 &camUp) :
            fov(camFov),
            speed(camSpeed),
            pos(camPos),
            target(camTarget),
            direction(glm::normalize(camPos - camTarget)),
            front(camFront),
            up(camUp)
    {}

    Camera(float d, float d1, glm::vec3 vec1, glm::vec3 vec2, glm::vec3 vec3, glm::vec3 vec4, glm::vec3 vec5) {
        this->fov = d;
        this->speed = d1;
        this->pos = vec1;
        this->target = vec2;
        this->direction = vec3;
        this->front = vec4;
        this->up = vec5;
    }

//    float& getFov();
//    void setFov(float &newFov);
//    float& getSpeed();
//    void setSpeed(float &newSpeed);
//    glm::vec3& getPosition();
//    void setPosition(glm::vec3 &newPos);
//    glm::vec3& getTarget();
//    void setTarget(glm::vec3 &newTarget);
//    glm::vec3& getDirection();
//    void setDirection(glm::vec3 &newDirection);
//    glm::vec3& getFront();
//    void setFront(glm::vec3 &newFront);
//    glm::vec3& getUp();
//    void setUp(glm::vec3 &newUp);
};


#endif //FORGE_CAMERA_H
