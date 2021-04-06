#include "terrain_pass.h"

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
  terrain->getHeightmap().bind(0);
  terrain->getNormalmap().bind(1);

  terrain->getMesh().bind();
  glDrawElements((GLenum)topology_, terrain->getMesh().count(), GL_UNSIGNED_INT, nullptr);
}
