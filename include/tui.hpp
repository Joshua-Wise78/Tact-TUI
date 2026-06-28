#pragma once

#include "engine.hpp"

struct AppConfig {
  int provider_type;
  int duration;
  bool quit;
};

class TUI {
public:
  TUI();
  ~TUI();

  AppConfig show_menu();
  void run_test(Engine &engine);
  void show_results(const GameStats &stats);
};
