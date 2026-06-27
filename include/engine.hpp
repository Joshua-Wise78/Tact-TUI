#pragma once

#include "text_provider.hpp"
#include <chrono>
#include <memory>
#include <string>

struct GameStats {
  int wpm;
  double accuracy;
  int correct_keystrokes;
  int total_keystrokes;
};

class Engine {
private:
  std::unique_ptr<TextProvider> provider;
  std::string current_target_word;
  std::string current_user_input;

  GameStats stats;

  bool running;
  std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
  Engine(std::unique_ptr<TextProvider> text_provider);
  ~Engine() = default;

  void start();
  void stop();
  void process_keystroke(int c);

  std::string get_target_word() const;
  std::string get_user_input() const;
  GameStats get_current_stats() const;
  bool is_running() const;
};
