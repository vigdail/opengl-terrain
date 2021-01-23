#include "terrain.h"
#include "normalmap_renderer.h"
#include "resource_manager.h"

#include <glad/glad.h>

Terrain::Terrain() : Terrain(100) {}

Terrain::Terrain(int size) : Terrain(size, 512, 512) {}

Terrain::Terrain(int size, int width, int length)
    : res_x_(width),
      res_z_(length),
      size_(size),
      vertices_(std::vector<Vertex>(res_x_ * res_z_)),
      indices_(std::vector<int>((res_x_ - 1) * (res_z_ - 1) * 2 * 3)) {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  GenerateVertices();
  GenerateIndices();

  HeightmapGenerator generator(res_x_, res_z_);
  generator.Generate();
  Texture heightmap = generator.CreateTexture();

  NormalmapRenderer nm_renderer;
  normalmap_ = nm_renderer.Render(heightmap);

  SetHeightmap(generator.GetData());

  BuildVAO();
}

void Terrain::SetHeightmap(const std::vector<float> &heightmap) {
  for (int i = 0; i < res_x_ * res_z_; i++) {
    vertices_[i].position.y = heightmap[i] * 20.0f;
  }
}

void Terrain::Draw(Shader &shader) {
  glm::mat4 model = glm::mat4(1.0f);
  shader.SetMat4("model", model);
  shader.SetVec3("color", glm::vec3(0.45f, 0.4f, 0.3f));
  glActiveTexture(GL_TEXTURE0);
  normalmap_.Bind();
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::GenerateVertices() {
  for (int i = 0; i < res_z_; i++) {
    for (int j = 0; j < res_x_; j++) {
      float x = (j - (res_x_ - 1) / 2.0f) * size_ / res_x_;
      float z = (i - (res_z_ - 1) / 2.0f) * size_ / res_z_;
      float y = 0.0f;
      Vertex v = {
          glm::vec3(x, y, z),
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
}

int Terrain::GetIndex(int x, int z) { return z * res_x_ + x; }

void Terrain::BuildVAO() {
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
               indices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        static_cast<void *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, uv)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
