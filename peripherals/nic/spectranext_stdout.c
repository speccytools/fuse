#include "config.h"

#include "spectranext_stdout.h"

#include <stdio.h>

#include "ui/ui.h"

/*
 * Spectranext terminal stdout: OUT to port 0x043b pushes a byte into the host
 * ring (see rp2350 critical.c address_high == 0x04). Fuse forwards each byte
 * to the UI debug log (Win32) and/or stdout.
 */
void spectranext_stdout_write(libspectrum_word port, libspectrum_byte data)
{
    (void)port;
#ifdef WIN32
    ui_spectranext_debug_log_byte((int)data);
#else
    putchar((int)data);
    fflush(stdout);
#endif
}
