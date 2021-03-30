#include "terrain.h"
#include "normalmap_renderer.h"
#include "heightmap_renderer.h"

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
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  GenerateVertices();
  GenerateIndices();

  HeightmapRenderer hm_renderer;
  heightmap_ = hm_renderer.Render(res_x_, res_z_, 8);

  NormalmapRenderer nm_renderer;
  normalmap_ = nm_renderer.Render(heightmap_);

  heightmap_.Bind();
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, heights_.data());

  BuildVAO();

  vertices_.clear();
  vertices_.shrink_to_fit();
  indices_.clear();
  indices_.shrink_to_fit();
}

void Terrain::Draw(ShaderHandle shader) {
  glm::mat4 model = glm::mat4(1.0f);
  shader->SetMat4("model", model);
  shader->SetInt("heightmap", 0);
  shader->SetInt("normalmap", 1);
  shader->SetFloat("scale_y", scale_y_);
  shader->SetVec3("color", glm::vec3(0.45f, 0.4f, 0.3f));
  heightmap_.Bind(0);
  normalmap_.Bind(1);
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, 0);
}

float Terrain::GetHeight(float x, float z) const {
  float h = 0;

  glm::vec2 pos(x, z);

  pos = pos * glm::vec2(res_x_, res_z_) / static_cast<float>(size_) +
        glm::vec2(res_x_ - 1, res_z_ - 1) / 2.0f;
  int x0 = std::floor(pos.x);
  int x1 = x0 + 1;
  int z0 = std::floor(pos.y);
  int z1 = z0 + 1;

  float h0 = heights_[res_x_ * z0 + x0];
  float h1 = heights_[res_x_ * z0 + x1];
  float h2 = heights_[res_x_ * z1 + x0];
  float h3 = heights_[res_x_ * z1 + x1];

  float percent_u = pos.x - x0;
  float percent_v = pos.y - z0;

  float dU, dV;
  if (percent_u > percent_v) {
    dU = h1 - h0;
    dV = h3 - h1;
  } else {
    dU = h3 - h2;
    dV = h2 - h0;
  }

  h = h0 + (dU * percent_u) + (dV * percent_v);
  h *= scale_y_;

  return h;
}

void Terrain::GenerateVertices() {
  for (int i = 0; i < res_z_; i++) {
    for (int j = 0; j < res_x_; j++) {
      float x = (j - (res_x_ - 1) / 2.0f) * size_ / res_x_;
      float z = (i - (res_z_ - 1) / 2.0f) * size_ / res_z_;
      Vertex v = {
          glm::vec2(x, z),
          glm::vec2(static_cast<float>(j) / res_x_,
                    static_cast<float>(i) / res_z_),
      };
      vertices_[i * res_x_ + j] = v;
    }
  }
}

void Terrain::GenerateIndices() {
  int index = 0;
  for (int z = 0; z < res_z_ - 1; z++) {
    for (int x = 0; x < res_x_ - 1; x++) {
      indices_[index] = GetIndex(x, z);
      indices_[index + 1] = GetIndex(x, z + 1);
      indices_[index + 2] = GetIndex(x + 1, z);
      indices_[index + 3] = GetIndex(x + 1, z);
      indices_[index + 4] = GetIndex(x, z + 1);
      indices_[index + 5] = GetIndex(x + 1, z + 1);
      index += 6;
    }
  }

  indices_count_ = indices_.size();
}

void Terrain::BuildVAO() {
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t),
               indices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        static_cast<void *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, uv)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
