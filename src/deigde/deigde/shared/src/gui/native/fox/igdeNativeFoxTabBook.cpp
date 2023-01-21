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

#include "igdeNativeFoxAutoScroller.h"
#include "igdeNativeFoxTabBook.h"
#include "foxIcons.h"
#include "../../igdeTabBook.h"
#include "../../igdeTabBook.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Header
///////////

class igdeNativeFoxTabBookHeader : public FXToggleButton{
	FXDECLARE( igdeNativeFoxTabBookHeader )
	
protected:
	igdeNativeFoxTabBookHeader();
	
public:
	igdeNativeFoxTabBookHeader( FXComposite *pparent, const char *text, FXIcon *iicon,
		FXObject *ttarget, FXSelector selector, FXuint ooptions,
		int padLeft, int padRight, int padTop, int padBottom );
	virtual ~igdeNativeFoxTabBookHeader();
	
	virtual FXbool canFocus() const;
};

FXDEFMAP( igdeNativeFoxTabBookHeader ) igdeNativeFoxTabBookHeaderMap[] = { };

FXIMPLEMENT( igdeNativeFoxTabBookHeader, FXToggleButton, igdeNativeFoxTabBookHeaderMap,
	ARRAYNUMBER( igdeNativeFoxTabBookHeaderMap ) )

igdeNativeFoxTabBookHeader::igdeNativeFoxTabBookHeader(){ }

igdeNativeFoxTabBookHeader::igdeNativeFoxTabBookHeader( FXComposite *pparent, const char *text,
FXIcon *iicon, FXObject *ttarget, FXSelector selector, FXuint ooptions,
int padLeft, int padRight, int padTop, int padBottom ) :
FXToggleButton( pparent, text, text, iicon, iicon, ttarget, selector, ooptions,
	0, 0, 0, 0, padLeft, padRight, padTop, padBottom ){
}

igdeNativeFoxTabBookHeader::~igdeNativeFoxTabBookHeader(){
}

FXbool igdeNativeFoxTabBookHeader::canFocus() const{
	return false;
}



// Events
///////////

FXDEFMAP( igdeNativeFoxTabBook ) igdeNativeFoxTabBookMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxTabBook::ID_HEADER, igdeNativeFoxTabBook::onHeaderCommand ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxTabBook::ID_HEADER, igdeNativeFoxTabBook::onHeaderUpdate ),
	FXMAPFUNC( SEL_MOUSEWHEEL, igdeNativeFoxTabBook::ID_HEADER, igdeNativeFoxTabBook::onHeaderMouseWheel ),
	FXMAPFUNC( SEL_MOTION, igdeNativeFoxTabBook::ID_HEADER, igdeNativeFoxTabBook::onHeaderMouseMoved ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxTabBook::onChildLayoutFlags ),
};



// Class igdeNativeFoxTabBook
///////////////////////////////

