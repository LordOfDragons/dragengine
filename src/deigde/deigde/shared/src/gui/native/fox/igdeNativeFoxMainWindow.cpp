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

#include "igdeNativeFoxMainWindow.h"
#include "../../igdeMainWindow.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gui/resources/igdeIcon.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxMainWindow ) igdeNativeFoxMainWindowMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxMainWindow::onConfigure ),
	FXMAPFUNC( SEL_MAP, 0, igdeNativeFoxMainWindow::onMap ),
	FXMAPFUNC( SEL_CLOSE, 0, igdeNativeFoxMainWindow::onClose ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxMainWindow::onChildLayoutFlags ),
	FXMAPFUNC( SEL_MINIMIZE, 0, igdeNativeFoxMainWindow::onMinimized ),
	FXMAPFUNC( SEL_RESTORE, 0, igdeNativeFoxMainWindow::onRestored ),
	FXMAPFUNC( SEL_MAXIMIZE, 0, igdeNativeFoxMainWindow::onMaximized )
};

FXIMPLEMENT( igdeNativeFoxMainWindow, FXMainWindow,
	igdeNativeFoxMainWindowMap, ARRAYNUMBER( igdeNativeFoxMainWindowMap ) )



// Class igdeNativeFoxMainWindow
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxMainWindow::igdeNativeFoxMainWindow(){
}

igdeNativeFoxMainWindow::igdeNativeFoxMainWindow( igdeMainWindow &powner ) :
FXMainWindow( FXApp::instance(), powner.GetTitle().GetString(),
	powner.GetIcon() ? ( FXIcon* ) powner.GetIcon()->GetNativeIcon() : NULL,
	powner.GetIcon() ? ( FXIcon* ) powner.GetIcon()->GetNativeIcon() : NULL,
	DECOR_ALL, 0, 0, powner.GetInitialSize().x, powner.GetInitialSize().y ),
pOwner( &powner )
{
	if( ! pOwner->GetVisible() ){
		hide();
	}
}

igdeNativeFoxMainWindow::~igdeNativeFoxMainWindow(){
}

igdeNativeFoxMainWindow *igdeNativeFoxMainWindow::CreateNativeWidget( igdeMainWindow &powner ){
	return new igdeNativeFoxMainWindow( powner );
}

void igdeNativeFoxMainWindow::PostCreateNativeWidget(){
	// NOTE we need to fix the maximize problem during showing the window using PLACEMENT_* .
	//      if we try to maximize(true) here things become the opposite. no idea how FOX
	//      manages to mess up so hard. calling maximize at the end does work albeit looking
	//      ugly while doing so. fixing this when main window is converted to new UI system
	create();
	
	// here maximize seems to work
	maximize( true );
	
	raise();
}

void igdeNativeFoxMainWindow::DestroyNativeWidget(){
	// we use close() on purpose instead of delete because fox requires this
	//delete ( igdeNativeFoxMainWindow* )GetNativeWidget();
	close( false );
}



// Management
///////////////

void igdeNativeFoxMainWindow::create(){
	FXMainWindow::create();
	show( PLACEMENT_SCREEN );
}

void igdeNativeFoxMainWindow::destroy(){
	pOwner->GetEngineController().UnparentMainRenderWindow();
	FXMainWindow::destroy();
}



decColor igdeNativeFoxMainWindow::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowBackground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getBackColor() );
		
	case igdeEnvironment::escWindowForeground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getForeColor() );
		
	case igdeEnvironment::escWidgetBackground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getBaseColor() );
		
	case igdeEnvironment::escWidgetForeground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getForeColor() );
		
	case igdeEnvironment::escWidgetHighlight:
		return igdeUIFoxHelper::ConvertColor( getApp()->getHiliteColor() );
		
	case igdeEnvironment::escWidgetShadow:
		return igdeUIFoxHelper::ConvertColor( getApp()->getShadowColor() );
		
	case igdeEnvironment::escWidgetSelectedBackground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getSelbackColor() );
		
	case igdeEnvironment::escWidgetSelectedForeground:
		return igdeUIFoxHelper::ConvertColor( getApp()->getSelforeColor() );
		
	default:
		return igdeUIFoxHelper::ConvertColor( getApp()->getBackColor() );
	}
}



