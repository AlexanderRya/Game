#ifndef GAME_CAMERA_HPP
#define GAME_CAMERA_HPP

#include <game/Forwards.hpp>
#include <game/Types.hpp>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace game::core::components {
    struct CameraData {
        glm::mat4 pvmat;
    };

    struct Camera {
        static constexpr f32 speed = 0.6f;
        static constexpr f32 sensitivity = 5.0e-2f;

        f64 yaw;
        f64 pitch;

        glm::vec3 prec_pos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cam_pos = glm::vec3(0.0f, 1.0f, 3.0f);
        glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cam_right = glm::vec3();
        glm::vec3 cam_wup = glm::vec3(0.0f, 1.0f, 0.0f);

        Camera();
        void move(const Window&);
        [[nodiscard]] glm::mat4 get_view_mat() const;
        void process(double xoffset, double yoffset);
        void update();
    };
} // namespace game::core::components

#endif //GAME_CAMERA_HPP
