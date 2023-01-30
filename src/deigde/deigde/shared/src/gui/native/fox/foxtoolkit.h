/* 
 * Drag[en]gine IGDE
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

#ifndef _FOXTOOLKIT_H_
#define _FOXTOOLKIT_H_

#include "../../../deigde_configuration.h"

#ifdef PI
#define FIX_FOX_BROKEN_PI
#undef PI
#endif

#include <fox-1.7/fx.h>
#include <fox-1.7/fx3d.h>
#include <fox-1.7/fxkeys.h>
#include <fox-1.7/FXPNGIcon.h>
#include <fox-1.7/FXPNGImage.h>

#ifdef FIX_FOX_BROKEN_PI
#include <dragengine/common/math/decMath.h>
#endif

// anti-windows macros
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#undef GetClassName
#undef GetObject
#undef LoadImage
#undef DrawText
#undef CreateFont
#undef RemoveProp
#undef CreateFile
#undef RemoveFile
#endif

// config.h madness remedy
#undef PACKAGE
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef PACKAGE_BUGREPORT
#undef VERSION



// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
// 
// See https://sourceforge.net/p/foxgui/mailman/message/27757881:
// 
// The following items should be explicitly deleted: FXFont, FXCursor, FXIcon,
// FXImage, FXBitmap, FXVisual, etc.
// 
// Basically, all items which may be shared between multiple widgets.  Because
// there may be multiple references to these items from the widget tree, a widget
// can not know that it is the last reference to a resource; the resource must
// be deleted by other means.
// 
// All subclasses of FXComposite will delete their child-widgets; so to delete
// any widget subtree, its sufficient to delete the parent widget of that
// subtree.
// 
// FXToolBarShell, FXMenuPane and FXPopup are a bit special: these are actually
// top-level widgets, but they're usually referenced from somewhere within another
// widget tree; for example FXMenuPane may be referenced from FXMenuTitle.
// 
// Now, it is possible to popup the FXMenuPane from different places (e.g. from
// FXMenuTitle or right-mouse popup context menu, for example).  So FXMenuPane's
// can also be shared, and must therefore be deleted explicitly.
// 
// For FXToolBarShell, the situation is that the toolbar (dockbar) may be either floated 
// or docked. If the toolbar is floated, then its a child of FXToolBarShell and would
// not be deleted as it constitutes a separate widget tree from the main program.
// 
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING



// internal stuff
#include <dragengine/input/deInputEvent.h>
#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class igdeHotKey;
class igdeWidget;
class igdeContainer;
class igdeEnvironment;
class igdeFilePatternList;

class igdeUIFoxHelper{
public:
	struct sChildLayoutFlags{
		igdeWidget *widget; //< input
		int flags; //< output
		bool canResizeVertical; //< output
		bool canResizeHorizontal; //< output
		sChildLayoutFlags();
	};
	
private:
	igdeUIFoxHelper();
	
public:
	static FXString AccelString( const igdeHotKey &hotKey );
	static FXuchar MnemonicKey( deInputEvent::eKeyCodes keyCode );
	static FXString MnemonizeString( const char* string, deInputEvent::eKeyCodes keyCode );
	static deInputEvent::eKeyCodes KeyCodeFromEvent( const FXEvent &event );
	static int ModifiersFromEvent( const FXEvent &event );
	static FXString FilePatternListToFOX( const igdeFilePatternList &filePatterns );
	
	static int GetChildLayoutFlags( igdeWidget *widget, int defaultFlags =
		LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_SIDE_LEFT );
	static sChildLayoutFlags GetChildLayoutFlagsAll( igdeWidget *widget,
		int defaultFlags = LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_SIDE_LEFT );
	static void UpdateLayoutFlags( igdeWidget *widget );
	static void UpdateLayoutFlagsChildren( igdeContainer *container );
	static FXColor BlendColor( FXColor color1, FXColor color2, float factor );
	static FX::FXColor ConvertColor(const decColor& color);
	static decColor ConvertColor(FX::FXColor color);
	static FX::FXColor BrighterColor(FX::FXColor color, int amount);
	
	static int DebugCountWindows( FXWindow *rootWindow );
};

#define SEL_IGDE_CHILD_LAYOUT_FLAGS (SEL_LAST + 1)
#define SEL_IGDE_FRAME_UPDATE (SEL_LAST + 2)

#ifdef OS_W32
#define FX_COLOR_WINDOW_SHAPE_TRANSPARENT FXRGB(255,255,255)
#define FX_COLOR_WINDOW_SHAPE_OPAQUE FXRGB(0,0,0)
#else
#define FX_COLOR_WINDOW_SHAPE_TRANSPARENT FXRGB(0,0,0)
#define FX_COLOR_WINDOW_SHAPE_OPAQUE FXRGB(255,255,255)
#endif

#endif
