/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifdef IGDE_TOOLKIT_NULL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeNullApplication.h"
#include "igdeNativeNullWindow.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../igdeWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/logger/deLogger.h>

#ifdef OS_W32
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#endif



// Class igdeNativeNullApplication
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullApplication::igdeNativeNullApplication( igdeApplication &owner ) :
pOwner( owner ),
pQuitRequested( false ){
}

igdeNativeNullApplication::~igdeNativeNullApplication(){
}

igdeNativeNullApplication *igdeNativeNullApplication::CreateNativeApplication( igdeApplication &application ){
	return new igdeNativeNullApplication( application );
}

void igdeNativeNullApplication::DestroyNativeApplication(){
	delete this;
}



// Management
///////////////

#ifdef OS_UNIX
void igdeNativeNullApplication::GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args ){
	int i;
	for( i=1; i<argCount; i++ ){
		arguments.Add( decUnicodeString::NewFromUTF8( args[ i ] ) );
	}
}

#elif defined OS_W32
void igdeNativeNullApplication::GetOSStartUpArguments( decUnicodeStringList &arguments,
const decUnicodeArgumentList &windowsArguments ){
	const int count = windowsArguments.GetArgumentCount();
	int i;
	for( i=1; i<count; i++ ){
		arguments.Add( *windowsArguments.GetArgumentAt( i ) );
	}
}
#endif

void igdeNativeNullApplication::Initialize( decUnicodeStringList& ){
}

void igdeNativeNullApplication::Run(){
	while( ! pQuitRequested ){
		igdeMainWindow * const mainWindow = pOwner.GetMainWindow();
		if( mainWindow ){
			mainWindow->OnFrameUpdate();
		}
	}
}

void igdeNativeNullApplication::Quit(){
	pQuitRequested = true;
}

decColor igdeNativeNullApplication::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowForeground:
	case igdeEnvironment::escWidgetForeground:
	case igdeEnvironment::escWidgetSelectedBackground:
		return decColor( 0.0f, 0.0f, 0.0f );
		
	case igdeEnvironment::escWidgetBackground:
		return decColor( 0.65f, 0.65f, 0.65f );
		
	case igdeEnvironment::escWidgetHighlight:
		return decColor( 0.9f, 0.9f, 0.9f );
		
	case igdeEnvironment::escWidgetShadow:
		return decColor( 0.2f, 0.2f, 0.2f );
		
	case igdeEnvironment::escWindowBackground:
	case igdeEnvironment::escWidgetSelectedForeground:
	default:
		return decColor( 1.0f, 1.0f, 1.0f );
	}
}

void igdeNativeNullApplication::GetAppFontConfig( igdeFont::sConfiguration &config ) const{
	config.name = "Courier";
	config.size = 12.0;
	config.bold = false;
	config.italic = false;
	config.underline = false;
	config.strikeThrough = false;
}

void igdeNativeNullApplication::ShowError( const deException &exception ) const{
	pOwner.GetMainWindow()->GetLogger()->LogError( "IGDE", exception.FormatOutput().Join( "\n" ) );
}

void igdeNativeNullApplication::RunModalWhileShown( igdeWindow &window ){
	igdeNativeNullWindow * const native = ( igdeNativeNullWindow* )window.GetNativeWidget();
	if( ! native ){
		DETHROW( deeInvalidParam );
	}
	
	while( ! pQuitRequested && ! native->GetIsClosed() ){
		igdeMainWindow * const mainWindow = pOwner.GetMainWindow();
		if( mainWindow ){
			mainWindow->OnFrameUpdate();
		}
		
		native->OnFrameUpdate();
	}
}

#endif
