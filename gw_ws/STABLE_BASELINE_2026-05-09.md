# Stable Baseline - 2026-05-09

## Baseline Name

`stable-switch-bt-usb-2026-05-09`

This is the first stable baseline where both routes are preserved:

- ESP32-S3 USB HID wired Switch controller path: stable and already used for drawing.
- ESP32-WROOM Classic Bluetooth HID path: stable reconnect behavior after selecting the serial port, with last-peer reconnect support.

## Stable Behavior Observed

- ESP32-WROOM on `COM11` can reconnect quickly after the serial port is selected.
- After a successful Bluetooth pairing, disconnection and reconnection work without repeatedly entering a broken broadcast-only state.
- The UI no longer treats "broadcasting only" as a reusable active connection.
- The firmware stores the last Switch host address and attempts delayed HID virtual cable reconnect, closer to commercial Switch controller behavior.
- The ESP32-S3 USB path still builds successfully.

## Important Firmware Details

Bluetooth route:

- PlatformIO environment: `esp32dev_wireless`
- Main transport: `classic-bt-uartswitchcon`
- Device identity:
  - Device name: `Pro Controller`
  - Provider: `Nintendo`
  - HID profile: Classic Bluetooth HID
- Last Switch peer is saved in NVS as `peer_addr`.
- ESP32 uses a Nintendo-like base MAC, then writes the real BT MAC into `reply02` at runtime.
- `BT RESET LAST-PEER` restarts the Bluetooth stack and schedules delayed reconnect to the stored Switch peer.
- Pro Controller readiness is not considered complete at `reply3001`; it waits for the later pairing-ready path.

USB route:

- PlatformIO environment: `esp32s3_usb_switch`
- Transport: `usb-hid-switch`
- Hardware path:
  - ESP32-S3 UART USB -> PC
  - ESP32-S3 native USB -> Switch Lite through USB-C OTG

## Key Files In This Baseline

- `firmware/esp32/src/classic_bt_controller_transport.cpp`
- `firmware/esp32/src/classic_bt_controller_transport.h`
- `firmware/esp32/src/main.cpp`
- `firmware/esp32/src/config.h`
- `firmware/esp32/platformio.ini`
- `firmware/esp32/src/switch_hid_gamepad.cpp`
- `firmware/esp32/src/switch_hid_gamepad.h`
- `firmware/esp32/src/usb_hid_controller_transport.cpp`
- `firmware/esp32/src/usb_hid_controller_transport.h`
- `apps/desktop/src/web/static/app.js`
- `apps/desktop/src/web/static/controllerStatus.js`
- `apps/desktop/src/serial/sender.ts`
- `apps/desktop/test/controller-status.test.ts`

## Verified Commands

Desktop tests:

```powershell
node --import tsx --test apps\desktop\test\controller-status.test.ts apps\desktop\test\execution-control.test.ts apps\desktop\test\three-layer-fix.test.ts
```

Result: 21 tests passed.

ESP32-WROOM Bluetooth firmware:

```powershell
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32dev_wireless
```

Result: build success.

ESP32-S3 USB firmware:

```powershell
C:\Users\86181\.platformio\penv\Scripts\pio.exe run -e esp32s3_usb_switch
```

Result: build success.

## Recovery / Compare Plan

This baseline should be kept as a local Git commit and tag. Future detail changes should be made after this point. If a future change breaks Bluetooth reconnect or USB HID drawing, compare against this baseline first.

Expected local tag:

```text
stable-switch-bt-usb-2026-05-09
```

