#pragma once

#include "controller_transport.h"

#if defined(SWITCH_AUTO_DRAW_USE_USB_HID)

#include "switch_hid_gamepad.h"

class UsbHidControllerTransport : public ControllerTransport {
 public:
  void begin() override;
  bool pressButtons(uint32_t buttonsMask, uint16_t holdMs, uint16_t settleMs) override;
  bool moveDirection(int x, int y, uint16_t holdMs, uint16_t settleMs) override;
  bool resetConnection(bool reconnectLastPeer = false) override;
  void printStatus(Print &output) const override;
  const char *name() const override;

 private:
  uint16_t mapButtons(uint32_t buttonsMask) const;
  void setDpadFromButtons(uint32_t buttonsMask);
  void setLeftStickFromVector(int x, int y);
  bool sendNeutral();
  bool writeAndDelay(uint16_t durationMs);

  SwitchHidGamepad gamepad_;
  bool usbStarted_ = false;
  uint32_t reportCount_ = 0;
  uint32_t failedReportCount_ = 0;
};

#endif  // defined(SWITCH_AUTO_DRAW_USE_USB_HID)
