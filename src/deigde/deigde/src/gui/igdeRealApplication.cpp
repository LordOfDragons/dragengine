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

#include "igdeRealApplication.h"
#include "igdeWindowMain.h"

#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeWidgetReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>



// Class igdeRealApplication
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeRealApplication::igdeRealApplication(){
}

igdeRealApplication::~igdeRealApplication(){
}



// Management
///////////////

igdeWindowMain &igdeRealApplication::GetWindowMain() const{
	igdeMainWindow * const mainWindow = GetMainWindow();
	DEASSERT_NOTNULL( mainWindow )
	return *( ( igdeWindowMain* )mainWindow );
}

bool igdeRealApplication::Initialize( const decUnicodeStringList &arguments ){
	try{
		SetMainWindow( new igdeWindowMain( pEnvironment ), true );
		GetWindowMain().CreateNativeWidget();
		
	// 	if( updateWithTimer ){
	// 		//GetWindowMain().SetMinUpdateTime( 0.0f ); // timer granularity limits already
	// 		GetWindowMain().SetMinUpdateTime( 1.0f / 100.0f - 0.001f ); // 100Hz max update speed, 1ms timer granularity
	// 		
	// 	}else{
			GetWindowMain().SetMinUpdateTime( 1.0f / 100.0f ); // 100Hz max update speed
	// 	}
		
		// process command line. run application only if it succeeded
		if( ! GetWindowMain().ProcessCommandLine( arguments ) ){
			GetWindowMain().Close();
			return false;
		}
		
	}catch( const deException &e ){
		// it is possible creating the window already failed so we have to check
		if( GetMainWindow() ){
			GetWindowMain().DisplayException( e );
			
		}else{
			// if is even possible the logger could not be created so check this too
			if( pEnvironment.GetLogger() ){
				pEnvironment.GetLogger()->LogException( "IGDE", e );
				
			}else{
				e.PrintError();
			}
			
			igdeCommonDialogs::ErrorFormat( nullptr, "Failed starting IGDE",
				"Please see logs for details:\n%s", e.FormatOutput().Join( "\n" ).GetString() );
		}
		return false;
	}
	
	return true;
}

void igdeRealApplication::CleanUp(){
}
