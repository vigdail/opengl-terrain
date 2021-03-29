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
  Mesh(const std::shared_ptr<VertexArray> &vertex_array,
       PrimitiveTopology topology);
  PrimitiveTopology GetTopology() const;
  std::shared_ptr<VertexArray> GetVertexArray() const;
  std::shared_ptr<VertexArray> GetVertexArray();

 private:
  std::shared_ptr<VertexArray> vertex_array_;
  PrimitiveTopology topology_;
};
