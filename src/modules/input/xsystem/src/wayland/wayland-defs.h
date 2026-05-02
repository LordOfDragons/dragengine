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

#ifndef _DEXSIWAYLANDDEFS_H_
#define _DEXSIWAYLANDDEFS_H_

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

/*
 * XSystem input module Wayland definitions.
 *
 * Declares module-level globals for Wayland functions and interface data that are not part of
 * the deOSUnix core exports. All globals are loaded from the dynamically-loaded
 * libwayland-client library.
 *
 * The protocol files have been generated like this:
 *
 * \code
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml \
 *   src/modules/input/xsystem/src/wayland/pointer-constraints-unstable-v1-protocol.{h|cpp}
 *
 * wayland-scanner {client-header|private-code} \
 *   /usr/share/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml \
 *   src/modules/input/xsystem/src/wayland/relative-pointer-unstable-v1-protocol.{h|cpp}
 * \endcode
 *
 * The files had then been modified the following way:
 * - Added OS_UNIX_X11/OS_UNIX_WAYLAND guards
 * - Removed extern references to interfaces (wl_*_interface)
 * - Replaces interfaces in pointer tables with deWlDynLibData instances.
 */

#include <dragengine/app/deOSUnix.h>

#include "pointer-constraints-unstable-v1-protocol.h"
#include "relative-pointer-unstable-v1-protocol.h"

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
#endif // _DEXSIWAYLANDDEFS_H_
