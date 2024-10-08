#pragma once

#include "collision_data.h"
#include "defines.h"
#include "math/transform.h"

#include <glm/vec3.hpp>

// Forward declaration ew 
struct PhysicsBody;

// ColliderType
/////////////////////////////////////////////////////////////////////////////////
enum ColliderType {
  COLLIDER_BOX = 0xff, 
  COLLIDER_SPHERE,
};
/////////////////////////////////////////////////////////////////////////////////

// Generic Collider struct
/////////////////////////////////////////////////////////////////////////////////
struct Collider {
  ColliderType type;
  void* data;

  PhysicsBody* body; // The attached body
};
/////////////////////////////////////////////////////////////////////////////////

// BoxCollider
/////////////////////////////////////////////////////////////////////////////////
struct BoxCollider {
  glm::vec3 half_size;
  glm::vec3 min, max;
};
/////////////////////////////////////////////////////////////////////////////////

// SphereCollider
/////////////////////////////////////////////////////////////////////////////////
struct SphereCollider {
  f32 radius;
};
/////////////////////////////////////////////////////////////////////////////////

// Public functions
/////////////////////////////////////////////////////////////////////////////////
CollisionData collider_colliding(Collider* coll_a, const Transform* trans_a, Collider* coll_b, const Transform* trans_b);

CollisionPoint sphere_colliding(SphereCollider* sphere_a, const Transform* trans_a, SphereCollider* sphere_b, const Transform* trans_b);
CollisionPoint sphere_aabb_colliding(SphereCollider* sphere, const Transform* sphere_trans, BoxCollider* box, const Transform* box_trans);

bool aabb_colliding(const glm::vec3& pos_a, const glm::vec3& size_a, const glm::vec3& pos_b, const glm::vec3& size_b);
CollisionPoint aabb_colliding_ex(BoxCollider* box_a, const Transform* trans_a, BoxCollider* box_b, const Transform* trans_b);

void collider_debug_render(const Transform& transform, const Collider* collider);
/////////////////////////////////////////////////////////////////////////////////
