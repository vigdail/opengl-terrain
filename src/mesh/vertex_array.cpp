#include "vertex_array.h"

VertexArray::VertexArray() { glCreateVertexArrays(1, &id_); }
VertexArray::VertexArray(VertexArray &&other)
    : id_{other.id_},
      vertex_buffers_{std::move(other.vertex_buffers_)},
      index_buffer_{std::move(other.index_buffer_)} {
  other.id_ = 0;
}
VertexArray &VertexArray::operator=(VertexArray &&other) {
  std::swap(id_, other.id_);
  std::swap(vertex_buffers_, other.vertex_buffers_);
  std::swap(index_buffer_, other.index_buffer_);
  return *this;
}
VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &id_);
  id_ = 0;
}
void VertexArray::Bind() const { glBindVertexArray(id_); }
void VertexArray::Unbind() const { glBindVertexArray(0); }
void VertexArray::AddVertexBuffer(VertexBuffer &&buffer) {
  glBindVertexArray(id_);
  buffer.Bind();

  auto layout = buffer.GetLayout();
  for (const auto &element : layout.attributes) {
    glEnableVertexAttribArray(element.location);
    glVertexAttribPointer(element.location, element.count,
                          static_cast<GLenum>(element.type),
                          element.normalized ? GL_TRUE : GL_FALSE,
                          layout.stride, (const void *)element.offset);
  }

  vertex_buffers_.push_back(std::move(buffer));
}

void VertexArray::SetIndexBuffer(IndexBuffer &&buffer) {
  glBindVertexArray(id_);
  buffer.Bind();

  index_buffer_ = std::move(buffer);
}
