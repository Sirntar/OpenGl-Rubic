#ifndef CUBE_SRC_CALLBACKS_H_
#define CUBE_SRC_CALLBACKS_H_

#include <chrono>
#include <cmath>

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"

#include "engine/Camera.h"
#include "engine/primitives/RubikAtomCube.h"

struct s_rubik {
  std::vector<engine::primitives::RubikAtomCube*> cubes;
  std::vector<int> front, back, right, left, top, bottom, center_f, center_r, center_t;

  int rotation_counter = 0;
};

struct s_rubik make_rubik() {
  struct s_rubik rubik;

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      for (int z = -1; z <= 1; z++) {
        if (x == 0 && y == 0 && z == 0)
          continue;

        auto *cube = new engine::primitives::RubikAtomCube(glm::vec3{x, y, z} *
                                                  glm::vec3(1 * 1.05f),
                                                  {1, 1, 1});
        engine::Camera camera;
        camera.move(glm::vec3(0.0f, 0.0f, 10.0f));
        camera.rotate(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f);
        camera.rotate(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f);

        camera.setView(glm::vec3(0.0f, 0.0f, 10.0f),
                       glm::vec3(0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));

        cube->setCamera(camera);
        cube->createShader("../src/shaders/rubikVertex.glsl",
                           "../src/shaders/rubikFragment.glsl");

        std::vector<glm::vec3> colors{
            {1.0f, 1.0f, 1.0f},     // 0 - front
            {0.68f, 0.07f, 0.08f},  // 1 - right
            {0.05f, 0.28f, 0.67f},  // 2 - back

            {1.0f, 0.34f, 0.14f},   // 3 - left
            {0.1f, 0.61f, 0.3f},    // 4 - top
            {1.0f, 0.84f, 0.18f},   // 5 - bottom
        };

        if (y == -1) {
          colors[4] = {0.0f, 0.0f, 0.0f};
          rubik.bottom.push_back((int)rubik.cubes.size());
        } else if (y == 0) {
          colors[4] = {0.0f, 0.0f, 0.0f};
          colors[5] = {0.0f, 0.0f, 0.0f};
          rubik.center_r.push_back((int)rubik.cubes.size());
        } else {
          colors[5] = {0.0f, 0.0f, 0.0f};
          rubik.top.push_back((int)rubik.cubes.size());
        }

        if (x == -1) {
          colors[1] = {0.0f, 0.0f, 0.0f};
          rubik.left.push_back((int)rubik.cubes.size());
        } else if (x == 0){
          colors[3] = {0.0f, 0.0f, 0.0f};
          colors[1] = {0.0f, 0.0f, 0.0f};
          rubik.center_f.push_back((int)rubik.cubes.size());
        } else {
          colors[3] = {0.0f, 0.0f, 0.0f};
          rubik.right.push_back((int)rubik.cubes.size());
        }

        if (z == -1) {
          colors[0] = {0.0f, 0.0f, 0.0f};
          rubik.front.push_back((int)rubik.cubes.size());
        } else if (z == 0){
          colors[0] = {0.0f, 0.0f, 0.0f};
          colors[2] = {0.0f, 0.0f, 0.0f};
          rubik.center_t.push_back((int)rubik.cubes.size());
        } else {
          colors[2] = {0.0f, 0.0f, 0.0f};
          rubik.back.push_back((int)rubik.cubes.size());
        }

        cube->setColors(colors);

        rubik.cubes.push_back(cube);
      }
    }
  }
  return rubik;
}

enum RubikRoteGroup {
  FRONT = 0,
  BACK = 1,
  RIGHT = 2,
  LEFT = 3,
  TOP = 4,
  BOTTOM = 5,
  CENTER_F = 6,
  CENTER_R = 7,
  CENTER_T = 8,
};

void rotate_rubik(struct s_rubik *rubik, enum RubikRoteGroup rotate_group,
                  bool negative = false) {
  glm::vec3 rotate_point[] = {
      {0, 0, 1},
      {0, 0, -1},
      {1, 0, 0},
      {-1, 0, 0},
      {0, 1, 0},
      {0, -1, 0},
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
  };
  std::vector<int> indexes;
  switch (rotate_group) {
    case FRONT:
      indexes = rubik->front;
      break;
    case BACK:
      indexes = rubik->back;
      break;
    case RIGHT:
      indexes = rubik->right;
      break;
    case LEFT:
      indexes = rubik->left;
      break;
    case TOP:
      indexes = rubik->top;
      break;
    case BOTTOM:
      indexes = rubik->bottom;
      break;
    case CENTER_F:
      indexes = rubik->center_f;
      break;
    case CENTER_R:
      indexes = rubik->center_r;
      break;
    case CENTER_T:
      indexes = rubik->center_t;
      break;
  }

  if (++rubik->rotation_counter >= 90) {
    rubik->rotation_counter = 0;
    // recalculate groups
    rubik->front.clear();
    rubik->back.clear();
    rubik->right.clear();
    rubik->left.clear();
    rubik->top.clear();
    rubik->bottom.clear();
    rubik->center_f.clear();
    rubik->center_r.clear();
    rubik->center_t.clear();

    int i = 0;
    for (auto& cube : rubik->cubes) {
      glm::vec3 pos = cube->getPosition();
      if ((int)pos.y == -1) {
        rubik->bottom.push_back(i);
      } else if ((int)pos.y == 0) {
        rubik->center_r.push_back(i);
      } else {
        rubik->top.push_back(i);
      }

      if ((int)pos.x == -1) {
        rubik->left.push_back(i);
      } else if ((int)pos.x == 0){
        rubik->center_f.push_back(i);
      } else {
        rubik->right.push_back(i);
      }

      if ((int)pos.z == -1) {
        rubik->front.push_back(i);
      } else if ((int)pos.z == 0){
        rubik->center_t.push_back(i);
      } else {
        rubik->back.push_back(i);
      }
      i++;
    }
  }

  for (int ix : indexes) {
    rubik->cubes[ix]->rotateXYZ(rotate_point[rotate_group], negative ? -1.0f : 1.0f);
  }
}

void calc_scale(engine::primitives::RubikAtomCube *cube, float scale) {
  glm::vec3 dim = cube->getDimensions();
  glm::vec3 pos = cube->getPosition() / (dim * 1.5f);
  dim *= scale;
  cube->setDimensions(dim);
  cube->setPosition(pos * (dim * 1.5f));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

double _xoffset = 0.0;
double _yoffset = 0.0;
bool _scroll = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  _xoffset = xoffset;
  _yoffset = yoffset;
  _scroll = true;
}

#endif // CUBE_SRC_CALLBACKS_H_
