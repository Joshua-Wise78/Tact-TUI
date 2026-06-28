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

  std::string target_text;
  std::string user_input;

  GameStats stats;

  int completed_chunks;

  bool running;
  bool finished;
  int time_limit;
  std::chrono::time_point<std::chrono::steady_clock> start_time;

  void load_next_chunk();

public:
  Engine(std::unique_ptr<TextProvider> text_provider, int duration_seconds);
  ~Engine() = default;

  void start();
  void stop();
  void update();
  void process_keystroke(int c);

  std::string get_target_text() const;
  std::string get_user_input() const;

  GameStats get_current_stats() const;
  bool is_running() const;
  bool is_finished() const;
  double get_time_remaining() const;
};
