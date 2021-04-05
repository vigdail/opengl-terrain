#pragma once

#include "../render_context.h"
#include "../../scene.h"
#include "../../resource_manager.h"

class SkyboxPass {
 public:
  void render(Scene *scene, RenderContext *context) {
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    shader_ = ResourceManager::getShader("skybox");
    Skybox *skybox = scene->skybox.get();
    Atmosphere &atmosphere = skybox->getAtmosphere();

    shader_->use();
    shader_->setMat4("view",
                     glm::mat4(glm::mat3(scene->camera.getViewMatrix())));
    shader_->setMat4("projection", scene->camera.getProjectionMatrix());
    shader_->setVec3("camera", scene->camera.position);
    shader_->setVec3("sun.direction",
                     glm::normalize(scene->light.getDirection()));
    shader_->setVec3("sun.color", scene->light.getColor());
    shader_->setFloat("sun.intensity", scene->light.getIntensity());

    shader_->setFloat("planet_radius", atmosphere.planet_radius);
    shader_->setFloat("atmosphere_radius", atmosphere.atmosphere_radius);
    shader_->setFloat("hR", atmosphere.h_r);
    shader_->setFloat("hM", atmosphere.h_m);
    shader_->setFloat("g", atmosphere.g);
    shader_->setVec3("beta_R", atmosphere.beta_r);
    shader_->setFloat("beta_M", atmosphere.beta_m);
    shader_->setInt("view_dir_samples", atmosphere.view_dir_samples);
    shader_->setInt("sun_dir_samples", atmosphere.sun_dir_samples);

    skybox->getMesh().bind();
    glDrawElements(static_cast<GLenum>(topology_), skybox->getMesh().count(),
                   GL_UNSIGNED_INT, nullptr);

    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
  }

 private:
  ShaderHandle shader_ = nullptr;
  PrimitiveTopology topology_ = PrimitiveTopology::TRIANGLE;
};
