#include "engine.hpp"
#include "text_provider.hpp"
#include <chrono>

Engine::Engine(std::unique_ptr<TextProvider> text_provider,
               int duration_seconds)
    : provider(std::move(text_provider)), running(false), finished(false),
      time_limit(duration_seconds) {
  stats = {0, 100.0, 0, 0};
  completed_chunks = 0;
  load_next_chunk();
}

void Engine::load_next_chunk() {
  target_text.clear();
  user_input.clear();

  while (target_text.size() < 150 && provider->has_next()) {
    target_text += provider->get_next_word();
  }

  if (!target_text.empty() && target_text.back() == ' ') {
    target_text.pop_back();
  }
}

void Engine::start() {
  running = true;
  finished = false;
  start_time = std::chrono::steady_clock::now();
}

void Engine::stop() { running = false; }

bool Engine::is_running() const { return running; }
bool Engine::is_finished() const { return finished; }

void Engine::update() {
  if (!running)
    return;

  auto now = std::chrono::steady_clock::now();
  double elapsed_seconds =
      std::chrono::duration<double>(now - start_time).count();

  if (elapsed_seconds >= time_limit) {
    running = false;
    finished = true;
    return;
  }

  if (elapsed_seconds > 0.0) {
    double minutes = elapsed_seconds / 60.0;
    stats.wpm = static_cast<int>((stats.correct_keystrokes / 5.0) / minutes);
  }
}

double Engine::get_time_remaining() const {
  if (!running && !finished)
    return time_limit;
  if (finished)
    return 0.0;

  auto now = std::chrono::steady_clock::now();
  double elapsed = std::chrono::duration<double>(now - start_time).count();
  return std::max(0.0, time_limit - elapsed);
}

void Engine::process_keystroke(int c) {
  if (c == 127 || c == 8 || c == 263) {
    if (!user_input.empty()) {
      user_input.pop_back();
    }
    return;
  }

  if (c >= 32 && c <= 126) {
    user_input += static_cast<char>(c);
    stats.total_keystrokes++;

    int current_chunk_correct = 0;
    for (size_t i = 0; i < user_input.size(); ++i) {
      if (i < target_text.size() && user_input[i] == target_text[i]) {
        current_chunk_correct++;
      }
    }

    stats.correct_keystrokes = completed_chunks + current_chunk_correct;

    if (stats.total_keystrokes > 0) {
      stats.accuracy = (static_cast<double>(stats.correct_keystrokes) /
                        stats.total_keystrokes) *
                       100.0;
    }

    if (user_input.size() >= target_text.size()) {
      completed_chunks += current_chunk_correct;
      load_next_chunk();
    }
  }
}

std::string Engine::get_target_text() const { return target_text; }
std::string Engine::get_user_input() const { return user_input; }
GameStats Engine::get_current_stats() const { return stats; }
