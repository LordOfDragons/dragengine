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

#ifdef IGDE_TOOLKIT_BEOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeBeOSApplication.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../igdeWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Class igdeNativeBeOSApplication::InnerApp
//////////////////////////////////////////////

igdeNativeBeOSApplication::InnerApp::InnerApp( igdeNativeBeOSApplication &app ) :
BApplication( "application/x-vnd.dragengine-igde.app" ),
pApp( app ){
}

igdeNativeBeOSApplication::InnerApp::~InnerApp(){
}

void igdeNativeBeOSApplication::InnerApp::ArgvReceived( int32 argc, char** argv ){
	printf( "[DEIGDE] ArgvReceived(%d)\n", argc );
	int i;
	for( i=1; i<argc; i++ ){
		//pApp.pArguments.Add( decUnicodeString::NewFromUTF8( argv[ i ] ) );
		printf( "[DEIGDE] - Argument %d: %s\n", i, argv[ i ] );
	}
}

void igdeNativeBeOSApplication::InnerApp::AboutRequested(){
	printf( "[DEIGDE] AboutRequested()\n" );
}

bool igdeNativeBeOSApplication::InnerApp::QuitRequested(){
	printf( "[DEIGDE] QuitRequested()\n" );
	return true;
}

void igdeNativeBeOSApplication::InnerApp::ReadyToRun(){
	printf( "[DEIGDE] ReadyToRun()\n" );
}



// Class igdeNativeBeOSApplication
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSApplication::igdeNativeBeOSApplication( igdeApplication &owner ) :
pOwner( &owner ),
pInnerApp( *this ),
pDisableModalUpdating( false ){
}

igdeNativeBeOSApplication::~igdeNativeBeOSApplication(){
}

igdeNativeBeOSApplication *igdeNativeBeOSApplication::CreateNativeApplication( igdeApplication &application ){
	return new igdeNativeBeOSApplication( application );
}

void igdeNativeBeOSApplication::DestroyNativeApplication(){
	delete this;
}



// Management
///////////////

void igdeNativeBeOSApplication::GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args ){
	int i;
	for( i=0; i<argCount; i++ ){
		arguments.Add( decUnicodeString::NewFromUTF8( args[ i ] ) );
	}
}

void igdeNativeBeOSApplication::Initialize( decUnicodeStringList &arguments ){
	// process arguments
	int i;
	for( i=0; i<arguments.GetCount(); i++ ){
		const decString argument( arguments.GetAt( i ).ToUTF8() );
		
		if( argument == "--disable-modal-updating" ){
			pDisableModalUpdating = true;
			printf( "Modal updating disabled!" );
			arguments.RemoveFrom( i-- );
			
		}else{
			break;
		}
	}
}

void igdeNativeBeOSApplication::Run(){
	pInnerApp.Run();
	
	// how to trigger OnFrameUpdate regularly?
	/*
	while( runWhileEvents() ){
		igdeMainWindow * const mainWindow = pOwner->GetMainWindow();
		if( mainWindow ){
			mainWindow->OnFrameUpdate();
		}
	}
	*/
}

void igdeNativeBeOSApplication::Quit(){
	pInnerApp.Quit();
}

decColor igdeNativeBeOSApplication::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowBackground:
		return decColor( 1.0f, 1.0f, 1.0f );
		
	case igdeEnvironment::escWindowForeground:
		return decColor( 0.0f, 0.0f, 0.0f );
		
	case igdeEnvironment::escWidgetBackground:
		return decColor( 0.75f, 0.75f, 0.75f );
		
	case igdeEnvironment::escWidgetForeground:
		return decColor( 0.0f, 0.0f, 0.0f );
		
	case igdeEnvironment::escWidgetHighlight:
		return decColor( 0.9f, 0.9f, 0.9f );
		
	case igdeEnvironment::escWidgetShadow:
		return decColor( 0.35f, 0.35f, 0.35f );
		
	case igdeEnvironment::escWidgetSelectedBackground:
		return decColor( 0.0f, 0.0f, 1.0f );
		
	case igdeEnvironment::escWidgetSelectedForeground:
		return decColor( 1.0f, 1.0f, 1.0f );
		
	default:
		return decColor( 1.0f, 1.0f, 1.0f );
	}
}

void igdeNativeBeOSApplication::GetAppFontConfig( igdeFont::sConfiguration &config ) const{
	config.name = "Noto Sans Display";
	config.size = 12.0f;
	config.bold = false;
	config.italic = false;
	config.underline = false;
	config.strikeThrough = false;
}

void igdeNativeBeOSApplication::ShowError( const deException &exception ) const{
	const decString message( exception.FormatOutput().Join( "\n" ) );
	BAlert * const alert = new BAlert( "Application Error", message, "OK", NULL, NULL,
		B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_STOP_ALERT);
	alert->SetShortcut( 0, B_ESCAPE );
	alert->Go();
}

void igdeNativeBeOSApplication::RunModalWhileShown( igdeWindow &window ){
	/*
	FXWindow * const native = ( FXWindow* )window.GetNativeWidget();
	if( ! native ){
		DETHROW( deeInvalidParam );
	}
	
	const bool updateWindowMain = ! pDisableModalUpdating;
	
	while( runModalWhileEvents( native ) && native->shown() ){
		if( updateWindowMain ){
			igdeMainWindow * const mainWindow = pOwner->GetMainWindow();
			if( mainWindow ){
				mainWindow->OnFrameUpdate();
			}
		}
		native->handle( native, FXSEL( SEL_IGDE_FRAME_UPDATE, 0 ), 0 );
	}
	*/
}

#endif
