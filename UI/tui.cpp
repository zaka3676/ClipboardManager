#include "../JSONmodel/historyJSON.hpp"
#include <ncurses.h>
#include <string>
#include <unistd.h> // For usleep

#if __APPLE__ 
  #include "../OSSpecific/darwin.hpp"
#endif

HistoryJSON history;
bool isView = false;
int selectedRow = 0;
WINDOW *topBar;
WINDOW *Window;
WINDOW *bottomBar;
std::vector<HistoryEntry> entries;


bool compareDates(const HistoryEntry& entry1, const HistoryEntry& entry2) {
    // Custom comparison function to compare dates
    return entry1.getDate() > entry2.getDate(); // ">" for descending order
}

void helpMenu() {
  int row, col;
  getmaxyx(bottomBar, row, col);
  std::string bottomMessage;
  if (isView) {
    bottomMessage = "ESC/q/v - CLOSE VIEW   c - COPY   d - DELETE   r - RELOAD";
  } else {
    bottomMessage = "ESC/q - EXIT   c - COPY   v - VIEW   d - DELETE   r - RELOAD";
  }
  wclear(bottomBar);
  box(bottomBar, 0,0);
  int bottomMessageX = (col - bottomMessage.length()) / 2;
  mvwprintw(bottomBar, 1, bottomMessageX, bottomMessage.c_str());
  wrefresh(bottomBar);
}

void drawTable() {
    auto win = Window;

    int row, col;
    getmaxyx(win, row, col);

    wclear(win);
    box(win, 0, 0);

    mvwprintw(win, 0, 2, "Content");
    mvwprintw(win, 0, col/2 + 2, "Date");

    for(size_t i = 0; i < entries.size(); ++i) {
        if (i == selectedRow) {
            wattron(win, A_REVERSE);
        }

        // Truncate shortContent if it's too long
        std::string truncatedContent = entries[i].getContent();
        if (truncatedContent.size() > col/2 - 4) { // Adjust based on column width
            truncatedContent = truncatedContent.substr(0, col/2 - 7) + "...";
        }

        // Replace '\n' with a space
        std::replace(truncatedContent.begin(), truncatedContent.end(), '\n', ' ');

        mvwprintw(win, i+1, 2, truncatedContent.c_str());
        mvwprintw(win, i+1, col/2 + 2, entries[i].getDate().c_str());
        if (i == selectedRow) {
            wattroff(win, A_REVERSE);
        }
    wrefresh(win);
    }
}

void showView(bool opt = isView) {
  if (opt) {
    isView = false;
    helpMenu();
    drawTable();
  } else {
    isView = true;
    wclear(Window);
    box(Window, 0, 0);
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type end = 0;

    mvwprintw(Window, 0, 2, entries[selectedRow].getDate().c_str());
    helpMenu();

    // Code from chatGPT to convert every line to strings in vector, replace soon 
    while ((end = entries[selectedRow].getContent().find('\n', start)) != std::string::npos) {
        result.push_back(entries[selectedRow].getContent().substr(start, end - start));
        start = end + 1;
    }

    // Add the last part of the string (after the last '\n')
    result.push_back(entries[selectedRow].getContent().substr(start));
    // end of code from chatGPT
    
    int row, col;
    getmaxyx(Window, row, col);

    for (int v = 0; v < result.size(); ++v) {
      std::string i = result[v];
      if (v < row - 2) {
        mvwprintw(Window, v + 1, 1, i.c_str());
      }
    }

    // mvwprintw(Window, 1, 1, entries[selectedRow].getContent().c_str()); 
    wrefresh(Window);
  }
}

void reload() {
    entries = history.loadHistory();
    // Sort entries by date
    std::sort(entries.begin(), entries.end(), compareDates);
    if (isView) {
      showView(false);
    } else {
      drawTable();
    }
}

void redraw() {
    int windowHeight, windowWidth;
    int topBarHeight = 3; // Height of the top bar
    int bottomBarHeight = 3; // Height of the bottom bar

    if (topBar != NULL) {
        delwin(topBar);
    }
    if (Window != NULL) {
        delwin(Window);
    } 
    if (bottomBar != NULL) {
        delwin(bottomBar);
    }

    getmaxyx(stdscr, windowHeight, windowWidth);

    // Create three windows
    topBar = newwin(topBarHeight, windowWidth, 0, 0);
    Window = newwin(windowHeight - topBarHeight - bottomBarHeight, windowWidth, topBarHeight, 0);
    bottomBar = newwin(bottomBarHeight, windowWidth, windowHeight - bottomBarHeight, 0);

    // Draw a box around each window
    box(topBar, 0, 0);

    // Centered text on the top bar
    std::string message = "Kontik's Clipboard Manager";
    int x = (windowWidth - message.length()) / 2;
    mvwprintw(topBar, 1, x, message.c_str()); 

    reload();
    helpMenu();

    // Refresh all windows
    wrefresh(topBar);
}

int DisplayHistory() {
    initscr(); // Initialize ncurses
    noecho();  // Don't echo input
    curs_set(FALSE); // Hide cursor
    keypad(stdscr, TRUE); // Enable special keys
    timeout(10); // Non-blocking input

    refresh();
    redraw();

    while (1) {
        int ch = getch();
        if (ch == KEY_RESIZE) {
            refresh();
            redraw();
        } else if (ch == 27 || ch == 113) {
          if (!isView) {
            break;
          } else {
            showView();
          }
        } else if (ch == KEY_UP) {
          if (selectedRow > 0) {
            selectedRow--;
            if (isView) {  
              showView(false);
            } else {
              drawTable();
            }
          }
        } else if (ch == KEY_DOWN) {
            if (selectedRow < entries.size() - 1) {
                selectedRow++;
                if (isView) {
                  showView(false);
                } else {
                  drawTable();
                }
            }
        } else if (ch == 114) {
            // r character logic here
            reload();
        } else if (ch == 118) {
            // v character logic here
            showView();
        } else if (ch == 99) {
            // c character logic here
            SetClipboard(entries[selectedRow].getContent());
        } else if (ch == 100) {
          // d character logic here
          history.removeHistoryEntry(entries[selectedRow].getDate());
          history.saveHistory();
          reload();
        }


        usleep(10000); // Sleep for 0.01 sec to avoid 100% CPU usage
    }

    endwin(); // Clean up ncurses
    return 0;
}
