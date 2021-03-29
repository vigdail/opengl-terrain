#pragma once

#include <glm/glm.hpp>
#include <vector>

class Sphere {
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
  };

 public:
  Sphere(int u, int v, float radius);
  void Draw();

 private:
  int sectors_;
  int stacks_;
  float radius_;
  unsigned int VAO_;
  unsigned int VBO_;
  unsigned int EBO_;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

 private:
  void GenerateVertices();
  void GenerateIndices();
  void BuildVAO();
};
