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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native/toolkit.h"
#include "igdeTabBook.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

#include "native/fox/igdeNativeFoxAutoScroller.h"

class cNativeIgdeTabBookHeader : public FXToggleButton{
	FXDECLARE( cNativeIgdeTabBookHeader )
	
protected:
	cNativeIgdeTabBookHeader();
	
public:
	cNativeIgdeTabBookHeader( FXComposite *parent, const char *text, FXIcon *icon,
		FXObject *target, FXSelector selector, FXuint options,
		int padLeft, int padRight, int padTop, int padBottom );
	virtual ~cNativeIgdeTabBookHeader();
	
	virtual FXbool canFocus() const;
};

FXDEFMAP( cNativeIgdeTabBookHeader ) cNativeIgdeTabBookHeaderMap[] = { };

FXIMPLEMENT( cNativeIgdeTabBookHeader, FXToggleButton, cNativeIgdeTabBookHeaderMap,
	ARRAYNUMBER( cNativeIgdeTabBookHeaderMap ) )

cNativeIgdeTabBookHeader::cNativeIgdeTabBookHeader(){ }

cNativeIgdeTabBookHeader::cNativeIgdeTabBookHeader( FXComposite *parent, const char *text,
FXIcon *icon, FXObject *target, FXSelector selector, FXuint options,
int padLeft, int padRight, int padTop, int padBottom ) :
FXToggleButton( parent, text, text, icon, icon, target, selector, options,
	0, 0, 0, 0, padLeft, padRight, padTop, padBottom ){
}

cNativeIgdeTabBookHeader::~cNativeIgdeTabBookHeader(){
}

FXbool cNativeIgdeTabBookHeader::canFocus() const{
	return false;
}



class cNativeIgdeTabBook : public FXVerticalFrame{
	FXDECLARE( cNativeIgdeTabBook )
	
protected:
	cNativeIgdeTabBook();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_HEADER,
		ID_AUTO_SCROLLER,
// 		ID_BTN_SCROLL_HEADER_LEFT,
// 		ID_BTN_SCROLL_HEADER_RIGHT
	};
	
private:
	igdeTabBook *pOwner;
// 	FXArrowButton *pBtnScrollHeadersLeft;
// 	FXArrowButton *pBtnScrollHeadersRight;
	igdeNativeFoxAutoScroller *pAutoScroller;
	FXHorizontalFrame *pHeaders;
	FXSwitcher *pSwitcher;
	igdeFontReference pFont;
	int pPadLeft;
	int pPadRight;
	int pPadTop;
	int pPadBottom;
	
public:
	cNativeIgdeTabBook( igdeTabBook &owner, FXComposite *parent, FXComposite *windowParent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeTabBook();
	
	inline FXSwitcher *GetSwitcher() const{ return pSwitcher; }
	
	void AddHeader( const igdeTabBook::cHeader &header );
	void RemoveHeader( int index );
	void RemoveAllHeaders();
	void ChangePanel( int index );
	
	long onHeaderCommand( FXObject *sender, FXSelector selector, void *data );
	long onHeaderUpdate( FXObject *sender, FXSelector selector, void *data );
	long onHeaderMouseWheel( FXObject *sender, FXSelector selector, void *data );
	long onHeaderMouseMoved( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	
	static igdeFont *TabBookFont( const igdeTabBook &owner, const igdeGuiTheme &guitheme );
	static int TabBookPadLeft( const igdeGuiTheme &guitheme );
	static int TabBookPadRight( const igdeGuiTheme &guitheme );
	static int TabBookPadTop( const igdeGuiTheme &guitheme );
	static int TabBookPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeTabBook ) cNativeIgdeTabBookMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeTabBook::ID_HEADER, cNativeIgdeTabBook::onHeaderCommand ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeTabBook::ID_HEADER, cNativeIgdeTabBook::onHeaderUpdate ),
	FXMAPFUNC( SEL_MOUSEWHEEL, cNativeIgdeTabBook::ID_HEADER, cNativeIgdeTabBook::onHeaderMouseWheel ),
	FXMAPFUNC( SEL_MOTION, cNativeIgdeTabBook::ID_HEADER, cNativeIgdeTabBook::onHeaderMouseMoved ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeIgdeTabBook::onChildLayoutFlags ),
};

FXIMPLEMENT( cNativeIgdeTabBook, FXVerticalFrame, cNativeIgdeTabBookMap, ARRAYNUMBER( cNativeIgdeTabBookMap ) )

cNativeIgdeTabBook::cNativeIgdeTabBook(){ }

