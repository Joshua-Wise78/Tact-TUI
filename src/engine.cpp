#include "engine.hpp"
#include "text_provider.hpp"
#include <chrono>

Engine::Engine(std::unique_ptr<TextProvider> text_provider)
    : provider(std::move(text_provider)), running(false) {
  current_target_word = provider->get_next_word();
  stats = {0, 100.0, 0, 0};
}

void Engine::start() {
  running = true;
  start_time = std::chrono::steady_clock::now();
}

void Engine::stop() { running = false; }

bool Engine::is_running() const { return running; }

void Engine::process_keystroke(int c) {
  if (c == 127 || c == 8 || c == 263) {
    if (!current_user_input.empty()) {
      current_user_input.pop_back();
    }
    return;
  }

  if (c >= 32 && c <= 126) {
    current_user_input += static_cast<char>(c);
    stats.total_keystrokes++;

    int correct = 0;
    for (size_t i = 0; i < current_user_input.size(); ++i) {
      if (i < current_target_word.size() &&
          current_user_input[i] == current_target_word[i]) {
        correct++;
      }
    }
    stats.correct_keystrokes = correct;

    if (stats.total_keystrokes > 0) {
      stats.accuracy =
          (static_cast<double>(correct) / stats.total_keystrokes) * 100.0;
    }

    if (current_user_input.size() >= current_target_word.size()) {
      current_target_word = provider->get_next_word();
      current_user_input.clear();
    }
  }

  auto now = std::chrono::steady_clock::now();
  double minutes =
      std::chrono::duration<double, std::ratio<60>>(now - start_time).count();

  if (minutes > 0.0) {
    stats.wpm = static_cast<int>((stats.correct_keystrokes / 5.0) / minutes);
  }
}

std::string Engine::get_target_word() const { return current_target_word; }
std::string Engine::get_user_input() const { return current_user_input; }
GameStats Engine::get_current_stats() const { return stats; }
