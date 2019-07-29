#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

#define CHARACTER_BOOST_DURATION 25

typedef enum direction {
  dir_up,
  dir_down,
  dir_left,
  dir_right
} direction_t;

class character {
 protected:
  int x, y;
  direction_t dir;
  uint32_t color;
  int boost_left;
  int score;
 public:
 character() : dir(dir_up), color(0), score(0) {}
  character(uint32_t color, int y, int x) {
    this->dir = dir_up;
    this->color = color;
    this->x = x;
    this->y = y;
    this->score = 0;
    this->boost_left = 0;
  }
  direction_t get_direction() { return dir; }
  void set_direction(direction_t dir) { this->dir = dir; }
  int get_x() { return this->x; }
  void set_x(int x) { this->x = x; }
  int get_y() { return this->y; }
  void set_y(int y) { this->y = y; }
  char get_symbol() {
    switch (dir) {
    case dir_up:
      return '^';
    case dir_down:
      return 'v';
    case dir_left:
      return '<';
    case dir_right:
      return '>';
    }
    return '0'; // Error symbol
  }
  char get_trail() { return '#'; }
  uint32_t get_color() { return this->color; }
  void set_color() { this->color = color; }
  int get_score() { return this->score; }
  void increment_score() { this->score++; }
  int get_is_boosting() { return this->boost_left != 0; }
  void set_is_boosting(int is_boosting) { if (is_boosting) this->boost_left = CHARACTER_BOOST_DURATION; }
  void move() {
    switch (this->dir) {
    case dir_up:
      this->y--;
      break;
    case dir_down:
      this->y++;
      break;
    case dir_left:
      this->x--;
      break;
    case dir_right:
      this->x++;
      break;
    }
    if (this->boost_left != 0) {
      this->boost_left--;
    }
  }
};

#endif
