#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "config.h"

#define NWS 3
#define MAXW 100
#define MIN_SIZE 50
static Display *dpy;
static Window root, wins[NWS][MAXW];
static Atom protocols, wm_delete;
static int running = 1, ws, count[NWS], focus[NWS];
static int floating[NWS][MAXW];
static float mfact = 0.50;

static void tile(void);
static void focus_win(void);
static void workspace(int);
static void move_to_ws(int);
static void spawn(const char **);
static int locate(Window, int *);
static void remove_win(int, int);
static void mouse_move(XButtonEvent *);

int main(void)
{
    if (!(dpy = XOpenDisplay(NULL))) return 1;
    signal(SIGCHLD, SIG_IGN);
    root = DefaultRootWindow(dpy);
    protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XDefineCursor(dpy, root, XCreateFontCursor(dpy, XC_left_ptr));
    XSelectInput(dpy, root, SubstructureRedirectMask | SubstructureNotifyMask);
    for (unsigned int i = 0; i < sizeof(keys)/sizeof(*keys); i++)
        XGrabKey(dpy, XKeysymToKeycode(dpy, keys[i].keysym), keys[i].mod, root, True, GrabModeAsync, GrabModeAsync);

    XEvent e;
    while (running) {
        XNextEvent(dpy, &e);
        if (e.type == MapRequest) {
            Window w = e.xmaprequest.window;
            if (locate(w, NULL) >= 0 || count[ws] == MAXW) continue;
            XSetWindowBorderWidth(dpy, w, BORDER_WIDTH);
            XSelectInput(dpy, w, StructureNotifyMask | EnterWindowMask); XGrabButton(dpy, Button1, MODKEY, w, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
            XGrabButton(dpy, Button3, MODKEY, w, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
            wins[ws][count[ws]] = w; floating[ws][count[ws]++] = 0;
            focus[ws] = count[ws] - 1;
            XMapWindow(dpy, w);
            tile(); focus_win();
        } else if (e.type == UnmapNotify || e.type == DestroyNotify) {
            Window w = e.type == UnmapNotify ? e.xunmap.window : e.xdestroywindow.window;
            int owner, i = locate(w, &owner);
            if (i >= 0 && (e.type == DestroyNotify || owner == ws)) {
                remove_win(owner, i);
                if (owner == ws) { tile(); focus_win(); }
            }
        } else if (e.type == EnterNotify) {
            int owner, i = locate(e.xcrossing.window, &owner);
            if (i >= 0 && owner == ws) { focus[ws] = i; focus_win(); }
        } else if (e.type == ButtonPress && (e.xbutton.state & MODKEY) &&
                   (e.xbutton.button == Button1 || e.xbutton.button == Button3)) {
            mouse_move(&e.xbutton);
        } else if (e.type == KeyPress) {
            KeySym sym = XLookupKeysym(&e.xkey, 0);
            unsigned int mod = e.xkey.state & (MODKEY | ShiftMask);
            for (unsigned int i = 0; i < sizeof(keys)/sizeof(*keys); i++) {
                if (keys[i].keysym != sym || keys[i].mod != mod) continue;
                switch (keys[i].action) {
                case CMD_TERM: spawn(terminal); break;
                case CMD_LAUNCH: spawn(launcher); break;
                case FOCUS_NEXT: if (count[ws]) focus[ws] = (focus[ws] + 1) % count[ws]; focus_win(); break;
                case FOCUS_PREV: if (count[ws]) focus[ws] = (focus[ws] + count[ws] - 1) % count[ws]; focus_win(); break;
                case KILL_WIN:
                    if (count[ws]) {
                        Window w = wins[ws][focus[ws]]; Atom *p; int n;
                        if (XGetWMProtocols(dpy, w, &p, &n)) {
                            for (int j = 0; j < n; j++) if (p[j] == wm_delete) {
                                XEvent m = {0}; m.xclient.type = ClientMessage; m.xclient.window = w;
                                m.xclient.message_type = protocols; m.xclient.format = 32;
                                m.xclient.data.l[0] = wm_delete; m.xclient.data.l[1] = CurrentTime;
                                XSendEvent(dpy, w, False, NoEventMask, &m); break;
                            }
                            XFree(p);
                        }
                    }
                    break;
                case WS_1: workspace(0); break;
                case WS_2: workspace(1); break;
                case WS_3: workspace(2); break;
                case MOV_WS_1: move_to_ws(0); break;
                case MOV_WS_2: move_to_ws(1); break;
                case MOV_WS_3: move_to_ws(2); break;
                case EXIT_WM: running = 0; break;
                case CMD_GROW: if (mfact < 0.95) { mfact += 0.05; tile(); } break;
                case CMD_SHRINK: if (mfact > 0.05) { mfact -= 0.05; tile(); } break;
                }
                break;
            }
        }
    }
    XCloseDisplay(dpy);
    return 0;
}

static void spawn(const char **cmd)
{
    if (fork() == 0) { setsid(); execvp(cmd[0], (char *const *)cmd); _exit(127); }
}

static int locate(Window w, int *owner)
{
    for (int s = 0; s < NWS; s++) for (int i = 0; i < count[s]; i++)
        if (wins[s][i] == w) { if (owner) *owner = s; return i; }
    return -1;
}
static void remove_win(int s, int i)
{
    int removed = i;
    for (; i < count[s] - 1; i++) {
        wins[s][i] = wins[s][i + 1];
        floating[s][i] = floating[s][i + 1];
    }
    if (--count[s] == 0) focus[s] = 0;
    else if (focus[s] >= count[s]) focus[s] = count[s] - 1;
    else if (focus[s] > removed) focus[s]--;
}

static void focus_win(void)
{
    XWindowAttributes a;
    if (!count[ws]) {
        XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
        return;
    }
    if (focus[ws] >= count[ws]) focus[ws] = count[ws] - 1;
    for (int i = 0; i < count[ws]; i++) {
        if (XGetWindowAttributes(dpy, wins[ws][i], &a))
            XSetWindowBorder(dpy, wins[ws][i],
                i == focus[ws] ? COLOR_FOCUSED : 0x222222);
    }
    Window w = wins[ws][focus[ws]];
    if (!XGetWindowAttributes(dpy, w, &a) || a.map_state != IsViewable) {
        XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
        return;
    }
    XRaiseWindow(dpy, w);
    XSetInputFocus(dpy, w, RevertToPointerRoot, CurrentTime);
}

static void tile(void)
{
    XWindowAttributes a;
    if (!XGetWindowAttributes(dpy, root, &a)) return;
    int n = 0;
    for (int i = 0; i < count[ws]; i++) if (!floating[ws][i]) n++;
    if (!n) return;
    int mw = a.width * mfact, k = 0;
    for (int i = 0; i < count[ws]; i++) if (!floating[ws][i]) {
        if (n == 1) XMoveResizeWindow(dpy, wins[ws][i], 0, 0, a.width, a.height);
        else if (!k) XMoveResizeWindow(dpy, wins[ws][i], 0, 0, mw, a.height);
        else {
            int y1 = (k - 1) * a.height / (n - 1), y2 = k * a.height / (n - 1);
            XMoveResizeWindow(dpy, wins[ws][i], mw, y1, a.width - mw, y2 - y1);
        }
        k++;
    }
}

static void mouse_move(XButtonEvent *e)
{
    int owner, i = locate(e->window, &owner);
    if (i < 0 || owner != ws) return;
    XWindowAttributes a;
    if (!XGetWindowAttributes(dpy, e->window, &a)) return;
    focus[ws] = i; focus_win(); floating[ws][i] = 1;
    int x = e->x_root, y = e->y_root;
    XGrabPointer(dpy, root, False, PointerMotionMask | ButtonReleaseMask,
                 GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XEvent ev;
    while (1) {
        XMaskEvent(dpy, PointerMotionMask | ButtonReleaseMask, &ev);
        if (ev.type == ButtonRelease) break;
        int dx = ev.xmotion.x_root - x, dy = ev.xmotion.y_root - y;
        if (e->button == Button1) XMoveWindow(dpy, e->window, a.x + dx, a.y + dy);
        else XResizeWindow(dpy, e->window,
            (unsigned)(a.width + dx < MIN_SIZE ? MIN_SIZE : a.width + dx),
            (unsigned)(a.height + dy < MIN_SIZE ? MIN_SIZE : a.height + dy));
    }
    XUngrabPointer(dpy, CurrentTime);
}

static void workspace(int s)
{
    if (s == ws || s < 0 || s >= NWS) return;
    for (int i = 0; i < count[ws]; i++) XUnmapWindow(dpy, wins[ws][i]);
    ws = s;
    for (int i = 0; i < count[ws]; i++) XMapWindow(dpy, wins[ws][i]);
    tile(); focus_win();
}

static void move_to_ws(int s)
{
    if (s == ws || s < 0 || s >= NWS || !count[ws] || count[s] == MAXW) return;
    int i = focus[ws]; Window w = wins[ws][i]; int f = floating[ws][i];
    remove_win(ws, i); XUnmapWindow(dpy, w);
    wins[s][count[s]] = w; floating[s][count[s]++] = f; focus[s] = count[s] - 1;
    tile(); focus_win();
}
