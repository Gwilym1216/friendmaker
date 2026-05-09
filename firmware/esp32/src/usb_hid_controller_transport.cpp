#include "usb_hid_controller_transport.h"

#if defined(SWITCH_AUTO_DRAW_USE_USB_HID)

#include "USB.h"
#include "config.h"

namespace {

constexpr uint8_t kStickMin = 0x00;
constexpr uint8_t kStickCenter = 0x80;
constexpr uint8_t kStickMax = 0xff;

bool hasButton(uint32_t mask, ControllerButton button) {
  return (mask & controllerButtonMask(button)) != 0;
}

}  // namespace

void UsbHidControllerTransport::begin() {
  gamepad_.begin();
  usbStarted_ = USB.begin();
  sendNeutral();
}

uint16_t UsbHidControllerTransport::mapButtons(uint32_t buttonsMask) const {
  uint16_t mapped = 0;

  if (hasButton(buttonsMask, ControllerButton::Y)) {
    mapped |= switchButtonMask(SwitchButtonY);
  }
  if (hasButton(buttonsMask, ControllerButton::B)) {
    mapped |= switchButtonMask(SwitchButtonB);
  }
  if (hasButton(buttonsMask, ControllerButton::A)) {
    mapped |= switchButtonMask(SwitchButtonA);
  }
  if (hasButton(buttonsMask, ControllerButton::X)) {
    mapped |= switchButtonMask(SwitchButtonX);
  }
  if (hasButton(buttonsMask, ControllerButton::L)) {
    mapped |= switchButtonMask(SwitchButtonL);
  }
  if (hasButton(buttonsMask, ControllerButton::R)) {
    mapped |= switchButtonMask(SwitchButtonR);
  }
  if (hasButton(buttonsMask, ControllerButton::ZL)) {
    mapped |= switchButtonMask(SwitchButtonZL);
  }
  if (hasButton(buttonsMask, ControllerButton::ZR)) {
    mapped |= switchButtonMask(SwitchButtonZR);
  }
  if (hasButton(buttonsMask, ControllerButton::Minus)) {
    mapped |= switchButtonMask(SwitchButtonMinus);
  }
  if (hasButton(buttonsMask, ControllerButton::Plus)) {
    mapped |= switchButtonMask(SwitchButtonPlus);
  }
  if (hasButton(buttonsMask, ControllerButton::LStick)) {
    mapped |= switchButtonMask(SwitchButtonLeftStick);
  }
  if (hasButton(buttonsMask, ControllerButton::RStick)) {
    mapped |= switchButtonMask(SwitchButtonRightStick);
  }
  if (hasButton(buttonsMask, ControllerButton::Home)) {
    mapped |= switchButtonMask(SwitchButtonHome);
  }
  if (hasButton(buttonsMask, ControllerButton::Capture)) {
    mapped |= switchButtonMask(SwitchButtonCapture);
  }

  return mapped;
}

void UsbHidControllerTransport::setDpadFromButtons(uint32_t buttonsMask) {
  const bool up = hasButton(buttonsMask, ControllerButton::DpadUp);
  const bool down = hasButton(buttonsMask, ControllerButton::DpadDown);
  const bool left = hasButton(buttonsMask, ControllerButton::DpadLeft);
  const bool right = hasButton(buttonsMask, ControllerButton::DpadRight);
  gamepad_.dpad(up, down, left, right);
}

void UsbHidControllerTransport::setLeftStickFromVector(int x, int y) {
  const uint8_t leftStickX = x < 0 ? kStickMin : (x > 0 ? kStickMax : kStickCenter);
  const uint8_t leftStickY = y < 0 ? kStickMin : (y > 0 ? kStickMax : kStickCenter);
  gamepad_.leftStick(leftStickX, leftStickY);
}

bool UsbHidControllerTransport::sendNeutral() {
  gamepad_.buttons(0);
  gamepad_.dpad(SWITCH_DPAD_CENTERED);
  gamepad_.centerSticks();
  const bool ok = gamepad_.write();
  reportCount_ += 1;
  if (!ok) {
    failedReportCount_ += 1;
  }
  return ok;
}

bool UsbHidControllerTransport::writeAndDelay(uint16_t durationMs) {
  const bool ok = gamepad_.write();
  reportCount_ += 1;
  if (!ok) {
    failedReportCount_ += 1;
  }
  delay(durationMs);
  return ok;
}

bool UsbHidControllerTransport::pressButtons(
    uint32_t buttonsMask, uint16_t holdMs, uint16_t settleMs) {
  gamepad_.buttons(mapButtons(buttonsMask));
  setDpadFromButtons(buttonsMask);
  gamepad_.centerSticks();

  bool ok = writeAndDelay(holdMs);
  if (!sendNeutral()) {
    ok = false;
  }
  delay(settleMs);
  return ok;
}

bool UsbHidControllerTransport::moveDirection(
    int x, int y, uint16_t holdMs, uint16_t settleMs) {
  gamepad_.buttons(0);
  gamepad_.dpad(SWITCH_DPAD_CENTERED);
  setLeftStickFromVector(x, y);

  bool ok = writeAndDelay(holdMs);
  if (!sendNeutral()) {
    ok = false;
  }
  delay(settleMs);
  return ok;
}

bool UsbHidControllerTransport::resetConnection(bool reconnectLastPeer) {
  (void)reconnectLastPeer;
  sendNeutral();
  Serial.println("INFO usb reset requested mode=noop");
  return true;
}

void UsbHidControllerTransport::printStatus(Print &output) const {
  output.println("INFO usb_mode=switch-hid");
  output.print("INFO usb_started=");
  output.println(usbStarted_ ? "true" : "false");
  output.print("INFO usb_reports=");
  output.println(reportCount_);
  output.print("INFO usb_report_failures=");
  output.println(failedReportCount_);
}

const char *UsbHidControllerTransport::name() const { return CONTROL_TRANSPORT; }

#endif  // defined(SWITCH_AUTO_DRAW_USE_USB_HID)
