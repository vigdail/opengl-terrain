#include "mesh.h"

Mesh::Mesh(PrimitiveTopology topology, size_t count)
    : count_{count}, topology_{topology} {
  glCreateVertexArrays(1, &id_);
}
Mesh::Mesh(Mesh &&other) noexcept
    : id_{other.id_},
      count_{other.count_},
      topology_{other.topology_},
      vertex_buffers_{std::move(other.vertex_buffers_)},
      index_buffer_{std::move(other.index_buffer_)} {
  other.id_ = 0;
}
Mesh &Mesh::operator=(Mesh &&other) noexcept {
  std::swap(id_, other.id_);
  std::swap(count_, other.count_);
  std::swap(topology_, other.topology_);
  std::swap(vertex_buffers_, other.vertex_buffers_);
  std::swap(index_buffer_, other.index_buffer_);
  return *this;
}
Mesh::~Mesh() {
  glDeleteVertexArrays(1, &id_);
  id_ = 0;
}
void Mesh::bind() const { glBindVertexArray(id_); }
void Mesh::unbind() const { glBindVertexArray(0); }
PrimitiveTopology Mesh::getTopology() const { return topology_; }
void Mesh::addVertexBuffer(VertexBuffer &&buffer) {
  glBindVertexArray(id_);
  buffer.bind();

  auto layout = buffer.getLayout();
  for (const auto &element : layout.attributes) {
    glEnableVertexAttribArray(element.location);
    glVertexAttribPointer(element.location, element.count,
                          static_cast<GLenum>(element.type),
                          element.normalized ? GL_TRUE : GL_FALSE,
                          layout.stride, (const void *)element.offset);
  }

  vertex_buffers_.push_back(std::move(buffer));
}

void Mesh::setIndexBuffer(IndexBuffer &&buffer) {
  glBindVertexArray(id_);
  buffer.bind();

  index_buffer_ = std::move(buffer);
}
