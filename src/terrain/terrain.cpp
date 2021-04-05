#include "terrain.h"
#include "heightmap_renderer.h"
#include "normalmap_renderer.h"

#include <glad/glad.h>

Terrain::Terrain() : Terrain(100) {}

Terrain::Terrain(int size) : Terrain(size, 512, 512) {}

Terrain::Terrain(int size, int width, int length)
    : res_x_(width),
      res_z_(length),
      size_(size),
      scale_y_(100.0f),
      vertices_(std::vector<Vertex>(res_x_ * res_z_)),
      indices_(std::vector<int>((res_x_ - 1) * (res_z_ - 1) * 2 * 3)),
      heights_(std::vector<float>(res_x_ * res_z_)) {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  generateVertices();
  generateIndices();

  HeightmapRenderer hm_renderer;
  heightmap_ = hm_renderer.render(res_x_, res_z_, 8);

  NormalmapRenderer nm_renderer;
  normalmap_ = nm_renderer.render(heightmap_);

  heightmap_.bind();
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, heights_.data());

  buildVao();

  vertices_.clear();
  vertices_.shrink_to_fit();
  indices_.clear();
  indices_.shrink_to_fit();
}

void Terrain::draw(ShaderHandle shader) {
  glm::mat4 model = glm::mat4(1.0f);
  shader->setMat4("model", model);
  shader->setInt("heightmap", 0);
  shader->setInt("normalmap", 1);
  shader->setFloat("scale_y", scale_y_);
  shader->setVec3("color", glm::vec3(0.45f, 0.4f, 0.3f));
  heightmap_.bind(0);
  normalmap_.bind(1);
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, nullptr);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"
float Terrain::getHeight(float x, float z) const {
  glm::vec2 pos(x, z);

  pos = pos * glm::vec2(res_x_, res_z_) / static_cast<float>(size_) + glm::vec2(res_x_ - 1, res_z_ - 1) / 2.0f;
  int x0 = std::floor(pos.x);
  int x1 = x0 + 1;
  int z0 = std::floor(pos.y);
  int z1 = z0 + 1;

  float h0 = heights_[res_x_ * z0 + x0];
  float h1 = heights_[res_x_ * z0 + x1];
  float h2 = heights_[res_x_ * z1 + x0];
  float h3 = heights_[res_x_ * z1 + x1];

  float percent_u = pos.x - (float)x0;
  float percent_v = pos.y - (float)z0;

  float dU, dV;
  if (percent_u > percent_v) {
    dU = h1 - h0;
    dV = h3 - h1;
  } else {
    dU = h3 - h2;
    dV = h2 - h0;
  }

  float h = h0 + (dU * percent_u) + (dV * percent_v);
  h *= scale_y_;

  return h;
}
#pragma clang diagnostic pop

void Terrain::generateVertices() {
  for (int i = 0; i < res_z_; i++) {
    for (int j = 0; j < res_x_; j++) {
      float x = (j - (res_x_ - 1) / 2.0f) * size_ / res_x_;
      float z = (i - (res_z_ - 1) / 2.0f) * size_ / res_z_;
      Vertex v = {
          glm::vec2(x, z),
          glm::vec2(j / (float)res_x_, (float)(i) / (float)res_z_),
      };
      vertices_[i * res_x_ + j] = v;
    }
  }
}

void Terrain::generateIndices() {
  int index = 0;
  for (int z = 0; z < res_z_ - 1; z++) {
    for (int x = 0; x < res_x_ - 1; x++) {
      indices_[index] = getIndex(x, z);
      indices_[index + 1] = getIndex(x, z + 1);
      indices_[index + 2] = getIndex(x + 1, z);
      indices_[index + 3] = getIndex(x + 1, z);
      indices_[index + 4] = getIndex(x, z + 1);
      indices_[index + 5] = getIndex(x + 1, z + 1);
      index += 6;
    }
  }

  indices_count_ = indices_.size();
}

void Terrain::buildVao() {
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t),
               indices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, uv)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
