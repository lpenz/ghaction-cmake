/****************************************************************************/
/**
 * @file
 * @brief xseturgent is a tool that sets/clears the urgency hint of a
 * window in X11
 *
 * See LICENSE file for copyright and license details.
 */
/****************************************************************************/

#include <X11/X.h>             // for Window
#include <X11/Xlib.h>          // for XErrorEvent, Display, XCloseDisplay
#include <X11/Xutil.h>         // for XWMHints, XUrgencyHint, XGetWMHints
#include <bits/getopt_core.h>  // for optarg, getopt, optind
#include <limits.h>            // for LONG_MAX
#include <stdarg.h>            // for va_end, va_list, va_start
#include <stdbool.h>           // for bool, false, true
#include <stdio.h>             // for fprintf, stderr, printf, NULL, vprintf
#include <stdlib.h>            // for strtol, getenv
#include <string.h>            // for strcmp

#define PROGRAM_NAME "xseturgent"
#define PROGRAM_VERSION "2.0"

//! Enum for operation type
enum op {
    URGENT_SET,     //!< Set the urgency bit to 1
    URGENT_RESET,   //!< Set the urgency bit to 0
    URGENT_TOGGLE,  //!< Toggle the urgency bit
};

/**
 * Print message if verbose is true
 *
 * @param verbose Control if the message should go to stdout
 * @param fmt Format string
 * @seealso vprintf
 */
static void printf_if(bool verbose, const char *fmt, ...) {
    va_list args;
    if (!verbose) {
        return;
    }
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/**
 * Operate on the urgency bit for the given display and window.
 *
 * @param verbose Set to true for more verbosity
 * @param dpy Display as returned by XOpenDisplay
 * @param windowid Window ID as shown by xwininfo
 * @param op Operation to perform on urgency bit
 */
static int xseturgent_window(bool verbose, Display *dpy, Window windowid,
                             enum op op) {
    int ret = 0;
    XWMHints *hints = XGetWMHints(dpy, windowid);
    if (!hints) {
        fprintf(stderr, "Unable to get window manager hints.\n");
        return 20;
    }
    printf_if(verbose, "Window 0x%08lx has hint->flags 0x%08lx\n", windowid,
              hints->flags);
    switch (op) {
        case URGENT_SET:
            hints->flags |= XUrgencyHint;
            break;
        case URGENT_RESET:
            hints->flags &= ~XUrgencyHint;
            break;
        case URGENT_TOGGLE:
            if (hints->flags | XUrgencyHint) {
                hints->flags |= XUrgencyHint;
            } else {
                hints->flags &= ~XUrgencyHint;
            }
            break;
    }
    printf_if(verbose, "Window 0x%08lx setting hint->flags 0x%08lx\n", windowid,
              hints->flags);
    if (!XSetWMHints(dpy, windowid, hints)) {
        fprintf(stderr, "Unable to set urgency hint.\n");
        ret = 30;
    }
    printf_if(verbose, "Freeing hints\n");
    XFree(hints);

    return ret;
}

/**
 * X11 error handler passed to XSetErrorHandler
 *
 * Just display the error information on stderr.
 */
static int error_handler(Display *d, XErrorEvent *e) {
    char msg[80];
    XGetErrorText(d, e->error_code, msg, sizeof(msg));
    fprintf(stderr, "Error %d (%s) on request %d.%d\n", e->error_code, msg,
            e->request_code, e->minor_code);
    return 0;
}

/**
 * Operate on the urgency bit
 *
 * This function opens the display and makes sure that it's closed later.
 *
 * @param verbose Set to true for more verbosity
 * @param windowid Window ID as shown by xwininfo
 * @param op Operation to perform on urgency bit
 */
static int xseturgent(bool verbose, Window windowid, enum op op) {
    Display *dpy;
    int ret = 0;

    printf_if(verbose, "Setting error handler\n");
    XSetErrorHandler(error_handler);

    printf_if(verbose, "Opening display\n");
    if (!(dpy = XOpenDisplay(NULL))) {
        fprintf(stderr, "Unable to open display.\n");
        return 10;
    }

    ret = xseturgent_window(verbose, dpy, windowid, op);

    printf_if(verbose, "Closing display\n");
    XCloseDisplay(dpy);

    return ret;
}

/****************************************************************************/

//! USAGE help message
const char USAGE[] =
    "Usage: " PROGRAM_NAME
    " [-V] [-v] [-h] [-i windowid] [-t operation]\n"
    "Change urgent hint of a window in X\n"
    "    -h             This help.\n"
    "    -V             Version information.\n"
    "    -v             Verbose mode.\n"
    "    -i <windowid>  Specify the window;"
    " default is WINDOWID environment variable\n"
    "    -t <operation> Operation is set, reset or toggle; default is set\n";

/****************************************************************************/

int main(int argc, char *argv[]) {
    bool verbose = false;
    Window windowid = LONG_MAX;
    enum op op = URGENT_SET;

    const char *windowidEnv = getenv("WINDOWID");
    if (windowidEnv) {
        windowid = strtol(windowidEnv, NULL, 0);
    }

    const char options[] = "i:t:hvV";
    int opt;
    while ((opt = getopt(argc, argv, options)) != -1) {
        switch (opt) {
            case 'i':
                windowid = strtol(optarg, NULL, 0);
                break;
            case 't':
                if (strcmp(optarg, "set") == 0) {
                    op = URGENT_SET;
                } else if (strcmp(optarg, "reset") == 0) {
                    op = URGENT_RESET;
                } else if (strcmp(optarg, "toggle") == 0) {
                    op = URGENT_TOGGLE;
                } else {
                    fprintf(stderr, "Invalid operation type %s.\n%s", optarg,
                            USAGE);
                    return 1;
                }
                break;
            case 'h':
                printf("%s", USAGE);
                return 0;
            case 'v':
                verbose = true;
                break;
            case 'V':
                printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
                return 0;
            case '?':
                fprintf(stderr, "Unknown option/command.\n%s", USAGE);
                return 1;
            default:
                fprintf(stderr, "Error: option/command not found!\n%s", USAGE);
                return 1;
        }
    }

    if (argc - optind != 0) {
        fprintf(stderr, "Invalid number of arguments.\n%s", USAGE);
        return 1;
    }

    if (windowid == LONG_MAX) {
        fprintf(stderr,
                "Window id discovery failed, please specify one with -i\n%s",
                USAGE);
        return 2;
    }
    return xseturgent(verbose, windowid, op);
}
