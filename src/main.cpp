#include "engine.hpp"
#include "text_provider.hpp"
#include "tui.hpp"
#include <memory>

int main() {
  TUI tui;

  while (true) {
    AppConfig config = tui.show_menu();

    if (config.quit) {
      break;
    }

    std::unique_ptr<TextProvider> provider;
    if (config.provider_type == 1) {
      provider = std::make_unique<SystemDictonaryProvider>();
    } else {
      provider = std::make_unique<CustomBookProvider>("data/default_book.txt");
    }

    Engine engine(std::move(provider), config.duration);

    tui.run_test(engine);

    if (engine.is_finished()) {
      tui.show_results(engine.get_current_stats());
    }
  }

  return 0;
}
