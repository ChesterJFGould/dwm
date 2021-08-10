/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int gappx     = 8;
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "LiterationMono Nerd Font:pixelsize=14" };

static char norm_fg[] = "#000000";
static char norm_bg[] = "#000000";
static char norm_bd[] = "#000000";
static char inv_fg[] = "#000000";
static char inv_bg[] = "#000000";
static char inv_bd[] = "#000000";
static char sel_fg[] = "#000000";
static char sel_bg[] = "#000000";
static char sel_bd[] = "#000000";

static char *colors[][3] = {
       /*               fg     bg     border */
       [SchemeNorm] = { norm_fg, norm_bg, norm_bd },
       [SchemeInv]  = { inv_fg, inv_bg, inv_bd },
       [SchemeSel]  = { sel_fg, sel_bg, sel_bd },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "",         NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static float mfact     = 0.75; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "tatami.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "=[]",      tileRight },    /* first entry is default */
	{ "[]=",      tileLeft },    /* first entry is default */
	{ "=M=",      centeredmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[]|",      deck },
	{ "[]+",      tatami },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 *  * Xresources preferences to load at startup
 *  */
ResourcePref resources[] = {
	{ "normfgcolor",        STRING,  &(norm_fg) },
	{ "normbgcolor",        STRING,  &(norm_bg) },
	{ "normbdcolor",    STRING,  &(norm_bd) },
	{ "invfgcolor",         STRING,  &(inv_fg) },
	{ "invbgcolor",         STRING,  &(inv_bg) },
	{ "invbdcolor",     STRING,  &(inv_bd) },
	{ "selfgcolor",         STRING,  &(sel_fg) },
	{ "selbgcolor",         STRING,  &(sel_bg) },
	{ "selbdcolor",     STRING,  &(sel_bd) },
};



static Key keys[] = {
	/* modifier                     key             function        argument */
	{ MODKEY,                       XK_b,           togglebar,      {0} },
	{ MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_semicolon,   focusmaster,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_k,           incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_j,           incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,           setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_k,           setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_l,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_h,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_c,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_d,           setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ControlMask,           XK_t,           setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_space,       zoom,           {0} },
	{ MODKEY,                       XK_Tab,         view,           {0} },
	{ MODKEY,                       XK_w,           killclient,     {0} },
	{ MODKEY,                       XK_f,           togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_0,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                           0)
	TAGKEYS(                        XK_2,                           1)
	TAGKEYS(                        XK_3,                           2)
	TAGKEYS(                        XK_4,                           3)
	TAGKEYS(                        XK_5,                           4)
	TAGKEYS(                        XK_6,                           5)
	TAGKEYS(                        XK_7,                           6)
	TAGKEYS(                        XK_8,                           7)
	TAGKEYS(                        XK_9,                           8)
	{ MODKEY|ControlMask,           XK_q,           quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

