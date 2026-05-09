#include "switch_hid_gamepad.h"

#if defined(SWITCH_AUTO_DRAW_USE_USB_HID)

#include <string.h>

namespace {

// Gamepad report descriptor compatible with the common Hori Pokken style
// Nintendo Switch USB controller profile:
// 14 buttons, one 8-way hat switch, and two analog sticks.
const uint8_t kSwitchReportDescriptor[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x05,        // Usage (Game Pad)
    0xa1, 0x01,        // Collection (Application)
    0x15, 0x00,        // Logical Minimum (0)
    0x25, 0x01,        // Logical Maximum (1)
    0x35, 0x00,        // Physical Minimum (0)
    0x45, 0x01,        // Physical Maximum (1)
    0x75, 0x01,        // Report Size (1)
    0x95, 0x0e,        // Report Count (14)
    0x05, 0x09,        // Usage Page (Button)
    0x19, 0x01,        // Usage Minimum (1)
    0x29, 0x0e,        // Usage Maximum (14)
    0x81, 0x02,        // Input (Data, Variable, Absolute)
    0x95, 0x02,        // Report Count (2)
    0x81, 0x01,        // Input (Constant)
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x25, 0x07,        // Logical Maximum (7)
    0x46, 0x3b, 0x01,  // Physical Maximum (315)
    0x75, 0x04,        // Report Size (4)
    0x95, 0x01,        // Report Count (1)
    0x65, 0x14,        // Unit (English Rotation)
    0x09, 0x39,        // Usage (Hat switch)
    0x81, 0x42,        // Input (Data, Variable, Absolute, Null State)
    0x65, 0x00,        // Unit (None)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x01,        // Input (Constant)
    0x26, 0xff, 0x00,  // Logical Maximum (255)
    0x46, 0xff, 0x00,  // Physical Maximum (255)
    0x09, 0x30,        // Usage (X)
    0x09, 0x31,        // Usage (Y)
    0x09, 0x32,        // Usage (Z)
    0x09, 0x35,        // Usage (Rz)
    0x75, 0x08,        // Report Size (8)
    0x95, 0x04,        // Report Count (4)
    0x81, 0x02,        // Input (Data, Variable, Absolute)
    0x75, 0x08,        // Report Size (8)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x01,        // Input (Constant)
    0xc0,              // End Collection
};

}  // namespace

SwitchHidGamepad::SwitchHidGamepad() : hid_() {
  static bool initialized = false;

  // Hori Pokken Tournament Controller VID/PID used by several Switch USB HID
  // examples. This is for local compatibility testing only.
  USB.VID(0x0f0d);
  USB.PID(0x00c1);
  USB.usbClass(0);
  USB.usbSubClass(0);
  USB.usbProtocol(0);
  USB.manufacturerName("HORI CO.,LTD.");
  USB.productName("POKKEN CONTROLLER");

  reset();

  if (!initialized) {
    initialized = true;
    hid_.addDevice(this, sizeof(kSwitchReportDescriptor));
  }
}

void SwitchHidGamepad::begin() {
  hid_.begin();
  reset();
}

void SwitchHidGamepad::reset() {
  memset(&report_, 0, sizeof(report_));
  report_.dpad = SWITCH_DPAD_CENTERED;
  centerSticks();
}

bool SwitchHidGamepad::write() {
  return hid_.SendReport(0, &report_, sizeof(report_));
}

void SwitchHidGamepad::buttons(uint16_t mask) {
  report_.buttons = mask;
}

void SwitchHidGamepad::dpad(SwitchDpadDirection direction) {
  report_.dpad = direction;
}

void SwitchHidGamepad::dpad(bool up, bool down, bool left, bool right) {
  static const SwitchDpadDirection kBitsToDirection[16] = {
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_RIGHT,
      SWITCH_DPAD_LEFT,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_DOWN,
      SWITCH_DPAD_DOWN_RIGHT,
      SWITCH_DPAD_DOWN_LEFT,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_UP,
      SWITCH_DPAD_UP_RIGHT,
      SWITCH_DPAD_UP_LEFT,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_CENTERED,
      SWITCH_DPAD_CENTERED,
  };

  const uint8_t bits =
      static_cast<uint8_t>((up << 3) | (down << 2) | (left << 1) | right);
  report_.dpad = kBitsToDirection[bits];
}

void SwitchHidGamepad::leftStick(uint8_t x, uint8_t y) {
  report_.left_x = x;
  report_.left_y = y;
  report_.right_x = 0x80;
  report_.right_y = 0x80;
}

void SwitchHidGamepad::centerSticks() {
  report_.left_x = 0x80;
  report_.left_y = 0x80;
  report_.right_x = 0x80;
  report_.right_y = 0x80;
}

uint16_t SwitchHidGamepad::_onGetDescriptor(uint8_t *buffer) {
  memcpy(buffer, kSwitchReportDescriptor, sizeof(kSwitchReportDescriptor));
  return sizeof(kSwitchReportDescriptor);
}

#endif  // defined(SWITCH_AUTO_DRAW_USE_USB_HID)
