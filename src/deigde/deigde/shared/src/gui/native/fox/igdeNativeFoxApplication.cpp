/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef OS_UNIX
#include <dragengine/app/deOSUnix.h>
#elif defined OS_W32
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#endif



// Event map
//////////////

FXIMPLEMENT( igdeNativeFoxApplication, FXApp, nullptr, 0 )



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
pFoxArgCount( 0 ),
pDisplayScaleFactor(100){
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
		#ifdef OS_W32_VS
		strcpy_s( pFoxArgs[ i ], argument.GetLength() + 1, argument.GetString() );
		#else
		strcpy( pFoxArgs[ i ], argument.GetString() );
		#endif
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
	
	// we have to fix the normal font for dpi awareness. at this point in time we do not yet
	// have safe access to igdeEnvironment. thus we have to do a bit of trickery here.
	#ifdef OS_UNIX
	pDisplayScaleFactor = deOSUnix().GetDisplayCurrentScaleFactor(0);
	#elif defined OS_W32
	pDisplayScaleFactor = deOSWindows().GetDisplayCurrentScaleFactor(0);
	#endif
	
	const FXFont &fontNormal = *getNormalFont();
	pAppFontConfig.name = fontNormal.getName().text();
	pAppFontConfig.size = (float)fontNormal.getSize() * 0.1f; // fox fonts are in 1/10pt
	pAppFontConfig.bold = fontNormal.getActualWeight() > FXFont::Normal;
	pAppFontConfig.italic = fontNormal.getActualSlant() == FXFont::Italic;
	pAppFontConfig.underline = false;
	pAppFontConfig.strikeThrough = false;
	
	FXFontDesc fontDesc(fontNormal.getFontDesc());
	fontDesc.size = (FXuint)((int)fontDesc.size * pDisplayScaleFactor / 100);
	setNormalFont(new FXFont(this, fontDesc));
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
			if( ! mainWindow->GetNativeWidget() ){
				return; // sometimes FOX manages to make us miss this event
			}
			
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

void igdeNativeFoxApplication::GetAppFontConfig(igdeFont::sConfiguration &config) const{
	config = pAppFontConfig;
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

int igdeNativeFoxApplication::GetDisplayScaleFactor(){
	return pDisplayScaleFactor;
}

#endif
