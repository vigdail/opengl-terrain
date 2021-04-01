#pragma once

#include <string>
#include <vector>
#include <utility>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "util.h"

class Shader;

class ShaderModule {
  friend Shader;

 public:
  enum class Type {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER,
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
  static void CheckCompileErrors(uint32_t id);
};

class Shader {
 public:
  Shader() noexcept = default;
  explicit Shader(const std::vector<ShaderModule> &shaders);
  Shader(const Shader &) = delete;
  Shader(Shader &&other) noexcept;
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&other);
  ~Shader();
  void Use();
  void SetInt(const char *name, const int value);
  void SetFloat(const char *name, const float value);
  void SetVec3(const char *name, const glm::vec3 &value);
  void SetVec4(const char *name, const glm::vec4 &value);
  void SetMat4(const char *name, const glm::mat4 &value);

 private:
  uint32_t id_;

 private:
  static void CheckLinkingErrors(uint32_t id);
  void Delete();
};

class ShaderBuilder {
 public:
  ShaderBuilder &Load(std::string_view path, ShaderModule::Type type);
  Shader Build() const;

 private:
  std::vector<ShaderModule> shaders_;
};
