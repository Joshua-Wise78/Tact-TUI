#pragma once

#include "engine.hpp"

class TUI {
public:
  TUI() = default;
  ~TUI() = default;

  void run(Engine &engine);
};
