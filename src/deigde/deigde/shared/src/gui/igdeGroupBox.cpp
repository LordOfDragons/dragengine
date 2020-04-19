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
#include "igdeGroupBox.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////

namespace{

class cNativeWidget : public FXGroupBox{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
public:
	enum eFoxIDs{
		ID_SELF = FXGroupBox::ID_LAST,
	};
	
private:
	igdeGroupBox *pOwner;
	int pUncollapsedFlags;
	int pCollapsedFlags;
	igdeFontReference pFont;
	
public:
	cNativeWidget( igdeGroupBox &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	virtual ~cNativeWidget();
	
	virtual FXint getDefaultHeight();
	
	void UpdateCollapsed();
	
	long onLeftMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	
	static int GroupBoxFlags( const igdeGroupBox &owner );
	static igdeFont *GroupBoxFont( const igdeGroupBox &owner, const igdeGuiTheme &guitheme );
	static int GroupBoxPadLeft( const igdeGuiTheme &guitheme );
	static int GroupBoxPadRight( const igdeGuiTheme &guitheme );
	static int GroupBoxPadTop( const igdeGuiTheme &guitheme );
	static int GroupBoxPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, cNativeWidget::onLeftMouseDown ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidget::onChildLayoutFlags )
};

FXIMPLEMENT( cNativeWidget, FXGroupBox, cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget( igdeGroupBox &owner, FXComposite *parent,
int layoutFlags,const igdeGuiTheme &guitheme ) :
FXGroupBox( parent, owner.GetTitle().GetString(), layoutFlags,
	0, 0, 0, 0,
	GroupBoxPadLeft( guitheme ), GroupBoxPadRight( guitheme ),
	GroupBoxPadTop( guitheme ), GroupBoxPadBottom( guitheme ) ),
pOwner( &owner ),
pUncollapsedFlags( layoutFlags ),
pCollapsedFlags( layoutFlags & ~LAYOUT_FILL_Y ),
pFont( GroupBoxFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( owner.GetCollapsed() ){
		setLayoutHints( pCollapsedFlags );
	}
	
// 	setTipText( owner.GetDescription().GetString() ); // not supported
// 	setHelpText( owner.GetDescription().GetString() );
}

cNativeWidget::~cNativeWidget(){
}

FXint cNativeWidget::getDefaultHeight(){
	if( pOwner->GetCollapsed() ){
		return getFont()->getSize() / 10 + 11;
		
	}else{
		return FXGroupBox::getDefaultHeight();
	}
}


void cNativeWidget::UpdateCollapsed(){
	setLayoutHints( pOwner->GetCollapsed() ? pCollapsedFlags : pUncollapsedFlags );
	
	FXWindow *parent = getParent();
	update();
	while( parent ){
		parent->recalc();
		parent = parent->getParent();
	}
}


long cNativeWidget::onLeftMouseDown( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetCanCollapse() ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	const int y = event.win_y;
	
	// change collapsed state only if clicked in the area of the caption
	// TODO figure out the correct height. check out FXPacker source to see where it places
	//      the first child item using LAYOUT_SIDE_TOP. this should give a hint
	if( y >= 0 && y < /*15*/ ( int )getFont()->getSize() / 10 + 11 ){
		pOwner->SetCollapsed( ! pOwner->GetCollapsed() );
	}
	
	return 1;
}

long cNativeWidget::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	if( pOwner->GetStretchLast() && index == pOwner->GetChildCount() - 1 ){
		clflags.flags |= LAYOUT_FILL_Y;
	}
	
	return 1;
}


int cNativeWidget::GroupBoxFlags( const igdeGroupBox &owner ){
	int flags = FRAME_RIDGE;
	
	switch( owner.GetTitleAlignment() ){
	case igdeGroupBox::etaLeft:
		flags |= GROUPBOX_TITLE_LEFT;
		break;
		
	case igdeGroupBox::etaRight:
		flags |= GROUPBOX_TITLE_RIGHT;
		break;
		
	case igdeGroupBox::etaCenter:
		flags |= GROUPBOX_TITLE_CENTER;
		break;
		
	default:
		flags |= GROUPBOX_TITLE_LEFT;
	}
	
	return flags;
}

igdeFont *cNativeWidget::GroupBoxFont( const igdeGroupBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::groupBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::groupBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::groupBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::groupBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeWidget::GroupBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingLeft, DEFAULT_PAD );
}

