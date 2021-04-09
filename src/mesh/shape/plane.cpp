#include "plane.h"

Plane::Plane(float size)
    : vertices_{
        {glm::vec3(-1.0f, 0.0f, -1.0f) * size, glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-1.0f, 0.0f, 1.0f) * size, glm::vec2(0.0f, 1.0f)},
        {glm::vec3(1.0f, 0.0f, -1.0f) * size, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(1.0f, 0.0f, 1.0f) * size, glm::vec2(1.0f, 1.0f)},
    },
      indices_{0, 1, 2, 2, 1, 3} {}

Mesh Plane::toMesh() const {
  Mesh mesh{PrimitiveTopology::TRIANGLE, indices_.size()};
  mesh.addVertexBuffer(VertexBuffer{vertices_, Vertex::getLayout()});
  mesh.setIndexBuffer(IndexBuffer{indices_});

  return mesh;
}
