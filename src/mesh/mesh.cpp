#include "mesh.h"

Mesh::Mesh(const std::shared_ptr<VertexArray> &vertex_array,
           PrimitiveTopology topology = PrimitiveTopology::Triangle)
    : vertex_array_{vertex_array}, topology_{topology} {}

PrimitiveTopology Mesh::GetTopology() const { return topology_; }

std::shared_ptr<VertexArray> Mesh::GetVertexArray() const {
  return vertex_array_;
}

std::shared_ptr<VertexArray> Mesh::GetVertexArray() { return vertex_array_; }
