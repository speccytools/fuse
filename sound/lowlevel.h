/* lowlevel.h: work out which lowlevel sound routines to use
   Copyright (c) 2004 Philip Kendall

   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

/* This header file should define HAVE_SOUND if any form of sound API
   is available. If so, it should define one of the SOUND_xxx defines,
   and ALWAYS_USE_TIMER if that sound API doesn't provide speed
   control */

#ifndef FUSE_SOUND_LOWLEVEL_H
#define FUSE_SOUND_LOWLEVEL_H

#if defined UI_SDL

#define HAVE_SOUND
#define SOUND_SDL
#define ALWAYS_USE_TIMER

#elif defined HAVE_SYS_SOUNDCARD_H	/* #if defined UI_SDL */

#define HAVE_SOUND
#define SOUND_OSS

#elif defined HAVE_SYS_AUDIOIO_H	/* #if defined UI_SDL */

#define HAVE_SOUND
#define SOUND_SUN

#elif defined HAVE_DSOUND_H		/* #if defined UI_SDL */

#define HAVE_SOUND
#define SOUND_DX
#define ALWAYS_USE_TIMER

#endif					/* #if defined UI_SDL */

#endif			/* #ifndef FUSE_SOUND_LOWLEVEL_H */   