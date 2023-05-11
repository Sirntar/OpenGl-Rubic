//
// Created by tymot on 31.01.2023.
//

#ifndef CUBE_SRC_ENGINE_PRIMITIVES_RUBIKATOMCUBE_H_
#define CUBE_SRC_ENGINE_PRIMITIVES_RUBIKATOMCUBE_H_

#include <vector>

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Camera.h"
#include "../Shader.h"

namespace engine::primitives {
  class RubikAtomCube {
  protected:
    std::vector<GLint> m_indices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_colors;

    std::vector<GLuint> m_vao{0}, m_vbo{0}, m_ebo{0}, m_nbo{0}, m_cbo{0};

    Camera m_camera;
    Shader* m_shader = nullptr;

    glm::vec3 m_dimensions{1.0f, 1.0f, 1.0f};
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};

    inline void triangulate(glm::vec3 pos, glm::vec3 dim) {
      m_vertices = {
          // front
          {pos.x - dim.x / 2.0f, pos.y - dim.y / 2.0f,
           pos.z + dim.z / 2.0f}, // front-left
          {pos.x + dim.x / 2.0f, pos.y - dim.y / 2.0f,
           pos.z + dim.z / 2.0f}, // front-right
          {pos.x + dim.x / 2.0f, pos.y + dim.y / 2.0f,
           pos.z + dim.z / 2.0f}, // front-top-right
          {pos.x - dim.x / 2.0f, pos.y + dim.y / 2.0f,
           pos.z + dim.z / 2.0f}, // front-top-left

          // back
          {pos.x - dim.x / 2.0f, pos.y - dim.y / 2.0f,
           pos.z - dim.z / 2.0f}, // back-left
          {pos.x + dim.x / 2.0f, pos.y - dim.y / 2.0f,
           pos.z - dim.z / 2.0f}, // back-right
          {pos.x + dim.x / 2.0f, pos.y + dim.y / 2.0f,
           pos.z - dim.z / 2.0f}, // back-top-right
          {pos.x - dim.x / 2.0f, pos.y + dim.y / 2.0f,
           pos.z - dim.z / 2.0f}, // back-top-left
      };

      // vertices order for creating triangles
      // there in total 12 triangles - 2 triangles per face
      // it means GPU will draw 36 vertices
      m_indices = {
          // front
          0, 1, 2, 2, 3, 0,
          // right
          1, 5, 6, 6, 2, 1,
          // back
          7, 6, 5, 5, 4, 7,
          // left
          0, 3, 7, 7, 4, 0,
          // top
          3, 2, 6, 6, 7, 3,
          // bottom
          0, 4, 5, 5, 1, 0
      };
    };

    std::vector<GLint> m_in{0, 1, 2, 2, 3, 0};

    void setupMesh() {
      for (int i = 0; i < 6; ++i) {
        std::vector<glm::vec3> v;

        v.push_back(m_vertices[m_indices[i * 6 + 0]]);
        v.push_back(m_vertices[m_indices[i * 6 + 1]]);
        v.push_back(m_vertices[m_indices[i * 6 + 2]]);
        v.push_back(m_vertices[m_indices[i * 6 + 4]]);

        glGenVertexArrays(1, &m_vao[i]);
        glBindVertexArray(m_vao[i]);

        // vertices
        glGenBuffers(1, &m_vbo[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[i]);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3),
                     v.data(), GL_STATIC_DRAW);

        // triangles indices (vertex order)
        glGenBuffers(1, &m_ebo[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint),
                     m_in.data(), GL_STATIC_DRAW);


        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              nullptr);

        /*// colors
        std::vector<glm::vec3> c = {
            {0.33f, 0.2f, 0.1f},
            {0.33f, 0.2f, 0.1f},
            {0.33f, 0.2f, 0.1f},
            {0.33f, 0.2f, 0.1f},
            {0.33f, 0.2f, 0.1f},
            {0.33f, 0.2f, 0.1f},
        };
        glGenBuffers(1, &m_cbo[i]);
        glEnableVertexAttribArray(1);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &c,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              nullptr);*/

