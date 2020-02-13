#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cstdint>
#include <engine/gameObject.hpp>

class Camera: public GameObject{
    public:
        Camera(uint16_t UID);
        Camera(uint16_t UID, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        ~Camera();
};
#endif
