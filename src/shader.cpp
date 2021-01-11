#include "shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const char *vertex_source, const char *fragment_source,
               const char *geometry_source) {
  uint vShader, fShader, gShader;

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vertex_source, NULL);
  glCompileShader(vShader);
  CheckCompileErrors(vShader, "VERTEX");

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fragment_source, NULL);
  glCompileShader(fShader);
  CheckCompileErrors(fShader, "FRAGMENT");

  if (geometry_source != nullptr) {
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gShader, 1, &geometry_source, NULL);
    glCompileShader(gShader);
    CheckCompileErrors(gShader, "GEOMETRY");
  }

  ID_ = glCreateProgram();
  glAttachShader(ID_, vShader);
  glAttachShader(ID_, fShader);
  if (geometry_source != nullptr) {
    glAttachShader(ID_, gShader);
  }
  glLinkProgram(ID_);
  CheckCompileErrors(ID_, "PROGRAM");

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  if (geometry_source != nullptr) {
    glDeleteShader(fShader);
  }
}

Shader &Shader::Use() {
  glUseProgram(ID_);
  return *this;
}

Shader &Shader::SetInt(const char *name, int value) {
  glUniform1i(glGetUniformLocation(ID_, name), value);
  return *this;
}
Shader &Shader::SetFloat(const char *name, float value) {
  glUniform1f(glGetUniformLocation(ID_, name), value);
  return *this;
}
Shader &Shader::SetVec3(const char *name, const glm::vec3 &value) {
  glUniform3f(glGetUniformLocation(ID_, name), value.x, value.y, value.z);
  return *this;
}
Shader &Shader::SetMat4(const char *name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(glGetUniformLocation(ID_, name), 1, false,
                     glm::value_ptr(matrix));
  return *this;
}

void Shader::CheckCompileErrors(uint object, const std::string &type) {
  int success = false;
  const uint kLogLength = 512;
  char infoLog[kLogLength] = {0};
  if (type != "PROGRAM") {
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
