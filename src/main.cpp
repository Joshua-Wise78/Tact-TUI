#include "engine.hpp"
#include "text_provider.hpp"
#include "tui.hpp"
#include <memory>

int main() {
  auto provider = std::make_unique<SystemDictonaryProvider>();

  Engine engine(std::move(provider));

  TUI tui;
  tui.run(engine);

  return 0;
}
