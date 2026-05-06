/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _WAYLANDDEFS_H_
#define _WAYLANDDEFS_H_

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

/*
 * Wayland and xdg-shell protocol definitions.
 *
 * Types, listener structs and opcode defines are provided by the system
 * wayland-client.h and the wayland-scanner-generated xdg-shell header.
 * 
 * The protocol files have been generated like this:
 * 
 * \code
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \
 *   src/modules/graphic/opengl/src/extensions/wayland/xdg-shell-protocol.{h|cpp}
 * 
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/staging/fractional-scale/fractional-scale-v1.xml \
 *   src/modules/graphic/opengl/src/extensions/wayland/fractional-scale-v1-protocol.{h|cpp}
 * 
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml \
 *   src/modules/graphic/opengl/src/extensions/wayland/xdg-decoration-protocol.{h|cpp}
 * \endcode
 * 
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/stable/viewporter/viewporter.xml \
 *   src/modules/graphic/opengl/src/extensions/wayland/viewporter-protocol.{h|cpp}
 * \endcode
 * 
 * The files had then been modified the following way:
 * - Added OS_UNIX_WAYLAND guards
 * - Removed 'const' from wl_output_interface, wl_seat_interface and wl_surface_interface.
 *   This allows to declare them in deoglRTCBUnixWaylandEGL.cpp and copy the actual data
 *   from the dynamically loaded library once possible
 */

#include <dragengine/app/deOSUnix.h>

#include "wayland-egl.h"

#include "color-management-protocol.h"
#include "fractional-scale-protocol.h"
#include "viewporter-protocol.h"
#include "xdg-decoration-protocol.h"
#include "xdg-shell-protocol.h"


// Wayland EGL
extern deWlDynLib deWaylandEglLib;

extern deWlDynLibFunc<wl_egl_window*(const wl_surface *surface, int width, int height)> deoglWlEglWindowCreate;
extern deWlDynLibFunc<void(wl_egl_window *egl_window)> deoglWlEglWindowDestroy;
extern deWlDynLibFunc<void(wl_egl_window *egl_window, int width, int height, int dx, int dy)> deoglWlEglWindowResize;


#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
#endif // _WAYLANDDEFS_H_
