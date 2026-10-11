> *"Simplicity is prerequisite for realiability."* - Edsger W. Dijkstra

hmwm (horrendous micro window manager) is a window manager i have made since i was frustrated at how large the source code of the window manager dwm had gotten,while they were still promoting "suckless" software. that's why i have come up with an ultra minimal/lightweight window manager. it runs on under **250** lines of C code,which is what i imagine an actual light window manager to be running on.
<img width="2172" height="724" alt="image" src="https://github.com/user-attachments/assets/3861bd29-bf82-4585-86bc-8a66dc5273a8" />

# My Philosophy 

`hmwm` contains only the features I use every single day. There is no massive feature list here because **I don’t want to overwhelm you** with options you will never touch. If a feature isn't essential to a lightning-fast, distraction-free environment, it doesn't belong. this reminds me of what st had become as a terminal,so many features that people don't even know about,for example did you know that you can zoom in with ctrl + Shift + Page up and Down inside of the st terminal?

# Features

to show you how much my window manager contains in just those lines without having u click onto the config.h,here they are:

1 - focusing between windows

2 - If your mouse hovers over a window,the window immediately gets focused upon (might be an annoyance for some users,though i find it useful since if you're writing code from a C tutorial and u have to pause the video,you can just hover your mouse over the browser window and pause the video).

3 - resizing windows left and right

4 - focusing color. You can change the color of your focused window in the config.h (the d efault color is gray 0x444444)

5 - The window manager supports floating windows (as you can see by the photo at the bottom.)

This window manager was inspired by :

dwm (https://dwm.suckless.org)

sxwm (https://github.com/uint23/sxwm)

ewm (https://github.com/pwnwriter/ewm)

example photo of my window manager running on 70 MBS of ram. (the right terminal shows that hmwm only uses 2.8 MBS of ram RSS):
<img width="1366" height="768" alt="2026-10-11-204213_1366x768_scrot" src="https://github.com/user-attachments/assets/707d58f3-9ef9-46b6-9c27-bf5d29e75410" />

# quick setup
git clone the repository,cd into it and

'''bash
make
sudo/doas make install
