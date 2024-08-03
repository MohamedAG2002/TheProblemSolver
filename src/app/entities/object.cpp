#include "object.h"
#include "graphics/renderer.h"
#include "physics/physics_body.h"
#include "physics/collider.h"
#include "physics/physics_world.h"
#include "resources/material.h"
#include "resources/mesh.h"
#include "resources/resource_manager.h"
#include "resources/texture.h"

#include <string>

// Public functions
/////////////////////////////////////////////////////////////////////////////////
Object* object_create(const glm::vec3& scale, const PhysicsBodyDesc desc, const std::string& texture_id, const bool active) {
  Object* obj = new Object{};
  obj->body = physics_world_add_body(desc);
  obj->collider = BoxCollider{.half_size = scale / 2.0f};
  physics_body_add_collider(obj->body, COLLIDER_BOX, &obj->collider);

  obj->mesh = mesh_create();

  if(texture_id != "") {
    obj->material = material_load(resources_get_texture(texture_id), nullptr, resources_get_shader("default_shader-3d"));
  }
  else {
    u32 pixels = 0xff00ffff;
    obj->material = material_load(texture_load(1, 1, TEXTURE_FORMAT_RGBA, &pixels), nullptr, resources_get_shader("default_shader-3d"));
  }
  
  obj->is_active = active;

  return obj;
}

void object_destroy(Object* obj) {
  // Why are you trying to delete a null object???
  if(!obj) {
    return;
  }

  mesh_destroy(obj->mesh);
  delete obj;
}

void object_render(Object* obj) {
  // Don't render an inactive object. 
  // This is better for performance
  if(!obj->is_active) {
    return;
  }

  render_mesh(obj->body->transform, obj->mesh, obj->material);
}

void object_activate(Object* obj, const bool active) {
  obj->is_active = active; 
  obj->body->is_active = active;

  // Reset the velocites too
  obj->body->linear_velocity = glm::vec3(0.0f);
  obj->body->angular_velocity = glm::vec3(0.0f);
}
/////////////////////////////////////////////////////////////////////////////////
