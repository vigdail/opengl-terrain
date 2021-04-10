#include "shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderModule::ShaderModule(const char *src, Type type)
    : id_{glCreateShader(static_cast<GLenum>(type))}, type_{type} {
  glShaderSource(id_, 1, &src, nullptr);
  glCompileShader(id_);
  checkCompileErrors(id_);
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

void ShaderModule::checkCompileErrors(uint32_t id) {
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    constexpr int max_log_size = 512;
    char info_log[max_log_size];
    glGetShaderInfoLog(id, max_log_size, nullptr,
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
  checkLinkingErrors(id_);
}
Shader::Shader(Shader &&other) noexcept : id_(std::exchange(other.id_, 0)) {}
Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    drop();
    std::swap(id_, other.id_);
  }

  return *this;
}
Shader::~Shader() { drop(); }
void Shader::drop() {
  glDeleteProgram(id_);
  id_ = 0;
}
void Shader::use() const { glUseProgram(id_); }
void Shader::setInt(const char *name, const int value) const {
  glUniform1i(glGetUniformLocation(id_, name), value);
}
void Shader::setFloat(const char *name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name), value);
}
void Shader::setVec2(const char *name, const glm::vec2 &value) const {
  glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}
void Shader::setVec3(const char *name, const glm::vec3 &value) const {
  glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}
void Shader::setVec4(const char *name, const glm::vec4 &value) const {
  glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z,
              value.w);
}
void Shader::setMat4(const char *name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false,
                     glm::value_ptr(value));
}

void Shader::checkLinkingErrors(uint32_t id) {
  int success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  GLchar info_log[1024];
  if (!success) {
    glGetProgramInfoLog(id, 1024, nullptr, static_cast<GLchar *>(info_log));
    std::cerr << "Shader linking error:" << info_log << std::endl;
    throw std::runtime_error("Shader linking error");
  }
}

ShaderBuilder &ShaderBuilder::load(std::string_view path,
                                   ShaderModule::Type type) {
  const std::string src = readFile(path);

  shaders_.emplace_back(src.c_str(), type);

  return *this;
}

Shader ShaderBuilder::build() const { return Shader{shaders_}; }
