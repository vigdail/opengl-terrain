#include "terrain.h"
#include <glad/glad.h>

Terrain::Terrain() : Terrain(100, 512, 512) {}

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
  BuildVAO();
}

void Terrain::GenerateVertices() {
  for (int i = 0; i < res_z_; i++) {
    for (int j = 0; j < res_x_; j++) {
      float x = (j - (res_x_ - 1) / 2.0f) * size_ / res_x_;
      float z = (i - (res_z_ - 1) / 2.0f) * size_ / res_z_;
      float y = sin(x + z) / 5.0f + cos((x - z) / 10);
      Vertex v = {
          glm::vec3(x, y, z),
          glm::vec3(0.0f, 1.0f, 0.0f),
          glm::vec2(static_cast<float>(j) / res_x_,
                    static_cast<float>(i) / res_z_),
      };
      vertices_[i * res_x_ + j] = v;
    }
  }
}

void Terrain::GenerateIndices() {
  int index = 0;
  for (int i = 0; i < res_z_ - 1; i++) {
    for (int j = 0; j < res_x_ - 1; j++) {
      int top_left = i * res_x_ + j;
      int top_right = top_left + 1;
      int bottom_left = (i + 1) * res_x_ + j;
      int bottom_right = bottom_left + 1;
      indices_[index] = top_left;
      indices_[index + 1] = bottom_left;
      indices_[index + 2] = top_right;
      indices_[index + 3] = top_right;
      indices_[index + 4] = bottom_left;
      indices_[index + 5] = bottom_right;
      index += 6;
    }
  }
}

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        static_cast<void *>(0) + offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        static_cast<void *>(0) + offsetof(Vertex, uv));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Terrain::Draw(Shader &shader) {
  shader.Use();
  glm::mat4 model = glm::mat4(1.0f);
  shader.SetMat4("model", model);
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}
