#ifndef FORGE_VIEWPORTCAMERA_H
#define FORGE_VIEWPORTCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// passing the data to imgui
class ViewportCamera {
public:
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
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    ViewportCamera() {}

    ViewportCamera(float &camFov, float &camSpeed, glm::vec3 &camPos, glm::vec3 &camTarget, glm::vec3 &camDirection, glm::vec3 &camFront, glm::vec3 &camUp) :
            fov(camFov),
            speed(camSpeed),
            pos(camPos),
            target(camTarget),
            direction(glm::normalize(camPos - camTarget)),
            front(camFront),
            up(camUp)
    {}

    ViewportCamera(float d, float d1, glm::vec3 vec1, glm::vec3 vec2, glm::vec3 vec3, glm::vec3 vec4, glm::vec3 vec5) {
        this->fov = d;
        this->speed = d1;
        this->pos = vec1;
        this->target = vec2;
        this->direction = vec3;
        this->front = vec4;
        this->up = vec5;
    }
};


#endif //FORGE_VIEWPORTCAMERA_H
