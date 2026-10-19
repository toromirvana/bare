> *"Simplicity is prerequisite for reliability."* — Edsger W. Dijkstra

**bare** is a minimal X11 window manager written in C.

I started this project because I was frustrated with how large the source code of `dwm` had become while still promoting the idea of "suckless" software. I wanted something smaller, simpler, and easier to understand, so I decided to build my own window manager.

The goal isn't to have the most features or the fewest lines of code possible. It's to have only what is useful, without unnecessary complexity.

## My philosophy

`bare` contains only the features I use every day. I don't want to overwhelm users with options they may never touch. If a feature isn't useful for a simple, distraction-free desktop, it probably doesn't belong.

This philosophy also reminds me of what happened to terminals like `st`: over time, software can accumulate features that many users don't even know exist. I want `bare` to stay small enough that its source code remains approachable and understandable.

## Features

- **Tiling:** Automatically arranges windows on the desktop.
- **Workspaces:** Switch between three workspaces.
- **Keyboard shortcuts:** Control windows and launch applications from the keyboard.
- **Hover focus:** Focus a window by moving the mouse over it.
- **Floating windows:** Move and resize floating windows using the mouse.
- **Return to tiling:** Send a floating window back to the tiling layout.
- **Configurable focus border:** Change the focused window's border color in `config.h`. The default is gray (`0x444444`).
- **Lightweight:** Designed to keep the implementation small and avoid unnecessary overhead.

## Screenshots

In my own setup, `bare` used approximately 2.8 MiB of resident memory (RSS). Memory usage varies depending on the system and workload.

## Configuration

Configuration is kept in `config.h`, where you can customize keybindings and other settings without digging through the main source file.

## Installing the window manager

```sh
git clone https://codeberg.org/toromirvana/bare.git
cd bare
make
sudo make install || doas make install
```

## Inspiration

- [dwm](https://dwm.suckless.org/)
- [sxwm](https://github.com/uint23/sxwm)
- [ewm](https://github.com/pwnwriter/ewm)
