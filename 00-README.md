# Waveshare RP2350-USB-A builds (branch `ws_rp2350_usb_a_UART_VIA_USB`)

CircuitPython 9.2.8 with three variants of the Waveshare RP2350-USB-A board, used by the kmk-brain project
(`hid-to-kmk-proxy`). All three are built from this one branch.

| `BOARD=` | `boot_out.txt` board name | What it is |
|---|---|---|
| `waveshare_rp2350_usb_a` | Waveshare RP2350-Usb-A | Original: `board_init()` creates the USB host port on GPIO12/13 at boot (the pins cannot be used as a PIO UART). |
| `waveshare_rp2350_usb_a_UART_VIA_USB` | Waveshare RP2350-Usb-A (UART via USB) | Empty `board_init()`: GPIO12/13 stay free, Python creates `usb_host.Port(...)` when needed. **What the project needs.** |
| `waveshare_rp2350_usb_a_overclocked` | Waveshare RP2350-Usb-A (500 Hz HID) | Same as `_UART_VIA_USB`, plus the HID endpoints are polled every 2 ms (500 Hz) instead of 8 ms (125 Hz). Not a CPU overclock. |

The polling interval comes from `CIRCUITPY_USB_HID_BINTERVAL` (`shared-module/usb_hid/__init__.c`, default 8, ms at
full speed). A board sets it in its `mpconfigboard.h`: 4 = 250 Hz, 2 = 500 Hz, 1 = 1000 Hz.

## Prerequisites (once)

- **GCC 13 or 14** `arm-none-eabi` toolchain. Older (10.x) is refused by `py/circuitpy_mpconfig.h` ("Minimum GCC
  version 13"), 16.x fails on warnings-as-errors in vendored code. Arm GNU Toolchain 13.3.rel1 works as a tarball
  (no package manager needed), put its `bin/` first on `PATH`.
- A Python venv with the build dependencies, activated for the build:

  ```
  python3 -m venv .venv
  . .venv/bin/activate
  pip install -r requirements-dev.txt     # at least: cascadetoml jinja2 typer
  ```

- Submodules and `mpy-cross`:

  ```
  make fetch-all-submodules
  make -C mpy-cross
  ```

## Build

From `ports/raspberrypi`:

```
cd ports/raspberrypi
make -j8 BOARD=waveshare_rp2350_usb_a
make -j8 BOARD=waveshare_rp2350_usb_a_UART_VIA_USB
make -j8 BOARD=waveshare_rp2350_usb_a_overclocked
```

Each build goes to its own directory: `build-<BOARD>/firmware.uf2`.

All three in one go:

```
cd ports/raspberrypi
for b in waveshare_rp2350_usb_a waveshare_rp2350_usb_a_UART_VIA_USB waveshare_rp2350_usb_a_overclocked; do
    make -j8 BOARD=$b || break
done
ls -l build-waveshare_rp2350_usb_a*/firmware.uf2
```

Stubs for the project's venv (once per firmware version, from the repository root):

```
make stubs
# then, in the project's venv: pip install ./dist/circuitpython_stubs-*.tar.gz
```

## Flash

1. Unplug the board, hold BOOTSEL, plug it in: it shows up as the `RP2350` drive.
2. Copy `build-<BOARD>/firmware.uf2` onto it. The board reboots by itself.
3. Check `boot_out.txt` on `CIRCUITPY` for the board name from the table above.

Files on `CIRCUITPY` (the project's `code.py`, `pie/`, ...) survive a firmware change.

## Notes

- A build with `BOARD=waveshare_rp2350_usb_a` is **not** what the project needs since the boards were split; use
  `_UART_VIA_USB` (or `_overclocked`).
- Written together with the board split; the build commands above were not run on the machine where this was written
  (its toolchain was unusable), so correct this file if a step differs in your build environment.
