#ifndef CONFIG_H
#define CONFIG_H

#include <X11/Xlib.h>
#include <X11/keysym.h>

/* Appearance */
#define BORDER_WIDTH 1
#define COLOR_FOCUSED 0x444444

/* Applications */
static const char *terminal[] = { "st", NULL };
static const char *launcher[] = { "dmenu_run", NULL };
static const char *browser[] = { "firefox", NULL };

#define MODKEY Mod4Mask

typedef enum {
    CMD_TERM,
    CMD_LAUNCH,
    CMD_BROWSER,
    WS_1, WS_2, WS_3,
    MOV_WS_1, MOV_WS_2, MOV_WS_3,
    FOCUS_NEXT, FOCUS_PREV,
    KILL_WIN,
    EXIT_WM,
    CMD_GROW,
    CMD_SHRINK,
    TILE_WIN
} Action;

typedef struct {
    unsigned int mod;
    KeySym keysym;
    Action action;
} KeyBind;

static const KeyBind keys[] = {
    { MODKEY,            XK_Return, CMD_TERM },
    { MODKEY,            XK_p,      CMD_LAUNCH },
    { MODKEY,            XK_j,      FOCUS_NEXT },
    { MODKEY,            XK_b,      CMD_BROWSER },
    { MODKEY,            XK_k,      FOCUS_PREV },
    { MODKEY,            XK_h,      CMD_SHRINK },
    { MODKEY,            XK_l,      CMD_GROW },
    { MODKEY,            XK_t,      TILE_WIN },
    { MODKEY,            XK_q,      KILL_WIN },
    { MODKEY|ShiftMask,  XK_e,      EXIT_WM },
    { MODKEY,            XK_1,      WS_1 },
    { MODKEY,            XK_2,      WS_2 },
    { MODKEY,            XK_3,      WS_3 },
    { MODKEY|ShiftMask,  XK_1,      MOV_WS_1 },
    { MODKEY|ShiftMask,  XK_2,      MOV_WS_2 },
    { MODKEY|ShiftMask,  XK_3,      MOV_WS_3 },
};

#endif