int cNativeWidget::GroupBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingRight, DEFAULT_PAD );
}

int cNativeWidget::GroupBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingTop, DEFAULT_PAD );
}

int cNativeWidget::GroupBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::groupBoxPaddingBottom, DEFAULT_PAD );
}

}



// Class igdeGroupBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pCanCollapse( false ),
pCollapsed( false ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	const char *description, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pDescription( description ),
pCanCollapse( false ),
pCollapsed( false ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	bool collapsed, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pCanCollapse( true ),
pCollapsed( collapsed ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	const char *description, bool collapsed, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pDescription( description ),
pCanCollapse( true ),
pCollapsed( collapsed ),
pStretchLast( false ){
}

igdeGroupBox::~igdeGroupBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeGroupBox::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeGroupBox::SetTitleAlignment( eTitleAlignment titleAlignment ){
	if( pTitleAlignment == titleAlignment ){
		return;
	}
	
	pTitleAlignment = titleAlignment;
	OnTitleAlignmentChanged();
}

void igdeGroupBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeGroupBox::SetCanCollapse( bool canCollapse ){
	if( pCanCollapse == canCollapse ){
		return;
	}
	
	if( ! canCollapse ){
		SetCollapsed( false );
	}
	
	pCanCollapse = canCollapse;
	OnCanCollapseChanged();
}

void igdeGroupBox::SetCollapsed( bool collapsed ){
	if( pCollapsed == collapsed ){
		return;
	}
	
	pCollapsed = collapsed;
	OnCollapsedChanged();
}

void igdeGroupBox::SetStretchLast( bool stretchLast ){
	if( stretchLast == pStretchLast ){
		return;
	}
	pStretchLast = stretchLast;
	OnStretchLastChanged();
}



void igdeGroupBox::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}

void igdeGroupBox::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	/* // TODO if stretching last widget is supported activate this
	if( GetChildCount() > 0 ){
		igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( GetChildCount() - 1 ) );
		( ( FXPacker* )GetNativeContainer() )->recalc();
	}
	*/
}



void igdeGroupBox::CreateNativeWidget(){
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
	
	int flags = cNativeWidget::GroupBoxFlags( *this );
	flags |= igdeUIFoxHelper::GetChildLayoutFlags( this );
	
	cNativeWidget * const groupbox = new cNativeWidget( *this, foxParent, flags, *GetGuiTheme() );
	SetNativeWidget( groupbox );
	if( foxParent->id() ){
		groupbox->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeGroupBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeWidget* )GetNativeWidget();
	DropNativeWidget();
}



void igdeGroupBox::OnTitleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &groupbox = *( ( cNativeWidget* )GetNativeWidget() );
	groupbox.setText( pTitle.GetString() );
}

void igdeGroupBox::OnTitleAlignmentChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &groupbox = *( ( cNativeWidget* )GetNativeWidget() );
	groupbox.setGroupBoxStyle( cNativeWidget::GroupBoxFlags( *this ) );
}

void igdeGroupBox::OnDescriptionChanged(){
	/* // not supported
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &groupbox = *( ( cNativeWidget* )GetNativeWidget() );
	groupbox.setTipText( pDescription.GetString() );
	groupbox.setHelpText( pDescription.GetString() );
	*/
}

void igdeGroupBox::OnCanCollapseChanged(){
}

void igdeGroupBox::OnCollapsedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &groupbox = *( ( cNativeWidget* )GetNativeWidget() );
	groupbox.UpdateCollapsed();
}

void igdeGroupBox::OnStretchLastChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	const int count = GetChildCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( i ) );
	}
	( ( FXPacker* )GetNativeContainer() )->recalc();
}
