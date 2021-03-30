#pragma once

#include "mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Quad {
 public:
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;

    static BufferLayout GetLayout() {
      VertexAttribute position_attr{};
      position_attr.count = 3;
      position_attr.offset = 0;
      position_attr.location = 0;
      position_attr.normalized = false;
      VertexAttribute uv_attr{};
      uv_attr.count = 2;
      uv_attr.offset = offsetof(Vertex, uv);
      uv_attr.location = 1;
      uv_attr.normalized = false;

      BufferLayout layout{};
      layout.stride = sizeof(Vertex);
      layout.attributes = {position_attr, uv_attr};

      return layout;
    }
  };

  Quad()
      : vertices_{
            {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
            {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        }, indices_{0, 1, 2, 2, 1, 3} {
  }

  Mesh ToMesh() const {
    Mesh mesh{PrimitiveTopology::Triangle, indices_.size()};
    mesh.AddVertexBuffer(VertexBuffer{vertices_, Vertex::GetLayout()});
    mesh.SetIndexBuffer(IndexBuffer{indices_});

    return mesh;
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};
