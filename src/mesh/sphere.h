#pragma once

#include "mesh.h"

#include <glm/glm.hpp>
#include <vector>

class Sphere {
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    static BufferLayout GetLayout() {
      VertexAttribute position_attr{};
      position_attr.count = 3;
      position_attr.offset = 0;
      position_attr.location = 0;
      position_attr.normalized = false;
      VertexAttribute normal_attr{};
      normal_attr.count = 3;
      normal_attr.offset = offsetof(Vertex, normal);
      normal_attr.location = 1;
      normal_attr.normalized = false;
      VertexAttribute uv_attr{};
      uv_attr.count = 2;
      uv_attr.offset = offsetof(Vertex, uv);
      uv_attr.location = 2;
      uv_attr.normalized = false;

      BufferLayout layout{};
      layout.stride = sizeof(Vertex);
      layout.attributes = {position_attr, normal_attr, uv_attr};

      return layout;
    }
  };

 public:
  Sphere(int u, int v, float radius);
  Mesh ToMesh() const {
    Mesh mesh{PrimitiveTopology::Triangle, indices_.size()};
    mesh.AddVertexBuffer(VertexBuffer{vertices_, Vertex::GetLayout()});
    mesh.SetIndexBuffer(IndexBuffer{indices_});

    return mesh;
  }

 private:
  int sectors_;
  int stacks_;
  float radius_;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

 private:
  void GenerateVertices();
  void GenerateIndices();
};
