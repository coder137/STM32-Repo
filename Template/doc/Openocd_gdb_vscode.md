- [OpenOCD with GDB integrated with VSCode](#openocd-with-gdb-integrated-with-vscode)
- [Pre-Requisite](#pre-requisite)
- [Configuration Files](#configuration-files)
- [Running](#running)
- [Usage](#usage)
  - [Using the Debug Console](#using-the-debug-console)
  - [VSCode Debugging](#vscode-debugging)

# OpenOCD with GDB integrated with VSCode

# Pre-Requisite

- Read the [Openocd_gdb_cli.md](Openocd_gdb_cli.md) file
- Install VSCode
- Install Extensions
  - C/C++ (Microsoft)
  - Cortex-Debug (marus25)
- OpenOCD `.cfg` file

# Configuration Files

- To your `.vscode` folder add a `launch.json` file
```json
{
  "configurations": [
    {
      "cwd": "${workspaceFolder}",
      "executable": "build/<your_executable_name>.elf",
      "configFiles": [
        "<your_cfg_file>.cfg"
      ],
      "name": "Debug Microcontroller",
      "request": "launch",
      "type": "cortex-debug",
      "servertype": "openocd"
    }
  ]
}
```

# Running

- Click on the Run Tab (attached to the left side of VSCode along with Extensions)
  - Alternate method is to press the F5 button (in Windows)
- Make sure `Debug Microcontroller` is set
- Click the **Start Debugging** button

# Usage

## Using the Debug Console

- Open the Debug Console to run commands as mentioned in the **Openocd_gdb_cli.md** document

## VSCode Debugging

- VSCode has a widget that can step through the code line by line or to the next breakpoint
- Clicking the line of code (near the number) also sets a **Breakpoint** automatically
