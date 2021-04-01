#include "shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderModule::ShaderModule(const char *src, Type type)
    : id_{glCreateShader(static_cast<GLenum>(type))}, type_{type} {
  glShaderSource(id_, 1, &src, nullptr);
  glCompileShader(id_);
  CheckCompileErrors(id_);
}

ShaderModule::~ShaderModule() {
  glDeleteShader(id_);
  id_ = 0;
}

ShaderModule::ShaderModule(ShaderModule &&other) noexcept
    : id_{other.id_}, type_{other.type_} {
  other.id_ = 0;
}

ShaderModule &ShaderModule::operator=(ShaderModule &&other) noexcept {
  std::swap(id_, other.id_);
  std::swap(type_, other.type_);
  return *this;
}

void ShaderModule::CheckCompileErrors(uint32_t id) {
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    constexpr int MAX_LOG_SIZE = 512;
    char info_log[MAX_LOG_SIZE];
    glGetShaderInfoLog(id, MAX_LOG_SIZE, nullptr,
                       static_cast<char *>(info_log));
    std::cerr << "ShaderModule compilation error: " << id << " " << info_log
              << '\n';
    throw std::runtime_error("ShaderModule compilation error for shader");
  }
}

Shader::Shader(const std::vector<ShaderModule> &shaders)
    : id_{glCreateProgram()} {
  for (const auto &shader : shaders) {
    glAttachShader(id_, shader.id_);
  }
  glLinkProgram(id_);
  CheckLinkingErrors(id_);
}
Shader::Shader(Shader &&other) noexcept : id_(std::exchange(other.id_, 0)) {}
Shader &Shader::operator=(Shader &&other) {
  if (this != &other) {
    Delete();
    std::swap(id_, other.id_);
  }

  return *this;
}
Shader::~Shader() { Delete(); }
void Shader::Delete() {
  glDeleteProgram(id_);
  id_ = 0;
}
void Shader::Use() { glUseProgram(id_); }
void Shader::SetInt(const char *name, const int value) {
  glUniform1i(glGetUniformLocation(id_, name), value);
}
void Shader::SetFloat(const char *name, float value) {
  glUniform1f(glGetUniformLocation(id_, name), value);
}
void Shader::SetVec3(const char *name, const glm::vec3 &value) {
  glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}
void Shader::SetVec4(const char *name, const glm::vec4 &value) {
  glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z,
              value.w);
}
void Shader::SetMat4(const char *name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false,
                     glm::value_ptr(matrix));
}

void Shader::CheckLinkingErrors(uint32_t id) {
  int success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  GLchar info_log[1024];
  if (!success) {
    glGetProgramInfoLog(id, 1024, nullptr, static_cast<GLchar *>(info_log));
    std::cerr << "Shader linking error:" << info_log << std::endl;
    throw std::runtime_error("Shader linking error");
  }
}

ShaderBuilder &ShaderBuilder::Load(std::string_view path,
                                   ShaderModule::Type type) {
  const std::string src = ReadFile(path);

  shaders_.push_back(ShaderModule(src.c_str(), type));

  return *this;
}

Shader ShaderBuilder::Build() const { return Shader{shaders_}; }
