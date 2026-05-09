# Environment Policy

## Do Not Modify Anaconda Base

This project must not install, upgrade, or uninstall Python packages in the
Anaconda `base` environment.

Do not run commands such as:

```powershell
python -m pip install --user platformio
pip install platformio
pip install --upgrade platformio
pip uninstall h11 cryptography pyOpenSSL
```

when `python` or `pip` points to Anaconda `base`, for example:

```text
D:\anaconda3\python.exe
D:\anaconda3\Scripts\pip.exe
```

## Preferred PlatformIO Environment

Use the PlatformIO IDE extension environment in VS Code when possible:

```text
C:\Users\<user>\.platformio\penv\Scripts\pio.exe
```

Or use a dedicated virtual environment created only for PlatformIO:

```text
F:\tools\pio-venv\Scripts\pio.exe
```

All ESP32 build, upload, and monitor commands should use one of those isolated
`pio.exe` paths.

## Existing Base Environment Warnings

If PlatformIO was already installed into Anaconda `base` and pip printed
dependency conflict warnings such as `httpcore`/`h11` or
`pyOpenSSL`/`cryptography`, do not immediately uninstall or downgrade packages
from `base`.

The safer rule is:

1. Stop using `base` for PlatformIO.
2. Use VS Code PlatformIO's own `penv` or a dedicated `pio-venv`.
3. Leave `base` unchanged unless another Anaconda workflow is actually broken.
4. If `base` must be repaired later, repair it as a separate Python/Anaconda
   task, not as part of this ESP32 project.

For the first observed conflict:

```text
httpcore 1.0.2 requires h11>=0.13,<0.15
but h11 0.16.0 is installed
```

do not blindly downgrade `h11`. If another package such as `wsproto` requires
`h11>=0.16`, the environment has incompatible old/new package pins. Prefer
upgrading `httpcore` so it can use the newer `h11`:

```powershell
python -m pip install --upgrade "httpcore>=1.0.9" "h11>=0.16,<1"
python -m pip check
```

If the second command prints `No broken requirements found.`, the Python
package metadata is consistent again. Do not do this for PlatformIO; PlatformIO
should keep using VS Code's PlatformIO environment or a dedicated `pio-venv`.

## Before Running Commands

Check which tools will be used:

```powershell
Get-Command python
Get-Command pip
Get-Command pio
```

If they resolve to `D:\anaconda3\...`, do not use them for this project.

In PowerShell, `where` is an alias for `Where-Object`. If you want the classic
Windows executable search, use:

```powershell
where.exe python
where.exe pip
where.exe pio
```
