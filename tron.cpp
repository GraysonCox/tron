#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#include "character.h"

#define GAME_Y 21
#define GAME_X 80
#define GAME_BORDER 'X'
#define GAME_SPEED 200000


const char* logo[] =
  {"8888888 8888888888 8 888888888o.      ,o888888o.     8 b.             8",
   "      8 8888       8 8888    `88.  . 8888     `88.   8 888o.          8",
   "      8 8888       8 8888     `88 ,8 8888       `8b  8 Y88888o.       8",
   "      8 8888       8 8888     ,88 88 8888        `8b 8 .`Y888888o.    8",
   "      8 8888       8 8888.   ,88\' 88 8888         88 8 8o. `Y888888o. 8",
   "      8 8888       8 888888888P\'  88 8888         88 8 8`Y8o. `Y88888o8",
   "      8 8888       8 8888`8b      88 8888        ,8P 8 8   `Y8o. `Y8888",
   "      8 8888       8 8888 `8b.    `8 8888       ,8P  8 8      `Y8o. `Y8",
   "      8 8888       8 8888   `8b.   ` 8888     ,88\'   8 8         `Y8o.`",
   "      8 8888       8 8888     `88.    `8888888P\'     8 8            `Yo"};

void init_terminal() {
  initscr();
  raw();
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  srand(time(0));
}

void reset_terminal() {
  endwin();
}

void print_border(character *p1, character *p2) {
  int i;
  clear();

  attron(A_BOLD);
  attron(COLOR_PAIR(COLOR_BLUE));
  mvaddch(GAME_Y + 1, GAME_X / 2 - 2, '0' + p2->get_score());
  attroff(COLOR_PAIR(COLOR_BLUE));
  attron(COLOR_PAIR(COLOR_RED));
  mvaddch(GAME_Y + 1, GAME_X / 2 + 2, '0' + p1->get_score());
  attroff(COLOR_PAIR(COLOR_RED));
  attroff(A_BOLD);
  refresh();

  for (i = 0; i < GAME_X; i++) {
    mvaddch(0, i, GAME_BORDER);
    mvaddch(GAME_Y - 1, GAME_X - 1 - i, GAME_BORDER);
    if (i < GAME_Y - 1) {
      mvaddch(i, 0, GAME_BORDER);
      mvaddch(GAME_Y - 1 - i, GAME_X - 1, GAME_BORDER);
    }
    refresh();
    usleep(10000);
  }
}

int show_title_menu() {
  int logo_y = 3,
    logo_x = 4;
  int i, j;
  attron(A_BOLD);
  attron(COLOR_PAIR(COLOR_BLUE));
  for (j = 0; j < 10; j++) {
    for (i = 0; i < 71; i++) {
      mvaddch(j + logo_y, i + logo_x, logo[j][i]);
      refresh();
    }
    usleep(100000);
  }
  attroff(COLOR_PAIR(COLOR_BLUE));
  attroff(A_BOLD);
  mvprintw(j + logo_y + 2, GAME_X / 2 - 5, "1 PLAYER");
  mvprintw(j + logo_y + 3, GAME_X / 2 - 5, "2 PLAYERS");
  mvprintw(j + logo_y + 4, GAME_X / 2 - 5, "QUIT");
  int cursor_x = GAME_X / 2 - 6,
    cursor_y = j + logo_y + 2;
  mvaddch(j + logo_y + 2, GAME_X / 2 - 6, '>');  
  while (true) {
    switch (getch()) {
    case KEY_UP:
      if (cursor_y > j + logo_y + 2) {
	mvaddch(cursor_y, cursor_x, ' ');
	cursor_y--;
	mvaddch(cursor_y, cursor_x, '>');
      }
      break;
    case KEY_DOWN:
      if (cursor_y < j + logo_y + 4) {
	mvaddch(cursor_y, cursor_x, ' ');
	cursor_y++;
	mvaddch(cursor_y, cursor_x, '>');
      }
      break;
    case ' ':
    case KEY_ENTER:
      if (cursor_y == j + logo_y + 2) {
	return 1;
      } else if (cursor_y == j + logo_y + 3) {
	return 2;
      } else {
	return 0;
      }
    case ERR:
      break;
    }
    refresh();
  }
}

