/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* enable OS_ANDROID */
#ifndef OS_ANDROID
#define OS_ANDROID 1
#endif

/* enable HAS_LIB_DL */
#ifndef HAS_LIB_DL
#define HAS_LIB_DL 1
#endif

/* enable OS_UNIX */
#ifndef OS_UNIX
#define OS_UNIX 1
#endif

/* disable OS_W32 */
#ifdef OS_W32
#undef OS_W32
#endif

/* enable HAS_FUNC_UTIMENSAT */
#ifndef HAS_FUNC_UTIMENSAT
#define HAS_FUNC_UTIMENSAT 1
#endif

/* disable HAS_FUNC_PTRHEAD_CANCEL */
#ifdef HAS_FUNC_PTRHEAD_CANCEL
#undef HAS_FUNC_PTRHEAD_CANCEL
#endif

/* disable OS_BEOS */
#ifdef OS_BEOS
#undef OS_BEOS
#endif

/* disable HAS_LIB_X11 */
#ifdef HAS_LIB_X11
#undef HAS_LIB_X11
#endif

/* enable HAS_LIB_PTHREAD */
#ifndef HAS_LIB_PTHREAD
#define HAS_LIB_PTHREAD 1
#endif

/* End of configuration file. */
