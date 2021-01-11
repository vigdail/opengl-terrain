
#include "terrain.h"

Terrain::Terrain() : Terrain(10, 10) {}

Terrain::Terrain(int width, int length)
    : width_(width),
      length_(length),
      vertices_(std::vector<Vertex>(width_ * length_)),
      indices_(std::vector<int>(width_ * length_)) {
  // fill the vectors
}
