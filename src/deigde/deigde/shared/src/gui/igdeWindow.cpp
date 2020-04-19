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

#include "native/toolkit.h"
#include "igdeWindow.h"
#include "igdeCommonDialogs.h"
#include "igdeContainerReference.h"
#include "igdeWidget.h"
#include "igdeWidgetReference.h"
#include "resources/igdeIcon.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

namespace{

class cNativeWidget : public FXTopWindow{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
	};
	
private:
	igdeWindow *pOwner;
	
public:
	cNativeWidget( igdeWindow &owner, const igdeGuiTheme &guitheme );
	virtual ~cNativeWidget();
	
	virtual void UpdateEnabled();
	virtual void UpdatePosition();
	virtual void UpdateIcon();
	
	long onConfigure( FXObject *sender, FXSelector selector, void *data );
	long onClose( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	
	static int WindowFlags( const igdeWindow &owner );
	
	static int WindowPadLeft( const igdeGuiTheme &guitheme );
	static int WindowPadRight( const igdeGuiTheme &guitheme );
	static int WindowPadTop( const igdeGuiTheme &guitheme );
	static int WindowPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, cNativeWidget::onConfigure ),
	FXMAPFUNC( SEL_CLOSE, 0, cNativeWidget::onClose ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidget::onChildLayoutFlags )
};


FXIMPLEMENT( cNativeWidget, FXTopWindow, cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget( igdeWindow &owner, const igdeGuiTheme &guitheme ) :
FXTopWindow( FXApp::instance(), owner.GetTitle().GetString(),
	owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL,
	owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL,
	WindowFlags( owner ),
	owner.GetPosition().x, owner.GetPosition().y, owner.GetSize().x, owner.GetSize().y,
	WindowPadLeft( guitheme ), WindowPadRight( guitheme ),
	WindowPadTop( guitheme ), WindowPadBottom( guitheme ),
	0, 0 ),
pOwner( &owner ){
}

cNativeWidget::~cNativeWidget(){
}


void cNativeWidget::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void cNativeWidget::UpdatePosition(){
	if( pOwner->GetPosition().x == getX() && pOwner->GetPosition().y == getY() ){
		return;
	}
	
	move( pOwner->GetPosition().x, pOwner->GetPosition().y );
}

void cNativeWidget::UpdateIcon(){
	FXIcon * const icon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( icon );
	setMiniIcon( icon );
}


long cNativeWidget::onConfigure( FXObject *sender, FXSelector selector, void *data ){
	const int result = FXTopWindow::onConfigure( sender, selector, data );
	pOwner->SetPosition( decPoint( getX(), getY() ) );
	pOwner->SetSize( decPoint( getWidth(), getHeight() ) );
	pOwner->OnResize();
	return result;
}

long cNativeWidget::onClose( FXObject*, FXSelector, void* ){
	return pOwner->CloseWindow() ? 0 : 1;
}

long cNativeWidget::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}


int cNativeWidget::WindowFlags( const igdeWindow &owner ){
	int flags = DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE | DECOR_MENU;
	
	if( owner.GetCanResize() ){
		flags |=  DECOR_RESIZE; //DECOR_STRETCHABLE, DECOR_SHRINKABLE
	}
	
	return flags;
}


int cNativeWidget::WindowPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingLeft, 0 );
}

int cNativeWidget::WindowPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingRight, 0 );
}

int cNativeWidget::WindowPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingTop, 0 );
}

int cNativeWidget::WindowPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingBottom, 0 );
}

}



// Class igdeWindow
/////////////////////

// Constructor, destructor
////////////////////////////

igdeWindow::igdeWindow( igdeEnvironment &environment, const char *title, igdeIcon *icon, bool canResize ) :
igdeContainer( environment ),
pTitle( title ),
pIcon( icon ),
pCanResize( canResize ),
pPosition( 10, 50 ),
pSize( 400, 300 ),
pEnabled( true )
{
	SetVisible( false );
}

igdeWindow::~igdeWindow(){
}



// Management
///////////////

void igdeWindow::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeWindow::SetIcon( igdeIcon* icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeWindow::SetSize( const decPoint &size ){
	if( size == pSize ){
		return;
	}
	
	pSize = size;
	OnSizeChanged();
}

void igdeWindow::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeWindow::SetPosition( const decPoint &position ){
	if( position == pPosition ){
		return;
	}
	
	pPosition = position;
	OnPositionChanged();
}



void igdeWindow::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}



bool igdeWindow::CloseWindow(){
	return true;
}

void igdeWindow::Close(){
	if( GetNativeWidget() ){
		DestroyNativeWidget();
	}
}



// IGDE Internal Use Only
///////////////////////////

void igdeWindow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	cNativeWidget * const native = new cNativeWidget( *this, *GetGuiTheme() );
	SetNativeWidget( native );
	CreateChildWidgetNativeWidgets();
	native->create();
	native->raise();
}

void igdeWindow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	//delete ( cNativeWidget* )GetNativeWidget();
	( ( cNativeWidget* )GetNativeWidget() )->close( false );
	DropNativeWidget();
}



void igdeWindow::OnTitleChanged(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->setTitle( pTitle.GetString() );
	}
}

void igdeWindow::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeWindow::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->resize( pSize.x, pSize.y );
	}
}

void igdeWindow::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->UpdatePosition();
	}
}

void igdeWindow::OnVisibleChanged(){
	if( GetVisible() ){
		CreateNativeWidget();
	}
	igdeWidget::OnVisibleChanged();
}

void igdeWindow::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->UpdateEnabled();
	}
}