void igdeNativeFoxMainWindow::UpdateWindowState(){
	switch( pOwner->GetWindowState() ){
	case igdeMainWindow::ewsNormal:
		if( ! isMinimized() && ! isMaximized() ){
			break;
		}
		if( ! restore() ){
			SetWindowState();
		}
		break;
		
	case igdeMainWindow::ewsMinimized:
		if( isMinimized() ){
			break;
		}
		if( ! minimize() ){
			SetWindowState();
		}
		break;
		
	case igdeMainWindow::ewsMaximized:
		if( isMaximized() ){
			break;
		}
		if( ! maximize() ){
			SetWindowState();
		}
		break;
		
	default:
		break;
	}
}

void igdeNativeFoxMainWindow::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxMainWindow::UpdatePosition(){
	if( pOwner->GetPosition().x == getX() && pOwner->GetPosition().y == getY() ){
		return;
	}
	
	move( pOwner->GetPosition().x, pOwner->GetPosition().y );
}

void igdeNativeFoxMainWindow::UpdateIcon(){
	FXIcon * const iicon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( iicon );
	setMiniIcon( iicon );
}

void igdeNativeFoxMainWindow::UpdateTitle(){
	setTitle( pOwner->GetTitle().GetString() );
}

void igdeNativeFoxMainWindow::UpdateSize(){
	resize( pOwner->GetSize().x, pOwner->GetSize().y );
}

void igdeNativeFoxMainWindow::SetWindowState(){
	if( isMaximized() ){
		pOwner->SetWindowState( igdeMainWindow::ewsMaximized );
		
	}else if( isMinimized() ){
		pOwner->SetWindowState( igdeMainWindow::ewsMinimized );
		
	}else{
		pOwner->SetWindowState( igdeMainWindow::ewsNormal );
	}
}

void igdeNativeFoxMainWindow::GetAppFontConfig( igdeFont::sConfiguration &config ){
	const FXFont &font = *FXApp::instance()->getNormalFont();
	config.name = font.getName().text();
	config.size = ( float )font.getSize() * 0.1f; // fox fonts are in 1/10pt
	config.bold = font.getActualWeight() > FXFont::Normal;
	config.italic = font.getActualSlant() == FXFont::Italic;
	config.underline = false;
	config.strikeThrough = false;
}



long igdeNativeFoxMainWindow::onConfigure( FXObject *sender, FXSelector selector, void *pdata ){
	const int result = FXMainWindow::onConfigure( sender, selector, pdata );
	
	const decPoint position( getX(), getY() );
	if( position != pOwner->GetPosition() ){
		pOwner->SetPosition( position );
	}
	
	const decPoint size( getWidth(), getHeight() );
	if( size != pOwner->GetSize() ){
		pOwner->SetSize( size );
		pOwner->OnResize();
	}
	return result;
}

long igdeNativeFoxMainWindow::onMap( FXObject*, FXSelector, void* ){
	pOwner->GetEngineController().InitEngine();
	pOwner->StartEngine();
	return 1;
}

long igdeNativeFoxMainWindow::onClose( FXObject*, FXSelector, void* ){
	if( pOwner->CloseWindow() ){
		pOwner->Close();
	}
	return 1;
}

long igdeNativeFoxMainWindow::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

long igdeNativeFoxMainWindow::onMinimized( FXObject*, FXSelector, void* ){
	SetWindowState();
	return 0;
}

long igdeNativeFoxMainWindow::onRestored( FXObject*, FXSelector, void* ){
	SetWindowState();
	return 0;
}

long igdeNativeFoxMainWindow::onMaximized( FXObject*, FXSelector, void* ){
	// NOTE listening to minimize/maximize event is not enough since FOX has a bug when
	//      uses on multi-desktop. switching the desktop causes the minimzed window to
	//      get the maximize event send twice although it is still minimized. by checking
	//      the window state directly this problem can be solved
	SetWindowState();
	return 0;
}

#endif
