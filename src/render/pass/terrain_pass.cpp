#include "terrain_pass.h"

void drawNode(const TerrainNode &node, ShaderHandle shader, const std::shared_ptr<Mesh> &mesh);

void TerrainPass::render(Scene *scene, RenderContext *context) {
  Terrain *terrain = scene->terrain.get();

  shader_ = ResourceManager::getShader("terrain");
  shader_->use();
  shader_->setMat4("view", scene->camera.getViewMatrix());
  shader_->setMat4("projection", scene->camera.getProjectionMatrix());
  shader_->setVec3("light.direction", glm::normalize(scene->light.getDirection()));
  shader_->setVec3("light.color", scene->light.getColor());
  shader_->setFloat("light.intensity", scene->light.getIntensity());
  shader_->setVec4("clipPlane", context->getClipPlane());

  glm::mat4 model = glm::mat4(1.0f);
  shader_->setMat4("model", model);
  shader_->setInt("heightmap", 0);
  shader_->setInt("normalmap", 1);
  shader_->setFloat("scale_y", terrain->getScaleY());
  shader_->setVec3("color", glm::vec3(0.45f, 0.4f, 0.3f));
  shader_->setVec3("camera", scene->camera.position);
  shader_->setMat4("world_matrix", terrain->getTransform().getMatrix());
  shader_->setFloat("tessellation_factor", terrain->getConfig().tessellation_factor);
  shader_->setFloat("tessellation_slope", terrain->getConfig().tessellation_slope);
  shader_->setFloat("tessellation_shift", terrain->getConfig().tessellation_shift);
  auto &areas = terrain->getConfig().lod_morphing_areas;
  for (auto i = 0; i < areas.size(); i++) {
    shader_->setInt(std::string("lod_morph_area[" + std::to_string(i) + "]").c_str(), areas[i]);
  }
  terrain->getHeightmap().bind(0);
  terrain->getNormalmap().bind(1);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  terrain->getMesh()->bind();
  glPatchParameteri(GL_PATCH_VERTICES, terrain->getMesh()->count());

  for (auto &node : terrain->getNodes()) {
    drawNode(node, shader_, terrain->getMesh());
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawNode(const TerrainNode &node, ShaderHandle shader, const std::shared_ptr<Mesh> &mesh) {
  if (node.isLeaf()) {
    shader->setMat4("local_matrix", node.getTransform().getMatrix());
    shader->setInt("lod", node.getLod());
    shader->setVec2("index", node.getIndex());
    shader->setFloat("gap", node.getGap());
    shader->setVec2("location", node.getLocation());
    glDrawArrays((GLenum)mesh->getTopology(), 0, mesh->count());
  } else {
    for (auto &n : node.getNodes()) {
      drawNode(n, shader, mesh);
    }
  }
}