void do_explosion(character *loser) {
  //int j, i;
  attron(A_BOLD);
  attron(COLOR_PAIR(COLOR_RED));
  mvaddch(loser->get_y(), loser->get_x(), 'X');
  refresh();
  usleep(100000);
  mvaddch(loser->get_y() - 1, loser->get_x(), 'X');
  mvaddch(loser->get_y() + 1, loser->get_x(), 'X');
  mvaddch(loser->get_y(), loser->get_x() - 1, 'X');
  mvaddch(loser->get_y(), loser->get_x() + 1, 'X');
  attroff(COLOR_PAIR(COLOR_RED));
  attron(COLOR_PAIR(COLOR_YELLOW));
  mvaddch(loser->get_y(), loser->get_x(), 'X');
  attroff(COLOR_PAIR(COLOR_YELLOW));
  refresh();
  attroff(A_BOLD);
  usleep(100000);
  mvaddch(loser->get_y(), loser->get_x(), ' ');
  mvaddch(loser->get_y() - 1, loser->get_x(), ' ');
  mvaddch(loser->get_y() + 1, loser->get_x(), ' ');
  mvaddch(loser->get_y(), loser->get_x() - 1, ' ');
  mvaddch(loser->get_y(), loser->get_x() + 1, ' ');
  refresh();
  usleep(1000000);
}

int show_end(character *p1, character *p2) {
  delete p1;
  delete p2;

  clear();
  attron(A_BOLD);
  if (p1->get_score() > p2->get_score()) {
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(GAME_Y / 2, GAME_X / 2 - 6, "PLAYER 1 WINS");
    attroff(COLOR_PAIR(COLOR_RED));
  } else {
    attron(COLOR_PAIR(COLOR_BLUE));
    mvprintw(GAME_Y / 2, GAME_X / 2 - 6, "PLAYER 2 WINS");
    attroff(COLOR_PAIR(COLOR_BLUE));
  }
  attroff(A_BOLD);

  mvprintw(GAME_Y / 2 + 2, GAME_X / 2 - 7, "SPACE - restart");
  mvprintw(GAME_Y / 2 + 3, GAME_X / 2 - 3, "q - quit");

  while (true) {
    switch (getch()) {
    case 'q':
      reset_terminal();
      return 0;
    case ' ':
      return 1;
    }
  }
}

int move_player_return_collision(character *c, char map[][GAME_X]) {
  attron(COLOR_PAIR(c->get_color()));
  mvaddch(c->get_y(), c->get_x(), c->get_trail()); // Print char
  map[c->get_y()][c->get_x()] = c->get_trail(); // Add to char map
  attroff(COLOR_PAIR(c->get_color()));

  c->move();

  if (map[c->get_y()][c->get_x()] != ' ') {
    do_explosion(c);
    return 1;
  } else {
    attron(A_BOLD);
    attron(COLOR_PAIR(c->get_color()));
    mvaddch(c->get_y(), c->get_x(), c->get_symbol());
    attroff(COLOR_PAIR(c->get_color()));
    attroff(A_BOLD);
  }
  return 0;
}



int handle_single_player_input(character *p, int input) {
  switch (input) {
  case ERR:
    break;
  case KEY_UP:
    p->set_direction(dir_up);
    break;
  case KEY_DOWN:
    p->set_direction(dir_down);
    break;
  case KEY_LEFT:
    p->set_direction(dir_left);
    break;
  case KEY_RIGHT:
    p->set_direction(dir_right);
    break;
  case '/':
    p->set_is_boosting(1);
    break;
  case 'Q':
    return 1;
  }
  return 0;
}

int handle_multi_player_input(character *p1, character *p2, int input) {
  switch (input) {
  case ERR:
    break;
  case KEY_UP:
    p1->set_direction(dir_up);
    break;
  case KEY_DOWN:
    p1->set_direction(dir_down);
    break;
  case KEY_LEFT:
    p1->set_direction(dir_left);
    break;
  case KEY_RIGHT:
    p1->set_direction(dir_right);
    break;
  case '/':
    p1->set_is_boosting(1);
    break;
  case 'w':
    p2->set_direction(dir_up);
    break;
  case 's':
    p2->set_direction(dir_down);
    break;
  case 'a':
    p2->set_direction(dir_left);
    break;
  case 'd':
    p2->set_direction(dir_right);
    break;
  case 'e':
    p2->set_is_boosting(1);
    break;
  case 'Q':
    return 1;
  }
  return 0;
}

