#include "state_manager.h"
#include "core/event.h"
#include "core/input.h"
#include "count_timer.h"
#include "graphics/renderer.h"
#include "graphics/renderer2d.h"
#include "states/game_state.h"
#include "states/state_type.h"
#include "engine/ui/ui_canvas.h"
#include "engine/resources/font.h"
#include "ui/ui_anchor.h"

// Callbacks 
/////////////////////////////////////////////////////////////////////////////////
static void menu_button_change_scene(UIButton* button, const UIButtonState button_state, void* user_data) {
  if(button_state != BUTTON_STATE_PRESSED) {
    return;
  }

  StateManger* state_manager = (StateManger*)user_data;

  // @NOTE: Not a good way to detect buttons by the way
  if(button->text.str == "START") {
    state_manager->current_state = STATE_GAME;
    game_state_reset(&state_manager->game_state);
  }
  else if(button->text.str == "SETTINGS") {
    state_manager->current_state = STATE_SETTINGS;
  }
  else if(button->text.str == "QUIT") {
    event_dispatch(EVENT_GAME_QUIT, EventDesc{});
  }
}

static void settings_button_change_scene(UIButton* button, const UIButtonState button_state, void* user_data) {
  if(button_state != BUTTON_STATE_PRESSED) {
    return;
  }

  StateManger* state_manager = (StateManger*)user_data;

  if(button->text.str == "MENU") {
    state_manager->current_state = STATE_MENU; 
  }
}

static void lose_button_change_scene(UIButton* button, const UIButtonState button_state, void* user_data) {
  if(button_state != BUTTON_STATE_PRESSED) {
    return;
  }

  StateManger* state_manager = (StateManger*)user_data;

  if(button->text.str == "RE-SOLVE") {
    state_manager->current_state = STATE_GAME;
    game_state_reset(&state_manager->game_state);
  }
}
/////////////////////////////////////////////////////////////////////////////////

// Private functions 
/////////////////////////////////////////////////////////////////////////////////
static void menu_state_init(StateManger* state, Font* font) {
  UICanvas* canvas = state->states[STATE_MENU];

  ui_canvas_push_text(canvas, "The Problem Solver", 50.0f, glm::vec4(1.0f), UI_ANCHOR_TOP_CENTER, glm::vec2(0, 10));

  ui_canvas_begin(canvas, glm::vec2(0.0f, 50.0f), UI_ANCHOR_CENTER);
  ui_canvas_push_button(canvas, "START", 30.0f, glm::vec4(1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), (void*)state, menu_button_change_scene);
  ui_canvas_push_button(canvas, "SETTINGS", 30.0f, glm::vec4(1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), (void*)state, menu_button_change_scene);
  ui_canvas_push_button(canvas, "QUIT", 30.0f, glm::vec4(1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), (void*)state, menu_button_change_scene);
  ui_canvas_end(canvas);
}

static void settings_state_init(StateManger* state, Font* font) {
  UICanvas* canvas = state->states[STATE_SETTINGS];

  ui_canvas_push_text(canvas, "SETTINGS", 50.0f, glm::vec4(1.0f), UI_ANCHOR_TOP_CENTER, glm::vec2(-5.0f, 20.0f));
  ui_canvas_push_button(canvas, 
                        "MENU", 
                        30.0f, 
                        glm::vec4(1.0f), 
                        glm::vec4(0, 0, 0, 1), 
                        (void*)state, 
                        settings_button_change_scene, 
                        UI_ANCHOR_BOTTOM_LEFT);
}

static void win_state_init(StateManger* state, Font* font) {
  ui_canvas_push_text(state->states[STATE_WIN], "ALL THE PROBLEMS ARE SOLVED!", 50.0f, glm::vec4(1.0f), UI_ANCHOR_CENTER);
}

static void lose_state_init(StateManger* state, Font* font) {
  UICanvas* canvas = state->states[STATE_LOSE];

  ui_canvas_push_text(canvas, "You ran out of time... as you always do", 50.0f, glm::vec4(1.0f), UI_ANCHOR_TOP_CENTER, glm::vec2(0.0f, 10.0f));
  ui_canvas_push_button(canvas, 
                        "RE-SOLVE", 
                        30.0f, 
                        glm::vec4(1.0f), 
                        glm::vec4(0, 0, 0, 1), 
                        (void*)state, 
                        lose_button_change_scene, 
                        UI_ANCHOR_CENTER);
}
/////////////////////////////////////////////////////////////////////////////////

// Public functions 
/////////////////////////////////////////////////////////////////////////////////
void state_manager_init(StateManger* state, Font* font) {
  state->current_state = STATE_MENU;

  // Canvas init 
  for(u32 i = 0; i < STATES_MAX; i++) {
    state->states[i] = ui_canvas_create(font);
  }

  // States init 
  menu_state_init(state, font);
  settings_state_init(state, font);
  game_state_init(&state->game_state);
  win_state_init(state, font);
  lose_state_init(state, font);
}

void state_manager_shutdown(StateManger* state) {
  for(auto& canvas : state->states) {
    if(canvas) {
      ui_canvas_destroy(canvas);
    }    
  }
}

void state_manager_update(StateManger* state) {
  if(state->current_state != STATE_GAME) {
    return;
  }

  // @NOTE: This is really really bad but it works so it's fine 
  if(count_timer_has_runout(&state->game_state.timer)) {
    state->current_state = STATE_LOSE;
    input_cursor_show(true);
  }

  game_state_update(&state->game_state);
}

void state_manager_render(StateManger* state) {
  renderer_begin(&state->game_state.camera);
  
  if(state->current_state == STATE_GAME) {
    game_state_render(&state->game_state);
  }
  
  renderer_end();
}

void state_manager_render_ui(StateManger* state) {
  renderer2d_begin();

  if(state->current_state == STATE_GAME) { // Render the game's ui
    game_state_render_ui(&state->game_state);
  }
  else { // Render the menus
    ui_canvas_render(state->states[state->current_state]);  
  }
  
  renderer2d_end();
}
/////////////////////////////////////////////////////////////////////////////////
