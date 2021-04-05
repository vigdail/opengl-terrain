#pragma once

#include <string>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "util.h"

class Shader;

class ShaderModule {
  friend Shader;

 public:
  enum class Type {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    COMPUTE = GL_COMPUTE_SHADER,
  };

  ShaderModule(const char *src, Type type);
  ~ShaderModule();
  ShaderModule(ShaderModule &&other) noexcept;
  ShaderModule(const ShaderModule &) = delete;
  ShaderModule &operator=(ShaderModule &&other) noexcept;
  ShaderModule &operator=(const ShaderModule &) = delete;

 private:
  uint32_t id_;
  Type type_;

 private:
  static void checkCompileErrors(uint32_t id);
};

class Shader {
 public:
  Shader() noexcept = default;
  explicit Shader(const std::vector<ShaderModule> &shaders);
  Shader(const Shader &) = delete;
  Shader(Shader &&other) noexcept;
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&other) noexcept;
  ~Shader();
  void use() const;
  void setInt(const char *name, int value) const;
  void setFloat(const char *name, float value) const;
  void setVec3(const char *name, const glm::vec3 &value) const;
  void setVec4(const char *name, const glm::vec4 &value) const;
  void setMat4(const char *name, const glm::mat4 &value) const;

 private:
  uint32_t id_;

 private:
  static void checkLinkingErrors(uint32_t id);
  void drop();
};

class ShaderBuilder {
 public:
  ShaderBuilder &load(std::string_view path, ShaderModule::Type type);
  Shader build() const;

 private:
  std::vector<ShaderModule> shaders_;
};
