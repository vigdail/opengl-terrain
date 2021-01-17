#include "terrain.h"
#include <glad/glad.h>
#include <random>

Terrain::Terrain() : Terrain(100, 512, 512) {}

Terrain::Terrain(int size) : Terrain(size, 512, 512) {}

Terrain::Terrain(int size, int width, int length)
    : res_x_(width),
      res_z_(length),
      size_(size),
      vertices_(std::vector<Vertex>(res_x_ * res_z_)),
      normals_(std::vector<glm::vec3>(res_x_ * res_z_)),
      indices_(std::vector<int>((res_x_ - 1) * (res_z_ - 1) * 2 * 3)) {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  GenerateVertices();
  GenerateIndices();
  BuildVAO();
}

void Terrain::Draw(Shader &shader) {
  shader.Use();
  glm::mat4 model = glm::mat4(1.0f);
  shader.SetMat4("model", model);
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::GenerateVertices() {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(0.0, 0.01f);

  for (int i = 0; i < res_z_; i++) {
    for (int j = 0; j < res_x_; j++) {
      float x = (j - (res_x_ - 1) / 2.0f) * size_ / res_x_;
      float z = (i - (res_z_ - 1) / 2.0f) * size_ / res_z_;
      float y = sin(z * x / 100.0f) + 1.0f;
      Vertex v = {
          glm::vec3(x, y, z),
          glm::vec2(static_cast<float>(j) / res_x_,
                    static_cast<float>(i) / res_z_),
      };
      vertices_[i * res_x_ + j] = v;
    }
  }

  for (int z = 0; z < res_z_; z++) {
    for (int x = 0; x < res_x_; x++) {
      float heightL = GetHeight(x - 1, z);
      float heightR = GetHeight(x + 1, z);
      float heightT = GetHeight(x, z - 1);
      float heightD = GetHeight(x, z + 1);
      // @TODO: Proper normal y calculations based on max height value maybe
      glm::vec3 normal =
          glm::vec3((heightL - heightR), 0.1f, (heightD - heightT));
      normal = glm::normalize(normal);
      normals_[z * res_x_ + x] = normal;
    }
  }
}

float Terrain::GetHeight(int x, int z) {
  if (x < 0 || z < 0 || x >= res_x_ || z >= res_z_) {
    return 0;
  }

  float r = vertices_[z * res_x_ + x].position.y;
  return r;
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
                        static_cast<void *>(0) + offsetof(Vertex, uv));

  unsigned int normals_VBO;
  glGenBuffers(1, &normals_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
  glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3),
               normals_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                        static_cast<void *>(0));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
