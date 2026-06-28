#include "tui.hpp"
#include <ncurses.h>

TUI::TUI() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  start_color();
  use_default_colors();
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_RED, -1);
}

TUI::~TUI() { endwin(); }

AppConfig TUI::show_menu() {
  AppConfig config = {0, 0, false};
  nodelay(stdscr, FALSE);

  std::string title1 = "=== Select Text Source ===";
  std::string opt1_1 = "1. System Dictionary";
  std::string opt1_2 = "2. Custom Book";

  std::string title2 = "=== Select Duration ===";
  std::string opt2_1 = "1. 10 Seconds";
  std::string opt2_2 = "2. 30 Seconds";
  std::string opt2_3 = "3. 60 Seconds";

  std::string quit_msg = "Press 'q' or ESC to quit.";

  while (true) {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int start_y = (max_y - 6) / 2;

    mvprintw(start_y, (max_x - title1.length()) / 2, "%s", title1.c_str());
    mvprintw(start_y + 2, (max_x - opt1_1.length()) / 2, "%s", opt1_1.c_str());
    mvprintw(start_y + 3, (max_x - opt1_2.length()) / 2, "%s", opt1_2.c_str());
    mvprintw(start_y + 5, (max_x - quit_msg.length()) / 2, "%s",
             quit_msg.c_str());

    refresh();

    int ch = getch();
    if (ch == '1') {
      config.provider_type = 1;
      break;
    }
    if (ch == '2') {
      config.provider_type = 2;
      break;
    }
    if (ch == 'q' || ch == 27) {
      config.quit = true;
      return config;
    }
  }

  while (true) {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int start_y = (max_y - 7) / 2;

    mvprintw(start_y, (max_x - title2.length()) / 2, "%s", title2.c_str());
    mvprintw(start_y + 2, (max_x - opt2_1.length()) / 2, "%s", opt2_1.c_str());
    mvprintw(start_y + 3, (max_x - opt2_2.length()) / 2, "%s", opt2_2.c_str());
    mvprintw(start_y + 4, (max_x - opt2_3.length()) / 2, "%s", opt2_3.c_str());
    mvprintw(start_y + 6, (max_x - quit_msg.length()) / 2, "%s",
             quit_msg.c_str());

    refresh();

    int ch = getch();
    if (ch == '1') {
      config.duration = 10;
      break;
    }
    if (ch == '2') {
      config.duration = 30;
      break;
    }
    if (ch == '3') {
      config.duration = 60;
      break;
    }
    if (ch == 'q' || ch == 27) {
      config.quit = true;
      return config;
    }
  }

  return config;
}

void TUI::run_test(Engine &engine) {
  nodelay(stdscr, TRUE);
  engine.start();

  while (engine.is_running()) {
    engine.update();

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

    std::string target = engine.get_target_text();
    std::string input = engine.get_user_input();
    GameStats stats = engine.get_current_stats();
    int time_left = static_cast<int>(engine.get_time_remaining());

    mvprintw(0, 2, "Time: %02ds | WPM: %d | Accuracy: %.1f%%", time_left,
             stats.wpm, stats.accuracy);
    mvprintw(0, max_x - 20, "Press ESC to Cancel");

    int box_width = std::min(60, max_x - 10);
    if (box_width < 20)
      box_width = 20;
    int start_x = (max_x - box_width) / 2;

    int lines = 1;
    int cur_x = 0;
    for (size_t i = 0; i < target.size(); ++i) {
      if (i == 0 || target[i - 1] == ' ') {
        size_t next_space = target.find(' ', i);
        int word_len =
            (next_space == std::string::npos ? target.size() : next_space) - i;

        if (cur_x + word_len > box_width && cur_x > 0) {
          lines++;
          cur_x = 0;
        }
      }
      cur_x++;
    }

    int start_y = (max_y - lines) / 2;
    int current_y = start_y;
    int current_x = start_x;

    int cursor_y = start_y;
    int cursor_x = start_x;

    for (size_t i = 0; i < target.size(); ++i) {
      if (i == 0 || target[i - 1] == ' ') {
        size_t next_space = target.find(' ', i);
        int word_len =
            (next_space == std::string::npos ? target.size() : next_space) - i;

        if ((current_x - start_x) + word_len > box_width &&
            (current_x - start_x) > 0) {
          current_y++;
          current_x = start_x;
        }
      }

      if (i < input.size()) {
        if (input[i] == target[i]) {
          attron(COLOR_PAIR(1));
          mvaddch(current_y, current_x, target[i]);
          attroff(COLOR_PAIR(1));
        } else {
          attron(COLOR_PAIR(2));
          mvaddch(current_y, current_x, target[i] == ' ' ? '_' : target[i]);
          attroff(COLOR_PAIR(2));
        }
      } else {
        mvaddch(current_y, current_x, target[i]);
      }

      if (i == input.size()) {
        cursor_y = current_y;
        cursor_x = current_x;
      }

      current_x++;
    }

    if (input.size() < target.size()) {
      attron(A_REVERSE);
      mvaddch(cursor_y, cursor_x, target[input.size()]);
      attroff(A_REVERSE);
    }

    refresh();
    napms(10);
  }
}

void TUI::show_results(const GameStats &stats) {
  nodelay(stdscr, FALSE);
  clear();

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  std::string title = "=== Test Complete ===";
  mvprintw((max_y / 2) - 2, (max_x - title.length()) / 2, "%s", title.c_str());
  mvprintw((max_y / 2), (max_x - 20) / 2, "Final WPM: %d", stats.wpm);
  mvprintw((max_y / 2) + 1, (max_x - 20) / 2, "Accuracy:  %.1f%%",
           stats.accuracy);

  std::string prompt = "Press ENTER or ESC to return to menu...";
  mvprintw((max_y / 2) + 4, (max_x - prompt.length()) / 2, "%s",
           prompt.c_str());
  refresh();
  flushinp();

  while (true) {
    int ch = getch();
    if (ch == 10 || ch == '\n' || ch == 27) {
      break;
    }
  }
}
