#include <chrono>
#include <iostream>
#include <thread>

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"

#include "callbacks.h"

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1080, 720, "rubik", nullptr, nullptr);
  if (!window) {
    std::cerr << "GLAD::ERROR: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwWindowHint(GLFW_SAMPLES, 4);

  if (!gladLoadGL(glfwGetProcAddress))
  {
    std::cerr << "GLAD::ERROR: Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_MULTISAMPLE);

  struct s_rubik rubik = make_rubik();

  int maxFrames = 144;

  bool canColorChange = true;
  int count = 0;

  auto lastTime = std::chrono::high_resolution_clock::now();
  double frameRate = 1000.0 / maxFrames;

  int rotateCounter = 0;
  int pressCounter = 0;

  bool is_rotating = false;
  bool is_reversed = false;

  RubikRoteGroup group = FRONT;

  while (!glfwWindowShouldClose(window)) {
    auto timePoint = std::chrono::high_resolution_clock::now();

    // input/process animation
    // rotate
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      for (auto& cube : rubik.cubes)
        cube->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      for (auto& cube : rubik.cubes)
        cube->rotate(glm::vec3(0.0f, 1.0f, 0.0f), -1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      for (auto& cube : rubik.cubes)
        cube->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      for (auto& cube : rubik.cubes)
        cube->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = FRONT;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = BACK;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = RIGHT;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = LEFT;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = TOP;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = BOTTOM;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = CENTER_F;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = CENTER_R;
        is_rotating = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
      if (!is_rotating) {
        rotateCounter = 90;
        group = CENTER_T;
        is_rotating = true;
      }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      if (pressCounter <= 0 && !is_rotating) {
        is_reversed = !is_reversed;
        pressCounter = 60;
      }
    }

    if (rotateCounter > 0) {
      rotate_rubik(&rubik, group, is_reversed);
      rotateCounter -= 1;
    } else {
      is_rotating = false;
    }

    // set perspective
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    for (auto& cube : rubik.cubes) {
      cube->setPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    }

    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw cubes
    for (auto cube : rubik.cubes) {
      cube->useCamera();
      cube->draw();
    }

    if (canColorChange)
      canColorChange = false;

    glfwSwapBuffers(window);
    glfwPollEvents();

    pressCounter--;
    // delay rendering to get set number of fps
    auto diff = std::chrono::duration<double>(timePoint - lastTime).count();
    if (diff < frameRate) {
      std::this_thread::sleep_for(std::chrono::milliseconds((int)(frameRate - diff)));
      // delay color change
      if(count++ > 30) {
        canColorChange = true;
        count = 0;
      }
    }
    lastTime = timePoint;
  }

  // clean up
  for (auto& cube : rubik.cubes)
    delete cube;
  glfwTerminate();
  return 0;
}