# button_resistor_network

A small Arduino helper for reading multiple buttons through a single analog input using a resistor-ladder button network.

This project converts analog readings from a button resistor network into button IDs and press actions such as pressed, released, short press, long press, and canceled press.

It is intended for small Arduino-style projects where several buttons share one analog input pin.

## Project Status

Prototype / utility code.

This repo is intended as a lightweight helper for personal embedded projects, not a polished general-purpose input library.

## Target Hardware

This code is intended for:

* Arduino-compatible boards
* one analog input pin
* resistor-ladder button input circuit
* momentary pushbuttons

The current implementation reads from:

```cpp
analogRead(A0)
```

## What It Does

The helper tracks:

* which button is currently pressed
* which button was previously pressed
* whether the input state changed
* press start time
* press duration
* whether the press was short, long, canceled, or released

## Button IDs

The current button IDs are:

```cpp
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
```

## Action IDs

The current action IDs are:

```cpp
enum ACTION_id {
  ACTION_NOPRESS,
  ACTION_PRESSED,
  ACTION_SHORTPRESS,
  ACTION_LONGPRESS,
  ACTION_RELEASED,
  ACTION_CANCELED,
  ACTION_END_OF_LIST
};
```

## Default Analog Values

The default resistor-ladder center values are:

| Button | Analog Center Value |
| ------ | ------------------: |
| Select |                 640 |
| Left   |                 408 |
| Down   |                 255 |
| Up     |                  99 |
| Right  |                   0 |

Each button match window currently uses a margin of `±20`.

These values are based on the resistor network used during development. Your hardware may require different values.

## Basic Use

Include the header:

```cpp
#include <PhoenixJack_button_resistor_network.h>
```

Set up the expected analog values:

```cpp
button_array.setup_values();
```

Then call `check()` repeatedly from `loop()`:

```cpp
void loop() {
  if (button_array.check()) {
    // A button state changed.
    // Inspect button_array.currentid and button_array.currentaction.
  }
}
```

## Example

```cpp
#include <button_resistor_network.h>

void setup() {
  Serial.begin(9600);
  button_array.setup_values();
}

void loop() {
  if (button_array.check()) {
    Serial.print("Button: ");
    Serial.print(button_array.currentid);
    Serial.print(" Action: ");
    Serial.println(button_array.currentaction);
  }
}
```

## Press Behavior

The helper classifies released button presses based on how long the button was held.

| Hold Duration       | Action              |
| ------------------- | ------------------- |
| Less than 1 second  | `ACTION_SHORTPRESS` |
| 1 to 3 seconds      | `ACTION_LONGPRESS`  |
| More than 3 seconds | `ACTION_CANCELED`   |

## Read Interval

The helper throttles analog reads using an internal minimum read interval.

The current interval is:

```cpp
uint8_t _min_read_interval = 100;
```

This means the input is checked roughly every 100 ms.

## Related Repositories

* [`lcd_functions`](https://github.com/Phoenixjack/lcd_functions) - companion LCD helper that can display button and action status

## Known Limitations

* Prototype utility code
* Hard-coded analog input: `A0`
* Hard-coded button threshold values
* Hard-coded `±20` analog match margin
* Hard-coded press-duration thresholds
* Not debounced beyond read interval and analog threshold matching
* Assumes one button is pressed at a time
* No formal calibration routine
* No warranty or support commitment

## Possible Future Improvements

Possible future improvements:

* make the analog input pin configurable
* make button center values configurable at runtime
* make match margin configurable
* add calibration helper
* add optional serial diagnostics
* add example wiring diagram
* add resistor value recommendations
* add support for different button counts
* add `#pragma once` or include guards
* split into `.h` and `.cpp` if the helper grows

## License

This project is released under the MIT License.

You are free to use, modify, and adapt it for your own projects. No warranty is provided, and no ongoing support or maintenance is implied.
