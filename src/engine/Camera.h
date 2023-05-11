#ifndef CUBE_SRC_ENGINE_CAMERA_H_
#define CUBE_SRC_ENGINE_CAMERA_H_

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

namespace engine {
  class Camera {
  protected:
    glm::mat4 m_model{1.0f}; // rotation
    glm::mat4 m_view{1.0f}; // position
    glm::mat4 m_projection{1.0f}; // perspective
  public:
    Camera() = default;

    [[nodiscard]] Camera& copy() {
      auto* c = new Camera();
      c->m_model = glm::mat4{m_model};
      c->m_view = glm::mat4{m_model};
      c->m_projection = glm::mat4{m_model};
      return *c;
    }

    void rotate(const glm::vec3& axis, float angle) {
      m_model = glm::rotate(m_model, glm::radians(angle), axis);
    }

    void resetRotation() {
      m_model = glm::mat4(1.0f);
    }

    void move(const glm::vec3& translation) {
      m_view = glm::translate(m_view, translation);
    }

    void setView(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
      m_view = glm::lookAt(position, target, up);
    }

    void resetTranslation() {
      m_view = glm::mat4(1.0f);
    }

    void setPerspective(float fov, float aspect, float near, float far) {
      m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void resetPerspective() {
      m_projection = glm::mat4(1.0f);
    }

    void apply(const Shader& shader) {
      shader.setUniform("model", glm::value_ptr(m_model));
      shader.setUniform("view", m_view);
      shader.setUniform("projection", m_projection);
    }

    [[nodiscard]] glm::vec3 getPosition() const {
      return m_view[3];
    }

    [[nodiscard]] glm::vec3 getRotation() const {
      return {m_model[0][0], m_model[1][1], m_model[2][2]};
    }

    [[nodiscard]] glm::vec3 getScale() const {
      return {m_model[0][0], m_model[1][1], m_model[2][2]};
    }

    [[nodiscard]] glm::mat4 getView() {
      return m_view;
    }
  };
}

#endif // CUBE_SRC_ENGINE_CAMERA_H_
