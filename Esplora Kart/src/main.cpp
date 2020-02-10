#include <Arduino.h>
#include <Esplora.h>
#include <Keyboard.h>
#include <Mouse.h>

/* ESPLORA KART NOTES:
  Do not attempt to upload on PlatformIO, use the Arduino IDE instead
*/


class Controller {

private:
  // Last sensed state of each of the buttons
  bool buttonStates[8];
  // Names of the buttons being read.
  const byte buttons[8] = {
    // Joystick = Thumbstick
    JOYSTICK_DOWN,  // Brake / Backwards
    JOYSTICK_LEFT,  // Left
    JOYSTICK_UP,    // Forward
    JOYSTICK_RIGHT, // Right
    // Switch = Button
    SWITCH_RIGHT,  // Fire
    SWITCH_LEFT,   // Bend
    SWITCH_UP,     // Nitro
    SWITCH_DOWN,   // Look Back
  };
  // Keystroke (what needs to be send to the computer when a button is pressed)
  const char keystrokes[8] = {
    // Keyboard commands that will perform the actions
    KEY_DOWN_ARROW,   // Brake / go backwards
    KEY_LEFT_ARROW,   // Left
    KEY_UP_ARROW,     // Forward
    KEY_RIGHT_ARROW,  // Right
    ' ',              // Fire
    'V',              // Bend
    'N',              // Nitro
    'B'               // Look Back
  };

public:
  // class methods
  void check_buttons();
  void press_buttons(bool newState, int thisButton);
};

Controller controller;

void setup() {
  // Initialize the virtual USB keyboard
  Keyboard.begin(); 
}

void loop() {
  controller.check_buttons();
}

void Controller::check_buttons() {
  // Checks if any of the buttons changed state
  for (int thisButton = 0; thisButton < 8; thisButton++) {
    bool lastState = buttonStates[thisButton];
    bool newState = Esplora.readButton(buttons[thisButton]);
    // Checks if the previous state is the same as the new state
    if (lastState != newState) {
      controller.press_buttons(newState, thisButton);
    }
    // Store the new button state, so you can sense a difference later:
    buttonStates[thisButton] = newState;
  }
  // Small delay so that the program does not overflow
  delay(50);
}

void Controller::press_buttons(bool newState, int thisButton) {
  // Performs a button press or release
  if (newState == PRESSED) {
    // Press
    Keyboard.press(keystrokes[thisButton]);
  } 
  else if (newState == RELEASED) {
    // Release
    Keyboard.release(keystrokes[thisButton]);
  }
}