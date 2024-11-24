# CHIP-8 Emulator

This repository contains a basic CHIP-8 emulator, developed as a learning project to understand how emulators work. The emulator accurately replicates the behavior of the CHIP-8 virtual machine, supporting features like input handling, customizable display scaling, runtime ROM loading, and more.

## Features

- **Custom ROM Loading:** Load and run any compatible CHIP-8 ROMs at runtime.
- **Dynamic Display Scaling:** Adjust the emulator's scale to fit your preferred display size.
- **Configurable Execution Speed:** Modify the delay between cycles to control simulation speed.
- **Input Handling:** Fully functional input system to interact with games.
- **State Management:** Pause, resume, and step through execution one cycle at a time.
- **Save states:** Save and load the states of your Chip8 application. It allows you to better test what is going on, or just retry part of a game when you failed it.

## How CHIP-8 Works

CHIP-8 is a virtual machine originally developed in the 1970s to simplify game development on systems like the COSMAC VIP and TELMAC 1800. It provides a simple platform with:

- **Memory:** A 4KB address space, where programs are loaded starting at address `0x200`.
- **Registers:** 16 8-bit general-purpose registers (`V0` through `VF`), with `VF` often used as a flag.
- **Stack:** A call stack for managing subroutine calls, typically with 16 levels.
- **Timers:** A delay timer and a sound timer, both counting down at 60Hz.
- **Display:** A monochrome grid of 64x32 pixels for rendering graphics.
- **Input:** A 16-key hexadecimal keypad (0-F).

Programs run in an emulated environment, executing one instruction at a time. CHIP-8 instructions are 2 bytes long and include operations like drawing sprites, conditional branching, arithmetic, and interacting with timers or memory.


## Screenshots

### Tetris Demo (the demo is not created by me, it's just a way to show the UI):
![Tetris Demo](https://github.com/user-attachments/assets/c419bdc4-8e1f-4d01-981a-027e9c4fbceb)

### Fullscreen with Lower Scale:
![Fullscreen Demo](https://github.com/user-attachments/assets/2b940491-61f7-4afe-83d0-8620f01e15ef)


## Installation

If you want to mess around with the project, you can simply clone it. Do however note that it is made for Visual C++ and requires Visual Studio in order to work out of the box. If you are using something else, you will need to configure your build system and link SFML.

## Possible improvements

- Adding audio (right now it just displays a BEEP in the console)
- Making the delay affect only the CHIP-8 processor, not the entire application
- Display more debugging data, such as the cycle count
- Tidy up the code
- Improve the UI

## ROMs

The project already contains a few ROMs I used for testing, such as: [Tetris](https://github.com/kripod/chip8-roms/tree/master/games), [Rocket](https://github.com/kripod/chip8-roms/tree/master/games) and a [test ROM](https://github.com/corax89/chip8-test-rom).
More ROMs are available on the same repository as Tetris and Rocket.

## Resources

If you'd like to learn more about CHIP-8 or emulator development, here are some excellent resources:

- [Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Austin Morlan's Blog](https://austinmorlan.com/posts/chip8_emulator/)
- [Cowgod's CHIP-8 Documentation](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)
