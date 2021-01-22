#include "shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

Shader::Shader() { ID_ = glCreateProgram(); }
Shader::Shader(Shader &&other) : ID_(other.ID_) { other.ID_ = 0; }
Shader &Shader::operator=(Shader &&other) {
  if (this != &other) {
    Delete();
    std::swap(ID_, other.ID_);
  }

  return *this;
}
Shader::~Shader() { Delete(); }
void Shader::Delete() {
  glDeleteProgram(ID_);
  ID_ = 0;
}
void Shader::Use() { glUseProgram(ID_); }
void Shader::AttachShader(const unsigned int type, const char *source) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  CheckCompileErrors(shader, type);

  glAttachShader(ID_, shader);
  // @TODO: Is it correct to remove attached shader before linking
  glDeleteShader(shader);
}
void Shader::Link() {
  glLinkProgram(ID_);
  CheckCompileErrors(ID_, GL_PROGRAM);
}
void Shader::SetInt(const char *name, const int value) {
  glUniform1f(glGetUniformLocation(ID_, name), value);
}
void Shader::SetFloat(const char *name, float value) {
  glUniform1f(glGetUniformLocation(ID_, name), value);
}
void Shader::SetVec3(const char *name, const glm::vec3 &value) {
  glUniform3f(glGetUniformLocation(ID_, name), value.x, value.y, value.z);
}
void Shader::SetMat4(const char *name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(glGetUniformLocation(ID_, name), 1, false,
                     glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(uint object, const unsigned int type) {
  int success = false;
  const uint kLogLength = 512;
  char infoLog[kLogLength] = {0};
  if (type != GL_PROGRAM) {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, kLogLength, NULL, infoLog);
      std::cout << "ERROR::SHADER: Compile-time error, type: " << type << "\n"
                << infoLog << "\n -- ------------------------------------ -- "
                << std::endl;
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(object, kLogLength, NULL, infoLog);
      std::cout << "ERROR::SHADER: Linking error: \n"
                << infoLog << "\n -- --------------------------------- --"
                << std::endl;
    }
  }
}
