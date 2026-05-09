#pragma once

#include <Arduino.h>

#if defined(SWITCH_AUTO_DRAW_USE_USB_HID)

#include "USB.h"
#include "USBHID.h"

#if !CONFIG_TINYUSB_HID_ENABLED
#error "TinyUSB HID is not enabled. Build for ESP32-S2/S3 with ARDUINO_USB_MODE=1."
#endif

// Minimal Nintendo Switch compatible USB HID gamepad helper.
// Based on the MIT-licensed esp32beans/switch_ESP32 report shape.

using SwitchDpadDirection = uint8_t;

constexpr SwitchDpadDirection SWITCH_DPAD_UP = 0;
constexpr SwitchDpadDirection SWITCH_DPAD_UP_RIGHT = 1;
constexpr SwitchDpadDirection SWITCH_DPAD_RIGHT = 2;
constexpr SwitchDpadDirection SWITCH_DPAD_DOWN_RIGHT = 3;
constexpr SwitchDpadDirection SWITCH_DPAD_DOWN = 4;
constexpr SwitchDpadDirection SWITCH_DPAD_DOWN_LEFT = 5;
constexpr SwitchDpadDirection SWITCH_DPAD_LEFT = 6;
constexpr SwitchDpadDirection SWITCH_DPAD_UP_LEFT = 7;
constexpr SwitchDpadDirection SWITCH_DPAD_CENTERED = 0x0f;

enum SwitchButton : uint8_t {
  SwitchButtonY = 0,
  SwitchButtonB,
  SwitchButtonA,
  SwitchButtonX,
  SwitchButtonL,
  SwitchButtonR,
  SwitchButtonZL,
  SwitchButtonZR,
  SwitchButtonMinus,
  SwitchButtonPlus,
  SwitchButtonLeftStick,
  SwitchButtonRightStick,
  SwitchButtonHome,
  SwitchButtonCapture,
};

struct __attribute__((packed, aligned(1))) SwitchGamepadReport {
  uint16_t buttons;
  uint8_t dpad;
  uint8_t left_x;
  uint8_t left_y;
  uint8_t right_x;
  uint8_t right_y;
  uint8_t filler;
};

class SwitchHidGamepad : public USBHIDDevice {
 public:
  SwitchHidGamepad();

  void begin();
  void reset();
  bool write();

  void buttons(uint16_t mask);
  void dpad(SwitchDpadDirection direction);
  void dpad(bool up, bool down, bool left, bool right);
  void leftStick(uint8_t x, uint8_t y);
  void centerSticks();

  uint16_t _onGetDescriptor(uint8_t *buffer) override;

 private:
  USBHID hid_;
  SwitchGamepadReport report_{};
};

constexpr uint16_t switchButtonMask(SwitchButton button) {
  return static_cast<uint16_t>(1u << static_cast<uint8_t>(button));
}

#endif  // defined(SWITCH_AUTO_DRAW_USE_USB_HID)
