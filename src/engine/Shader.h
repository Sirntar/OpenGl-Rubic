#ifndef CUBE_SRC_ENGINE_SHADER_H_
#define CUBE_SRC_ENGINE_SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#include "glad/gl.h"

namespace engine {
struct s_shader {
  std::string vShaderCode;
  std::string fShaderCode;
};

class Shader {
 protected:
  GLuint m_ID = 0;
  s_shader m_shader;

 public:
  Shader() = default;

  Shader(const char* vertexPath, const char* fragmentPath) {
    load(vertexPath, fragmentPath);
    compile();
  }

  ~Shader() {
    glDeleteProgram(m_ID);
  }

  void loadFromText(const std::string& vertexCode, const std::string& fragmentCode) {
    m_shader.vShaderCode = vertexCode;
    m_shader.fShaderCode = fragmentCode;
  }

  void load(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vShaderFile.open(vertexPath, std::ios::out);
      fShaderFile.open(fragmentPath, std::ios::out);

      vShaderFile.seekg(0, std::ios::beg);
      fShaderFile.seekg(0, std::ios::beg);

      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      m_shader.vShaderCode = vShaderStream.str();
      m_shader.fShaderCode = fShaderStream.str();

      vShaderFile.close();
      fShaderFile.close();
    } catch(const std::ofstream::failure& e) {
      std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << '\n' << e.code() << std::endl;
    }
  }

  void compile() {
    const char* vShaderCode = m_shader.vShaderCode.c_str();
    const char* fShaderCode = m_shader.fShaderCode.c_str();
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success) {
      glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
      std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if(!success) {
      glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
      std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
      std::cerr << "ERROR::SHADER::CREATE_PROGRAM_FAILED\n" << infoLog << glGetError() << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    m_shader.vShaderCode.clear();
    m_shader.fShaderCode.clear();
  }

  void use() const {
    glUseProgram(m_ID);
  }

  [[nodiscard]] GLuint get() const {
    return m_ID;
  }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
  }

  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
  }

  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
  }

  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
  }

  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
  }

  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
  }

  void setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
  }

  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setUniform(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setUniform(const std::string &name, const void * ptr) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, static_cast<const GLfloat *>(ptr));
  }

  void setUniform(const std::string &name, const glm::mat3 &mat) const {
      glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setUniform(const std::string &name, const glm::mat2 &mat) const {
      glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
};
}

#endif //CUBE_SRC_ENGINE_SHADER_H_