        glGenBuffers(1, &m_cbo[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo[i]);

        if (!m_colors.empty()) {
          std::vector<glm::vec3> color = {
              m_colors[i],
              m_colors[i],
              m_colors[i],
              m_colors[i],
              m_colors[i],
              m_colors[i],
          };
          glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), color.data(),
                       GL_STATIC_DRAW);
        }
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              nullptr);
        glEnableVertexAttribArray(1);
      }
    }

    void update() {
      for (int i = 0; i < 6; ++i) {
        std::vector<glm::vec3> color = {
          m_colors[i],
          m_colors[i],
          m_colors[i],
          m_colors[i],
          m_colors[i],
          m_colors[i],
        };

        glBindVertexArray(m_vao[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo[i]);
        glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), color.data(),
                     GL_STATIC_DRAW);
      }
      /*std::vector<glm::vec3> c = {
          {0.33f, 0.2f, 0.1f},
          {0.33f, 0.2f, 0.1f},
          {0.33f, 0.2f, 0.1f},
          {0.33f, 0.2f, 0.1f},
          {0.33f, 0.2f, 0.1f},
          {0.33f, 0.2f, 0.1f},
      };
      glBindVertexArray(m_vao[0]);
      glGenBuffers(1, &m_cbo[0]);
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, m_cbo[0]);
      glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &c,
                   GL_STATIC_DRAW);*/
    }
  public:
    explicit RubikAtomCube(const glm::vec3& center, const glm::vec3& dimensions)
        : m_dimensions(dimensions), m_position(center) {
      m_vao.reserve(6);
      m_vbo.reserve(6);
      m_ebo.reserve(6);
      m_nbo.reserve(6);
      m_cbo.reserve(6);
      triangulate(m_position, m_dimensions);
      setupMesh();
    }
    ~RubikAtomCube() {
      for (int i = 0; i < 6; ++i) {
        glDeleteVertexArrays(1, &m_vao[i]);
        glDeleteBuffers(1, &m_vbo[i]);
        glDeleteBuffers(1, &m_ebo[i]);
        glDeleteBuffers(1, &m_nbo[i]);
        glDeleteBuffers(1, &m_cbo[i]);
      }
    }

    void addColor(glm::vec3 color) {
      m_colors.push_back(color);
      update();
    }

    void setColors(std::vector<glm::vec3> colors) {
      m_colors = std::move(colors);
      update();
    }

    std::vector<glm::vec3>* getColors() {
      return &m_colors;
    }

    inline virtual void draw() {
      glBindVertexArray(m_vao[0]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glBindVertexArray(m_vao[1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glBindVertexArray(m_vao[2]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glBindVertexArray(m_vao[3]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glBindVertexArray(m_vao[4]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glBindVertexArray(m_vao[5]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);
    }

    void setCamera(const Camera& camera) {
      m_camera = camera;
    }

    void setViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
      m_camera.setView(position, target, up);
    }

    void rotate(const glm::vec3& axis, float angle) {
      m_camera.rotate(axis, angle);
    }

    void rotateXYZ(glm::vec3 axis, float angle) {
      glm::vec3 center(0.0f);
      for (auto v : m_vertices)
      {
        center += v;
      }
      center /= m_vertices.size();
      m_position = center;

      glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

      // Apply the rotation to each vertex
      for (auto & v : m_vertices)
      {
        glm::vec4 vertex = glm::vec4(v, 1.0f);
        vertex = rotationMatrix * vertex;
        v = glm::vec3(vertex.x, vertex.y, vertex.z);
      }
      setupMesh();
    }

    void setPerspective(float fov, float aspect, float near, float far) {
      m_camera.setPerspective(fov, aspect, near, far);
    }

    void applyCamera(Shader& shader) {
      m_camera.apply(shader);
    }

    void createShader(const char* vertexShader, const char* fragmentShader) {
      m_shader = new Shader(vertexShader, fragmentShader);
    }

    inline void useShader() {
      m_shader->use();
    }

    inline void useCamera() {
      useShader();
      m_camera.apply(*m_shader);
    }

    void move(const glm::vec3& offset) {
      m_position += offset;
      triangulate(m_position, m_dimensions);
      setupMesh();
    }

    void scale(const glm::vec3& scale) {
      m_dimensions *= scale;
      triangulate(m_position, m_dimensions);
      setupMesh();
    }

    void setDimensions(const glm::vec3& dimensions) {
      m_dimensions = dimensions;
      triangulate(m_position, m_dimensions);
      setupMesh();
    }

    void setPosition(const glm::vec3& position) {
      m_position = position;
      triangulate(m_position, m_dimensions);
      setupMesh();
    }

    [[nodiscard]] glm::vec3 getDimensions() const {
      return m_dimensions;
    }

    [[nodiscard]] float getSize() const {
      return m_dimensions.x;
    }

    [[nodiscard]] glm::vec3 getPosition() const {
      return m_position;
    }
  };
}

#endif // CUBE_SRC_ENGINE_PRIMITIVES_RUBIKATOMCUBE_H_
