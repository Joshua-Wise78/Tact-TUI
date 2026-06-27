#include "tui.hpp"
#include <ncurses.h>

void TUI::run(Engine &engine) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, TRUE);

  start_color();
  use_default_colors();
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_RED, -1);

  engine.start();

  while (engine.is_running()) {
    int ch = getch();

    if (ch != ERR) {
      if (ch == 27) {
        engine.stop();
        break;
      }
      engine.process_keystroke(ch);
    }

    erase();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::string target = engine.get_target_word();
    std::string input = engine.get_user_input();
    GameStats stats = engine.get_current_stats();

    mvprintw(0, 2, "WPM: %d | Accuracy: %.2f%%", stats.wpm, stats.accuracy);
    mvprintw(0, max_x - 20, "Press ESC to Quit");

    int start_y = max_y / 2;
    int start_x = 5;

    mvprintw(start_y, start_x, "");

    for (size_t i = 0; i < target.size(); ++i) {
      if (i < input.size()) {
        if (input[i] == target[i]) {
          attron(COLOR_PAIR(1));
          addch(target[i]);
          attroff(COLOR_PAIR(1));
        } else {
          attron(COLOR_PAIR(2));
          addch(target[i] == ' ' ? '_' : target[i]);
          attroff(COLOR_PAIR(2));
        }
      } else {
        addch(target[i]);
      }
    }
    refresh();
    napms(10);
  }

  endwin();
}
