#pragma once

#include "defines.h"
#include "core/input.h"

#include <glm/vec2.hpp>

struct Texture;
struct GLFWwindow;

// Public functions
/////////////////////////////////////////////////////////////////////////////////
const bool window_create(const i32 width, const i32 height, const char* title); 
void window_destroy();
void window_poll_events();
void window_swap_buffers();

const bool window_should_close();
const glm::vec2 window_get_size();
const f64 window_get_time();
const f32 window_get_aspect_ratio();
GLFWwindow* window_get_handle(); 
const KeyCode window_get_exit_key();
const bool window_get_fullscreen();

void window_set_size(const glm::vec2& size);
void window_set_current_context();
void window_set_vsync(const bool vsync);
void window_set_fullscreen(const bool fullscreen);
void window_set_close(const bool close);
void window_set_exit_key(KeyCode key);
void window_set_sensitivity(const f32 sens);
void window_set_icon(const Texture* icon);
void window_set_cursor_image(const Texture* img);
/////////////////////////////////////////////////////////////////////////////////
