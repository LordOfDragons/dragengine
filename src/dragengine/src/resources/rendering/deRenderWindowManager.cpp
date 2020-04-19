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

#include "deRenderWindow.h"
#include "deRenderWindowManager.h"
#include "../../deEngine.h"
#include "../../systems/deGraphicSystem.h"
#include "../../common/exceptions.h"


// Class deRenderWindowManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindowManager::deRenderWindowManager( deEngine *engine ) :
deResourceManager( engine,ertRenderWindow ){
	SetLoggingName( "render window" );
}

deRenderWindowManager::~deRenderWindowManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deRenderWindowManager::GetRenderWindowCount() const{
	return pRenderWindows.GetCount();
}

deRenderWindow *deRenderWindowManager::GetRootRenderWindow() const{
	return ( deRenderWindow* )pRenderWindows.GetRoot();
}

deRenderWindow *deRenderWindowManager::CreateRenderWindow(){
	deRenderWindow *renderWindow = NULL;
	
	try{
		renderWindow = new deRenderWindow( this );
		GetGraphicSystem()->LoadRenderWindow( renderWindow );
		pRenderWindows.Add( renderWindow );
		
	}catch( const deException & ){
		if( renderWindow ){
			renderWindow->FreeReference();
		}
		throw;
	}
	
	return renderWindow;
}

#ifdef ANDROID
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside( void *window )
#endif
#ifdef OS_BEOS
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside( BWindow *window )
#endif
#if defined OS_UNIX && defined HAS_LIB_X11
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside( Window window )
#endif
#ifdef OS_W32
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside( HWND window )
#endif
#if ( defined OS_UNIX && defined HAS_LIB_X11 ) || defined OS_BEOS || defined OS_W32 || defined ANDROID
{
	deRenderWindow *renderWindow = NULL;
	
	try{
		renderWindow = new deRenderWindow( this );
		renderWindow->SetHostWindow( window ); // tells graphic module to created hosted window
		GetGraphicSystem()->LoadRenderWindow( renderWindow );
		pRenderWindows.Add( renderWindow );
		
	}catch( const deException & ){
		if( renderWindow ){
			renderWindow->FreeReference();
		}
		throw;
	}
	
	return renderWindow;
}
#endif



void deRenderWindowManager::ReleaseLeakingResources(){
	if( GetRenderWindowCount() > 0 ){
		LogWarnFormat( "%i leaking render windows", GetRenderWindowCount() );
		pRenderWindows.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deRenderWindowManager::SystemGraphicLoad(){
	deRenderWindow *renderWindow = ( deRenderWindow* )pRenderWindows.GetRoot();
	
	while( renderWindow ){
		if( ! renderWindow->GetPeerGraphic() ){
			GetGraphicSystem()->LoadRenderWindow( renderWindow );
		}
		
		renderWindow = ( deRenderWindow* )renderWindow->GetLLManagerNext();
	}
}

void deRenderWindowManager::SystemGraphicUnload(){
	deRenderWindow *renderWindow = ( deRenderWindow* )pRenderWindows.GetRoot();
	
	while( renderWindow ){
		renderWindow->SetPeerGraphic( NULL );
		renderWindow = ( deRenderWindow* )renderWindow->GetLLManagerNext();
	}
}

void deRenderWindowManager::RemoveResource( deResource *resource ){
	pRenderWindows.RemoveIfPresent( resource );
}
