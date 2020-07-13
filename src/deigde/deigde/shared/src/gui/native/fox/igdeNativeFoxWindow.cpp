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

#include "igdeNativeFoxWindow.h"
#include "foxIcons.h"
#include "../../igdeWindow.h"
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



// Events
///////////

FXDEFMAP( igdeNativeFoxWindow ) igdeNativeFoxWindowMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxWindow::onConfigure ),
	FXMAPFUNC( SEL_CLOSE, 0, igdeNativeFoxWindow::onClose ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxWindow::onChildLayoutFlags )
};



// Class igdeNativeFoxWindow
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxWindow, FXTopWindow, igdeNativeFoxWindowMap, ARRAYNUMBER( igdeNativeFoxWindowMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxWindow::igdeNativeFoxWindow(){ }

igdeNativeFoxWindow::igdeNativeFoxWindow( igdeWindow &owner, const igdeGuiTheme &guitheme ) :
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

igdeNativeFoxWindow::~igdeNativeFoxWindow(){
}

igdeNativeFoxWindow *igdeNativeFoxWindow::CreateNativeWidget( igdeWindow &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxWindow( owner, *owner.GetGuiTheme() );
}

void igdeNativeFoxWindow::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( ! parent.id() ){
		return;
	}
	
	create();
	raise();
}

void igdeNativeFoxWindow::DestroyNativeWidget(){
	close( false );
}



// Management
///////////////

void igdeNativeFoxWindow::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxWindow::UpdatePosition(){
	if( pOwner->GetPosition().x == getX() && pOwner->GetPosition().y == getY() ){
		return;
	}
	
	move( pOwner->GetPosition().x, pOwner->GetPosition().y );
}

void igdeNativeFoxWindow::UpdateIcon(){
	FXIcon * const icon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( icon );
	setMiniIcon( icon );
}

void igdeNativeFoxWindow::UpdateTitle(){
	setTitle( pOwner->GetTitle().GetString() );
}

void igdeNativeFoxWindow::UpdateSize(){
	resize( pOwner->GetSize().x, pOwner->GetSize().y );
}



int igdeNativeFoxWindow::WindowFlags( const igdeWindow &owner ){
	int flags = DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE | DECOR_MENU;
	
	if( owner.GetCanResize() ){
		flags |=  DECOR_RESIZE; //DECOR_STRETCHABLE, DECOR_SHRINKABLE
	}
	
	return flags;
}

int igdeNativeFoxWindow::WindowPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingLeft, 0 );
}

int igdeNativeFoxWindow::WindowPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingRight, 0 );
}

int igdeNativeFoxWindow::WindowPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingTop, 0 );
}

int igdeNativeFoxWindow::WindowPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::windowPaddingBottom, 0 );
}



// Events
///////////

long igdeNativeFoxWindow::onConfigure( FXObject *sender, FXSelector selector, void *data ){
	const int result = FXTopWindow::onConfigure( sender, selector, data );
	pOwner->SetPosition( decPoint( getX(), getY() ) );
	pOwner->SetSize( decPoint( getWidth(), getHeight() ) );
	pOwner->OnResize();
	return result;
}

long igdeNativeFoxWindow::onClose( FXObject*, FXSelector, void* ){
	return pOwner->CloseWindow() ? 0 : 1;
}

long igdeNativeFoxWindow::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}
