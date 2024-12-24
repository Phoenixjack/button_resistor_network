#include "PhoenixJack_button_resistor_network.h"

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n\n\nAnalog resistor network test");
  button_array.setup_values();
}

void loop() {
  if (button_array.check()) {
    if (button_array.pressed) {
      printbutton();
    } else {
      printaction();
    }
  }
}

void printbutton() {
  String label = "\nButton: ";
  switch (button_array.currentid) {
    case ID_DOWN:
      label += "DOWN";
      break;
    case ID_LEFT:
      label += "LEFT";
      break;
    case ID_RIGHT:
      label += "RIGHT";
      break;
    case ID_UP:
      label += "UP";
      break;
    case ID_SELECT:
      label += "SELECT";
      break;
    case ID_TOO_SOON:
      label += "TOO SOON";
      break;
    case ID_NO_MATCH:
      label += "NO MATCH";
      break;
    case ID_END_OF_LIST:
      label += "END OF LIST";
      break;
    case ID_NO_PRESS:
      label += "NO PRESS";
      break;
    default:
      label += "default";
      break;
  }
  Serial.print(label);
}

void printaction() {
  String label = "\tACTION: ";
  switch (button_array.currentaction) {
    case ACTION_NOPRESS:
      label += "NO PRESS";
      break;
    case ACTION_LONGPRESS:
      label += "LONG PRESS";
      break;
    case ACTION_PRESSED:
      label += "PRESSED";
      break;
    case ACTION_CANCELED:
      label += "CANCELED";
      break;
    case ACTION_SHORTPRESS:
      label += "SHORT PRESS";
      break;
    case ACTION_END_OF_LIST:
      label += "END OF LIST";
      break;
    case ACTION_RELEASED:
      label += "RELEASED";
      break;
    default:
      label += "default";
      break;
  }
  Serial.print(label);
}