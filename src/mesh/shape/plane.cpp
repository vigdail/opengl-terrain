#include "plane.h"

Plane::Plane(float size)
    : vertices_{
          {glm::vec3(-1.0f, 0.0f, -1.0f) * size, glm::vec2(0.0f, 0.0f)},
          {glm::vec3(-1.0f, 0.0f, 1.0f) * size, glm::vec2(0.0f, 1.0f)},
          {glm::vec3(1.0f, 0.0f, -1.0f) * size, glm::vec2(1.0f, 0.0f)},
          {glm::vec3(1.0f, 0.0f, 1.0f) * size, glm::vec2(1.0f, 1.0f)},
      },
      indices_{0, 1, 2, 2, 1, 1} {}

Mesh Plane::ToMesh() const {
  Mesh mesh{PrimitiveTopology::Triangle, indices_.size()};
  mesh.AddVertexBuffer(VertexBuffer{vertices_, Vertex::GetLayout()});
  mesh.SetIndexBuffer(IndexBuffer{indices_});

  return mesh;
}
