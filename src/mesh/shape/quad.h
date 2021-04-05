#pragma once

#include "../mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Quad {
 public:
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;

    static BufferLayout getLayout() {
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

  Quad();
  Mesh toMesh() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};
