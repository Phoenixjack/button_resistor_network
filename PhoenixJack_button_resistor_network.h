#include <stdint.h>
#include "Arduino.h"
enum ACTION_id {
  ACTION_NOPRESS,
  ACTION_PRESSED,
  ACTION_SHORTPRESS,
  ACTION_LONGPRESS,
  ACTION_RELEASED,
  ACTION_CANCELED,
  ACTION_END_OF_LIST
};
enum BUTTON_id {
  ID_SELECT,
  ID_LEFT,
  ID_DOWN,
  ID_UP,
  ID_RIGHT,
  ID_END_OF_LIST,
  ID_TOO_SOON,
  ID_NO_MATCH,
  ID_NO_PRESS
};

struct button_array {
private:
  uint8_t _min_read_interval = 100;
  struct this_button {
  private:
    uint16_t _lowvalue = 0;
    uint16_t _highvalue = 0;
  public:
    void set_values(uint16_t centervalue) {
      uint16_t margin = 20;
      if (centervalue < margin) { centervalue = margin; }
      if (centervalue > 1023 - margin) { centervalue = 1023 - margin; }
      _lowvalue = centervalue - margin;
      _highvalue = centervalue + margin;
    };
    bool match(uint16_t thisvalue) {
      return ((thisvalue >= _lowvalue) && (thisvalue <= _highvalue));
    };
  } this_button[ID_END_OF_LIST];
public:
  ACTION_id lastaction = ACTION_NOPRESS;
  ACTION_id currentaction = ACTION_NOPRESS;
  BUTTON_id lastid = ID_END_OF_LIST;
  BUTTON_id currentid = ID_END_OF_LIST;
  bool state_change = false;
  unsigned long holdDuration = 0;
  unsigned long press_start_time = 0;
  unsigned long nextcheck = 0;
  bool pressed = false;
  void setup_values() {
    this_button[ID_SELECT].set_values(640);
    this_button[ID_LEFT].set_values(408);
    this_button[ID_DOWN].set_values(255);
    this_button[ID_UP].set_values(99);
    this_button[ID_RIGHT].set_values(0);
  };
  BUTTON_id id_of_current_value(int value_to_match) {
    int lookupIndex = 0;
    bool match_not_found = true;
    while (match_not_found && lookupIndex < ID_END_OF_LIST) {
      if (this_button[lookupIndex].match((uint16_t)value_to_match)) {
        match_not_found = false;
      } else {
        lookupIndex++;
      }
    }
    if (match_not_found) {
      return ID_END_OF_LIST;
    } else {
      return (BUTTON_id)lookupIndex;
    }
  };
  bool check() {  // yes, I could shorten the code, BUT it'd be harder to read
    if (millis() > nextcheck) {
      lastid = currentid;
      currentid = id_of_current_value(analogRead(A0));
      if (currentid < ID_END_OF_LIST) {
        pressed = true;
      } else {
        pressed = false;
      }
      if (currentid == lastid) {
        state_change = false;
      } else {
        state_change = true;
        if (pressed) {
          press_start_time = millis();
          currentaction = ACTION_PRESSED;
        } else {
          holdDuration = millis() - press_start_time;
          if (holdDuration < 1000) {
            currentaction = ACTION_SHORTPRESS;
          } else if (holdDuration < 3000) {
            currentaction = ACTION_LONGPRESS;
          } else {
            currentaction = ACTION_CANCELED;
          }
        }
      }
      nextcheck = millis() + (uint16_t)_min_read_interval;
    } else {
      state_change = false;
    }
    return state_change;
  };
} button_array;