FXIMPLEMENT( igdeNativeFoxTabBook, FXVerticalFrame, igdeNativeFoxTabBookMap, ARRAYNUMBER( igdeNativeFoxTabBookMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxTabBook::igdeNativeFoxTabBook(){ }

igdeNativeFoxTabBook::igdeNativeFoxTabBook( igdeTabBook &powner, FXComposite *pparent,
	FXComposite *windowParent, int layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pAutoScroller( NULL ),
// pBtnScrollHeadersLeft( NULL ),
// pBtnScrollHeadersRight( NULL ),
pHeaders( NULL ),
pSwitcher( NULL ),
pFont( TabBookFont( powner, guitheme ) ),
pPadLeft( TabBookPadLeft( guitheme ) ),
pPadRight( TabBookPadRight( guitheme ) ),
pPadTop( TabBookPadTop( guitheme ) ),
pPadBottom( TabBookPadBottom( guitheme ) )
{
// 	FXHorizontalFrame * const frameHeader = new FXHorizontalFrame( this,
// 		LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	pAutoScroller = new igdeNativeFoxAutoScroller( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X );
	igdeNativeFoxAutoScroller * const frameHeader = pAutoScroller;
	
// 	pBtnScrollHeadersLeft = new FXArrowButton( frameHeader, this, ID_BTN_SCROLL_HEADER_LEFT,
// 		ARROW_LEFT | FRAME_SUNKEN | LAYOUT_FILL_Y, 0, 0, 0, 0,
// 		pPadLeft, pPadRight, pPadTop, pPadBottom );
// 	pBtnScrollHeadersLeft->hide();
	
	pHeaders = new FXHorizontalFrame( frameHeader, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	pHeaders->setBackColor( getApp()->getShadowColor() );
	
// 	pBtnScrollHeadersRight = new FXArrowButton( frameHeader, this, ID_BTN_SCROLL_HEADER_RIGHT,
// 		ARROW_RIGHT | FRAME_SUNKEN | LAYOUT_FILL_Y, 0, 0, 0, 0,
// 		pPadLeft, pPadRight, pPadTop, pPadBottom );
// 	pBtnScrollHeadersRight->hide();
	
	new FXSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );
	pSwitcher = new FXSwitcher( this, /*SWITCHER_VCOLLAPSE |*/ LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0 );
}

igdeNativeFoxTabBook::~igdeNativeFoxTabBook(){
}

igdeNativeFoxTabBook *igdeNativeFoxTabBook::CreateNativeWidget( igdeTabBook &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxTabBook( powner, pparent, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxTabBook::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxTabBook::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxTabBook::AddHeader( const igdeTabBook::cHeader &header ){
	FXIcon * const iicon = header.icon ? ( FXIcon* )header.icon->GetNativeIcon() : NULL;
	
	igdeNativeFoxTabBookHeader * const widget = new igdeNativeFoxTabBookHeader(
		pHeaders, header.text.GetString(), iicon, this, ID_HEADER,
		TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN,
		pPadLeft, pPadRight, pPadTop, pPadBottom );
	widget->setFont( (FXFont*)pFont->GetNativeFont() );
	if( id() ){
		widget->create();
	}
}

void igdeNativeFoxTabBook::RemoveHeader( int index ){
	FXWindow * const child = pHeaders->childAtIndex( index );
	if( child ){
		delete child;
	}
}

void igdeNativeFoxTabBook::RemoveAllHeaders(){
	while( pHeaders->numChildren() > 0 ){
		delete pHeaders->childAtIndex( 0 );
	}
}

void igdeNativeFoxTabBook::ChangePanel( int index ){
	const int count = pSwitcher->numChildren();
	if( index == -1 && count == 0 ){
		pOwner->SetActivePanel( -1 );
		return;
	}
	
	if( index < 0 || index >= count ){
		return;
	}
	
	pSwitcher->setCurrent( index );
	pHeaders->update();
	
	pOwner->SetActivePanel( index );
}

void *igdeNativeFoxTabBook::GetNativeContainer(){
	return pSwitcher;
}



igdeFont *igdeNativeFoxTabBook::TabBookFont( const igdeTabBook &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::tabBookFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::tabBookFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::tabBookFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::tabBookFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxTabBook::TabBookPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxTabBook::TabBookPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxTabBook::TabBookPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxTabBook::TabBookPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxTabBook::onHeaderCommand( FXObject *sender, FXSelector, void* ){
	ChangePanel( pHeaders->indexOfChild( ( FXWindow* )sender ) );
	return 1;
}

long igdeNativeFoxTabBook::onHeaderUpdate( FXObject *sender, FXSelector, void* ){
	const int headerIndex = pHeaders->indexOfChild( ( FXWindow* )sender );
	const int switcherIndex = pSwitcher->getCurrent();
	
	return sender->handle( this, FXSEL( SEL_COMMAND,
		headerIndex == switcherIndex ? ID_CHECK : ID_UNCHECK ), NULL );
}

long igdeNativeFoxTabBook::onHeaderMouseWheel( FXObject*, FXSelector, void *pdata ){
	const FXEvent &event = *( ( FXEvent* )pdata );
	const int index = pSwitcher->getCurrent();
	
	if( event.code > 0 ){ // wheel down
		if( index > 0 ){
			ChangePanel( index - 1 );
			
		}else{
			//ChangePanel( pSwitcher->numChildren() - 1 );  // wrapping around is annoying
		}
		
	}else if( event.code < 0 ){ // wheel up
		if( index < pSwitcher->numChildren() - 1 ){
			ChangePanel( index + 1 );
			
		}else{
			//ChangePanel( 0 );  // wrapping around is annoying
		}
	}
	
	return 1;
}

long igdeNativeFoxTabBook::onHeaderMouseMoved( FXObject *sender, FXSelector selector, void *pdata ){
	return pAutoScroller->onChildMouseMoved( sender, selector, pdata );
}

long igdeNativeFoxTabBook::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

#endif
