/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_W32
#include <stdint.h>
#endif

#include "deRenderWindow.h"
#include "deRenderWindowManager.h"
#include "../deResourceManager.h"
#include "../canvas/deCanvasManager.h"
#include "../canvas/deCanvasView.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/graphic/deBaseGraphicRenderWindow.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deRenderWindow
/////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindow::deRenderWindow( deRenderWindowManager *manager ) :
deResource( manager ),

#ifdef ANDROID
pHostWindow( NULL ),
pWindow( NULL ),
#endif

#ifdef OS_W32
pHostWindow( NULL ),
pWindow( NULL ),
#endif

#ifdef OS_BEOS
pHostWindow( NULL ),
pWindow( NULL ),
#endif

#ifdef OS_MACOS
pHostWindow( NULL ),
pWindow( NULL ),
#endif

#if defined OS_UNIX && defined HAS_LIB_X11
pHostWindow( 0 ),
pWindow( 0 ),
#endif

pTitle( "Dragengine" ),
pWidth( 100 ),
pHeight( 100 ),
pFullScreen( false ),
pNotifyScriptOnResize( false ),
pPaint( true ),

pPeerGraphic( NULL )
{
	pCanvasView.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasView() );
	pCanvasView->SetSize( decPoint( pWidth, pHeight ) );
}

deRenderWindow::~deRenderWindow(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
}



// Management
///////////////

#ifdef ANDROID
void deRenderWindow::SetHostWindow( void *window ){
	if( window == pHostWindow ){
		return;
	}
	
	pHostWindow = window;
	
	if( pPeerGraphic ){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow( void *window ){
	pWindow = window;
}
#endif

#ifdef OS_W32
void deRenderWindow::SetHostWindow( HWND window ){
	if( window == pHostWindow ){
		return;
	}
	
	pHostWindow = window;
	
	if( pPeerGraphic ){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow( HWND window ){
	pWindow = window;
}
#endif

#ifdef OS_BEOS
void deRenderWindow::SetHostWindow( BWindow *window ){
	if( window == pHostWindow ){
		return;
	}
	
	pHostWindow = window;
	
	if( pPeerGraphic ){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow( BWindow *window ){
	pWindow = window;
}
#endif

#ifdef OS_MACOS
void deRenderWindow::SetHostWindow( NSWindow *window ){
	if( window == pHostWindow ){
		return;
	}
	
	pHostWindow = window;
	
	if( pPeerGraphic ){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow( NSWindow *window ){
	pWindow = window;
}
#endif

#if defined OS_UNIX && defined HAS_LIB_X11
void deRenderWindow::SetHostWindow( Window window ){
	if( window == pHostWindow ){
		return;
	}
	
	pHostWindow = window;
	
	if( pPeerGraphic ){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow( Window window ){
	pWindow = window;
}
#endif

void deRenderWindow::SetSize( int width, int height ){
	if( width < 0 || height < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pWidth == width && pHeight == height ){
		return;
	}
	
	pWidth = width;
	pHeight = height;
	
	pCanvasView->SetSize( decPoint( width, height ) );
	
	if( pNotifyScriptOnResize ){
		GetResourceManager()->GetScriptingSystem()->OnResizeRenderWindow();
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->SizeChanged();
	}
}

void deRenderWindow::SetFullScreen( bool fullScreen ){
	if( pFullScreen == fullScreen ){
		return;
	}
	
	pFullScreen = fullScreen;
	
	if( pPeerGraphic ){
		pPeerGraphic->FullScreenChanged();
	}
}

void deRenderWindow::SetNotifyScriptOnResize( bool notify ){
	if( pNotifyScriptOnResize == notify ){
		return;
	}
	
	pNotifyScriptOnResize = notify;
	
	if( pPeerGraphic ){
		pPeerGraphic->ScriptNotifyChanged();
	}
}

void deRenderWindow::SetPaint( bool paint ){
	if( paint == pPaint ){
		return;
	}
	
	pPaint = paint;
	
	if( pPeerGraphic ){
		pPeerGraphic->PaintEnabledChanged();
	}
}

void deRenderWindow::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	
	if( pPeerGraphic ){
		pPeerGraphic->TitleChanged();
	}
}

void deRenderWindow::SetIcon( deImage *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	
	if( pPeerGraphic ){
		pPeerGraphic->IconChanged();
	}
}



// Graphic system management
//////////////////////////////

void deRenderWindow::SetPeerGraphic( deBaseGraphicRenderWindow *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