void handle_control_computer(character *c, char map[][GAME_X]) {
  int y = c->get_y();
  int x = c->get_x();

  if (rand() % 10000000 == 0) {
    switch (rand() % 4) {
    case 0:
      c->set_direction(dir_up);
      break;
    case 1:
      c->set_direction(dir_down);
      break;
    case 2:
      c->set_direction(dir_left);
      break;
    case 3:
      c->set_direction(dir_right);
      break;
    }
  }

  switch (c->get_direction()) {
  case dir_up:
    if (map[y - 1][x] != ' ') {
      if (x < GAME_X / 2) {
	c->set_direction(dir_right);
      } else {
	c->set_direction(dir_left);
      }
    }
    break;
  case dir_down:
    if (map[y + 1][x] != ' ') {
      if (x < GAME_X / 2) {
	c->set_direction(dir_right);
      } else {
	c->set_direction(dir_left);
      }
    }
    break;
  case dir_left:
    if (map[y][x - 1] != ' ') {
      if (y < GAME_Y / 2) {
	c->set_direction(dir_down);
      } else {
	c->set_direction(dir_up);
      }
    }
    break;
  case dir_right:
    if (map[y][x + 1] != ' ') {
      if (y < GAME_Y / 2) {
	c->set_direction(dir_down);
      } else {
	c->set_direction(dir_up);
      }
    }
    break;
  }
}

int start_game_single_player(character *PC, character *NPC, char map[][GAME_X]) {
  int t;

  print_border(PC, NPC);

  for (t = 0; true; t++) {
    // Get input
    if (handle_single_player_input(PC, getch())) {
      return 1; // Quit if returns something non-zero
    }
    handle_control_computer(NPC, map);

    // Update screen
    if (t % (GAME_SPEED - CHARACTER_BOOST_SPEED * PC->get_is_boosting()) == 0) {
      if (move_player_return_collision(PC, map)) {
	NPC->increment_score();
	break;
      }
    }
    if (t % (GAME_SPEED - CHARACTER_BOOST_SPEED * NPC->get_is_boosting()) == 0) {
      if (move_player_return_collision(NPC, map)) {
	PC->increment_score();
	break;
      }
    }
  }
  return 0;
}

int start_game_multi_player(character *PC1, character *PC2, char map[][GAME_X]) {
  int t;

  print_border(PC1, PC2);

  for (t = 0; true; t++) {
    // Get input
    if (handle_multi_player_input(PC1, PC2, getch())) {
      return 1; // Quit if returns something non-zero
    }

    // Update screen
    if (t % (GAME_SPEED - CHARACTER_BOOST_SPEED * PC1->get_is_boosting()) == 0) {
      if (move_player_return_collision(PC1, map)) {
	PC2->increment_score();
	break;
      }
    }
    if (t % (GAME_SPEED - CHARACTER_BOOST_SPEED * PC2->get_is_boosting()) == 0) {
      if (move_player_return_collision(PC2, map)) {
	PC1->increment_score();
	break;
      }
    }
  }

  return 0;
}

void init_map(char map[][GAME_X]) {
  int j, i;
  for (j = 0; j < GAME_Y; j++) {
    for (i = 0; i < GAME_X; i++) {
      if (j != 0 && j != GAME_Y - 1 && i != 0 && i != GAME_X - 1) {
	map[j][i] = ' ';
      } else {
	map[j][i] = GAME_BORDER;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int game_mode;

  init_terminal();

  game_mode = show_title_menu();
  //game_mode = 1;
  if (game_mode == 0) {
    reset_terminal();
    return 0;
  }
  character *player1;
  character *player2;
  char map[GAME_Y][GAME_X];

  do {
    player1 = new character(COLOR_RED, GAME_Y / 2, 3 * GAME_X / 4);
    player2 = new character(COLOR_BLUE, GAME_Y / 2, GAME_X / 4);
    while (player1->get_score() < 3 && player2->get_score() < 3) {
      player1->set_x(3 * GAME_X / 4);
      player1->set_y(GAME_Y / 2);
      player1->set_direction(dir_left);
      player2->set_x(GAME_X / 4);
      player2->set_y(GAME_Y / 2);
      player2->set_direction(dir_right);
      init_map(map);
      switch (game_mode) {
      case 1:
	if (start_game_single_player(player1, player2, map)) {
	  reset_terminal();
	  return 0;
	}
	break;
      case 2:
	if (start_game_multi_player(player1, player2, map)) {
	  reset_terminal();
	  return 0;
	}
	break;
      }
    }
  } while (show_end(player1, player2));
  return 0;
} 