cNativeIgdeTabBook::cNativeIgdeTabBook( igdeTabBook &owner, FXComposite *parent,
	FXComposite *windowParent, int layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pAutoScroller( NULL ),
// pBtnScrollHeadersLeft( NULL ),
// pBtnScrollHeadersRight( NULL ),
pHeaders( NULL ),
pSwitcher( NULL ),
pFont( TabBookFont( owner, guitheme ) ),
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

cNativeIgdeTabBook::~cNativeIgdeTabBook(){
}
	
void cNativeIgdeTabBook::AddHeader( const igdeTabBook::cHeader &header ){
	FXIcon * const icon = header.icon ? ( FXIcon* )header.icon->GetNativeIcon() : NULL;
	
	cNativeIgdeTabBookHeader * const widget = new cNativeIgdeTabBookHeader(
		pHeaders, header.text.GetString(), icon, this, ID_HEADER,
		TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN,
		pPadLeft, pPadRight, pPadTop, pPadBottom );
	widget->setFont( (FXFont*)pFont->GetNativeFont() );
	if( id() ){
		widget->create();
	}
}

void cNativeIgdeTabBook::RemoveHeader( int index ){
	FXWindow * const child = pHeaders->childAtIndex( index );
	if( child ){
		delete child;
	}
}

void cNativeIgdeTabBook::RemoveAllHeaders(){
	while( pHeaders->numChildren() > 0 ){
		delete pHeaders->childAtIndex( 0 );
	}
}

void cNativeIgdeTabBook::ChangePanel( int index ){
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


long cNativeIgdeTabBook::onHeaderCommand( FXObject *sender, FXSelector selector, void *data ){
	ChangePanel( pHeaders->indexOfChild( ( FXWindow* )sender ) );
	return 1;
}

long cNativeIgdeTabBook::onHeaderUpdate( FXObject *sender, FXSelector selector, void *data ){
	const int headerIndex = pHeaders->indexOfChild( ( FXWindow* )sender );
	const int switcherIndex = pSwitcher->getCurrent();
	
	return sender->handle( this, FXSEL( SEL_COMMAND,
		headerIndex == switcherIndex ? ID_CHECK : ID_UNCHECK ), NULL );
}

long cNativeIgdeTabBook::onHeaderMouseWheel( FXObject *sender, FXSelector selector, void *data ){
	const FXEvent &event = *( ( FXEvent* )data );
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

long cNativeIgdeTabBook::onHeaderMouseMoved( FXObject *sender, FXSelector selector, void *data ){
	return pAutoScroller->onChildMouseMoved( sender, selector, data );
}

long cNativeIgdeTabBook::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}



igdeFont *cNativeIgdeTabBook::TabBookFont( const igdeTabBook &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
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
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeIgdeTabBook::TabBookPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeTabBook::TabBookPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeTabBook::TabBookPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeTabBook::TabBookPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::tabBookPaddingBottom, DEFAULT_PAD );
}



// Class igdeTabBook::cHeader
///////////////////////////////

igdeTabBook::cHeader::cHeader( const char *ptext, igdeIcon *picon, const char *pdescription ) :
text( ptext ),
description( pdescription ),
icon( picon ){
}

igdeTabBook::cHeader::~cHeader(){
}



// Class igdeTabBook
//////////////////////

// Constructor, destructor
////////////////////////////

igdeTabBook::igdeTabBook( igdeEnvironment &environment ) :
igdeContainer( environment ),
pActivePanel( -1 ){
}

igdeTabBook::~igdeTabBook(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTabBook::SetActivePanel( int index ){
	if( index == pActivePanel ){
		return;
	}
	
	if( index == -1 ){
		if( GetChildCount() > 0 ){
			DETHROW( deeInvalidParam );
		}
		
	}else if( index < 0 || index >= GetChildCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pActivePanel = index;
	
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( native ){
		native->ChangePanel( index );
	}
}



void igdeTabBook::AddChild( igdeWidget *child, const char *text ){
	AddChild( child, text, NULL, "" );
}

void igdeTabBook::AddChild( igdeWidget *child, const char *text, igdeIcon *icon ){
	AddChild( child, text, icon, "" );
}

void igdeTabBook::AddChild( igdeWidget *child, const char *text, igdeIcon *icon,
const char *description ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::AddChild( child );
	
	deObjectReference header;
	try{
		header.TakeOver( new cHeader( text, icon, description ) );
		pHeaders.Add( header );
		
	}catch( const deException & ){
		igdeContainer::RemoveChild( child );
		throw;
	}
	
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( ! native ){
		return;
	}
	
	try{
		native->AddHeader( ( const cHeader & )header );
		
	}catch( const deException & ){
		pHeaders.RemoveFrom( pHeaders.GetCount() - 1 );
		igdeContainer::RemoveChild( child );
		throw;
	}
}

void igdeTabBook::AddChild( igdeWidget *child ){
	DETHROW( deeInvalidParam );
}

void igdeTabBook::RemoveChild( igdeWidget *child ){
	const int index = IndexOfChild( child );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( native ){
		native->RemoveHeader( index );
	}
	pHeaders.RemoveFrom( index );
	
	igdeContainer::RemoveChild( child );
}

void igdeTabBook::RemoveAllChildren(){
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( native ){
		native->RemoveAllHeaders();
	}
	pHeaders.RemoveAll();
	
	igdeContainer::RemoveAllChildren();
}



void igdeTabBook::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeContainer * const parent = GetParent();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )parent->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeIgdeTabBook * const native = new cNativeIgdeTabBook(
		*this, foxParent, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
	
	native->ChangePanel( pActivePanel );
}

void igdeTabBook::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeTabBook* )GetNativeWidget();
	DropNativeWidget();
}

void *igdeTabBook::GetNativeContainer() const{
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( ! native ){
		return NULL;
	}
	
	return native->GetSwitcher();
}



void igdeTabBook::CreateChildWidgetNativeWidgets(){
	cNativeIgdeTabBook * const native = ( cNativeIgdeTabBook* )GetNativeWidget();
	if( ! native ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::CreateChildWidgetNativeWidgets();
	
	const int count = pHeaders.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		native->AddHeader( *( ( cHeader* )pHeaders.GetAt( i ) ) );
	}
}
