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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxApplication.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../igdeWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>

#ifdef OS_W32
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#endif



// Event map
//////////////

FXDEFMAP( igdeNativeFoxApplication ) igdeNativeFoxApplicationMap[] = { };

FXIMPLEMENT( igdeNativeFoxApplication, FXApp,
	igdeNativeFoxApplicationMap, ARRAYNUMBER( igdeNativeFoxApplicationMap ) )



// Class igdeNativeFoxApplication
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxApplication::igdeNativeFoxApplication(){
}

igdeNativeFoxApplication::igdeNativeFoxApplication( igdeApplication &powner ) :
FXApp( "DEIGDE", "Drag[en]gine" ),
pOwner( &powner ),
pToolTip( NULL ),
pDisableModalUpdating( false ),
pFoxArgs( NULL ),
pFoxArgCount( 0 ){
}

igdeNativeFoxApplication::~igdeNativeFoxApplication(){
	if( pFoxArgs ){
		int i;
		for( i=0; i<pFoxArgCount; i++ ){
			delete [] pFoxArgs[ i ];
		}
		delete [] pFoxArgs;
	}
}

igdeNativeFoxApplication *igdeNativeFoxApplication::CreateNativeApplication( igdeApplication &application ){
	return new igdeNativeFoxApplication( application );
}

void igdeNativeFoxApplication::DestroyNativeApplication(){
	delete this;
}



// Management
///////////////

#ifdef OS_UNIX
void igdeNativeFoxApplication::GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args ){
	// WARNING FOX expects the first parameter to be present. stripping it causes segfaults!
	int i;
	for( i=0; i<argCount; i++ ){
		arguments.Add( decUnicodeString::NewFromUTF8( args[ i ] ) );
	}
}

#elif defined OS_W32
void igdeNativeFoxApplication::GetOSStartUpArguments( decUnicodeStringList &arguments,
const decUnicodeArgumentList &windowsArguments ){
	// WARNING FOX expects the first parameter to be present. stripping it causes segfaults!
	const int count = windowsArguments.GetArgumentCount();
	int i;
	for( i=0; i<count; i++ ){
		arguments.Add( *windowsArguments.GetArgumentAt( i ) );
	}
}
#endif

	/*
void igdeNativeFoxApplication::Initialize(){
	int i;
	for( i=0; i<arguments.GetArgumentCount(); i++ ){
		const decString argument( arguments.GetAt( i ).ToUTF8() );
		
		if( argument == "--notimer" ){
			pUpdateWithTimer = false;
			arguments.RemoveFrom( i-- );
			
		}else if( argument == "--timer" ){
			pUpdateWithTimer = true;
			arguments.RemoveFrom( i-- );
		}
	}
}
	*/

void igdeNativeFoxApplication::Initialize( decUnicodeStringList &arguments ){
	// WARNING FOX expects first parameter to be present. stripping it causes segfaults!
	// 
	// WARNING FOX expects arguments to live for the entire lifetime of the application!
	//         Using temporary strings causes segfaults!
	
	pFoxArgCount = arguments.GetCount();
	pFoxArgs = new char*[ pFoxArgCount + 1 ]; // workaround: fox seems to write past the buffer
	
	int i;
	for( i=0; i<pFoxArgCount; i++ ){
		const decString argument( arguments.GetAt( i ).ToUTF8() );
		pFoxArgs[ i ] = new char[ argument.GetLength() + 1 ];
		strcpy( pFoxArgs[ i ], argument.GetString() );
	}
	
	arguments.RemoveFrom( 0 );
	
	// process arguments
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
	
	// call init. this method is VERY picky that the first argument is present
	init( pFoxArgCount, pFoxArgs );
	
	// create tool tip and application
	pToolTip = new FXToolTip( this, TOOLTIP_PERMANENT ); // TOOLTIP_PERMANENT, TOOLTIP_VARIABLE
	//setTooltipTime( num_milliseconds );
	//setTooltipPause( num_milliseconds );
	
	create();
}

void igdeNativeFoxApplication::Run(){
	// now comes the funny part. to avoid fox getting flooded with messages and to achieve a
	// situation more closely resembling the one found in an actual game the event loop is
	// taken care of on our own. the documentation lacks any information on what the return
	// value of runWhileEvents is in various scenarios. On a mailing list somebody claims that
	// TRUE is returned if there are no more events and most probably FALSE if stop and
	// company has been called. this is the behavior we assume right now in this code.
	// should it not work please let me know what this method is supposed to do.
	
	while( runWhileEvents() ){
		igdeMainWindow * const mainWindow = pOwner->GetMainWindow();
		if( mainWindow ){
			mainWindow->OnFrameUpdate();
		}
	}
}

void igdeNativeFoxApplication::Quit(){
	if( pToolTip ){
		delete pToolTip;
		pToolTip = NULL;
	}
	
	// leak check
	const int widgetCount = igdeUIFoxHelper::DebugCountWindows( NULL );
	if( widgetCount > 1 ){
		printf("igdeNativeFoxApplication: %d leaking widgets\n", widgetCount );
		dumpWidgets();
	}
	
	// fox deletes the window if closed. same goes for the application
	exit( 0 );
}

decColor igdeNativeFoxApplication::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowBackground:
		return igdeUIFoxHelper::ConvertColor( getBackColor() );
		
	case igdeEnvironment::escWindowForeground:
		return igdeUIFoxHelper::ConvertColor( getForeColor() );
		
	case igdeEnvironment::escWidgetBackground:
		return igdeUIFoxHelper::ConvertColor( getBaseColor() );
		
	case igdeEnvironment::escWidgetForeground:
		return igdeUIFoxHelper::ConvertColor( getForeColor() );
		
	case igdeEnvironment::escWidgetHighlight:
		return igdeUIFoxHelper::ConvertColor( getHiliteColor() );
		
	case igdeEnvironment::escWidgetShadow:
		return igdeUIFoxHelper::ConvertColor( getShadowColor() );
		
	case igdeEnvironment::escWidgetSelectedBackground:
		return igdeUIFoxHelper::ConvertColor( getSelbackColor() );
		
	case igdeEnvironment::escWidgetSelectedForeground:
		return igdeUIFoxHelper::ConvertColor( getSelforeColor() );
		
	default:
		return igdeUIFoxHelper::ConvertColor( getBackColor() );
	}
}

void igdeNativeFoxApplication::GetAppFontConfig( igdeFont::sConfiguration &config ) const{
	const FXFont &font = *getNormalFont();
	config.name = font.getName().text();
	config.size = ( float )font.getSize() * 0.1f; // fox fonts are in 1/10pt
	config.bold = font.getActualWeight() > FXFont::Normal;
	config.italic = font.getActualSlant() == FXFont::Italic;
	config.underline = false;
	config.strikeThrough = false;
}

void igdeNativeFoxApplication::ShowError( const deException &exception ) const{
	const decString foxMessage( exception.FormatOutput().Join( "\n" ) );
	FXMessageBox::error( FXApp::instance(), FX::MBOX_OK, "Application Error", "%s", foxMessage.GetString() );
}

void igdeNativeFoxApplication::RunModalWhileShown( igdeWindow &window ){
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
}

#endif
