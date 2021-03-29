#pragma once

#include "vertex_array.h"

#include <memory>

enum class PrimitiveTopology {
  Points = GL_POINTS,
  Lines = GL_LINES,
  LineStrip = GL_LINE_STRIP,
  Triangle = GL_TRIANGLES,
  TriangleStrip = GL_TRIANGLE_STRIP,
  TriangleFan = GL_TRIANGLE_FAN,
};

class Mesh {
 public:
  explicit Mesh(const std::shared_ptr<VertexArray> &vertex_array,
                PrimitiveTopology topology = PrimitiveTopology::Triangle)
      : vertex_array_{vertex_array}, topology_{topology} {}
  PrimitiveTopology GetTopology() const { return topology_; }
  std::shared_ptr<VertexArray> GetVertexArray() const { return vertex_array_; }
  std::shared_ptr<VertexArray> GetVertexArray() { return vertex_array_; }

 private:
  std::shared_ptr<VertexArray> vertex_array_;
  PrimitiveTopology topology_;
};
