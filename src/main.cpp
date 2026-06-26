#include <ncurses.h>
#include <string>

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  bool running = true;

  while (running) {
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::string title = "=== TactTUI ===";
    std::string opt1 = "1. Use System Dictionary";
    std::string opt2 = "2. Use Custom Book";
    std::string optq = "q. Quit";
    std::string prompt = "Select an option: ";

    int start_y = (max_y - 7) / 2;
    mvprintw(start_y, (max_x - title.length()) / 2, "%s", title.c_str());
    mvprintw(start_y + 2, (max_x - opt1.length()) / 2, "%s", opt1.c_str());
    mvprintw(start_y + 3, (max_x - opt2.length()) / 2, "%s", opt2.c_str());
    mvprintw(start_y + 4, (max_x - optq.length()) / 2, "%s", optq.c_str());
    mvprintw(start_y + 6, (max_x - prompt.length()) / 2, "%s", prompt.c_str());

    refresh();

    int ch = getch();

    clear();

    std::string result_msg;
    std::string detail_msg = "";

    switch (ch) {
    case '1':
      result_msg = "You selected: System Dictionary.";
      detail_msg = "Initializing dictionary provider...";
      break;
    case '2':
      result_msg = "You selected: Custom Book.";
      detail_msg = "Loading text from book...";
      break;
    case 'q':
    case 'Q':
      result_msg = "Shutting down TactTUI...";
      running = false;
      break;
    default:
      result_msg = "Unrecognized input. Please try again.";
      break;
    }

    // Center the result messages
    mvprintw(max_y / 2, (max_x - result_msg.length()) / 2, "%s",
             result_msg.c_str());
    if (!detail_msg.empty()) {
      mvprintw((max_y / 2) + 1, (max_x - detail_msg.length()) / 2, "%s",
               detail_msg.c_str());
    }
    if (running) {
      std::string cont_msg = "Press any key to return to the menu...";
      mvprintw((max_y / 2) + 3, (max_x - cont_msg.length()) / 2, "%s",
               cont_msg.c_str());
      refresh();
      getch();
    }
  }

  endwin();

  return 0;
}
