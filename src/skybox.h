#pragma once

#include "shader.h"

#include <string>
#include <vector>

class Skybox {
 public:
  explicit Skybox(std::vector<std::string> faces);
  void Draw(Shader& shader);

 private:
  unsigned int VAO_;
  unsigned int VBO_;
  unsigned int cubemap_;

  static unsigned int CreateVAO(unsigned int VBO);
  static unsigned int CreateVBO();
  static unsigned int LoadCubemap(const std::vector<std::string>& faces);
};
