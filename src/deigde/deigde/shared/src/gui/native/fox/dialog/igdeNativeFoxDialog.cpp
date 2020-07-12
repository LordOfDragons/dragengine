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

#include "igdeNativeFoxDialog.h"
#include "../foxIcons.h"
#include "../../../dialog/igdeDialog.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../resources/igdeIcon.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxDialog ) igdeNativeFoxDialogMap[] = {
	FXMAPFUNC( SEL_CLOSE, 0, igdeNativeFoxDialog::onClose ),
	FXMAPFUNC( SEL_COMMAND, FXDialogBox::ID_ACCEPT, igdeNativeFoxDialog::onCmdAccept ),
	FXMAPFUNC( SEL_COMMAND, FXDialogBox::ID_CANCEL, igdeNativeFoxDialog::onCmdCancel ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxDialog::onChildLayoutFlags ),
	FXMAPFUNC( SEL_IGDE_FRAME_UPDATE, 0, igdeNativeFoxDialog::onFrameUpdate )
};



// Class igdeNativeFoxDialog
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxDialog, FXDialogBox, igdeNativeFoxDialogMap, ARRAYNUMBER( igdeNativeFoxDialogMap ) )

// Constructor, destructor
////////////////////////////


igdeNativeFoxDialog::igdeNativeFoxDialog(){ }

igdeNativeFoxDialog::igdeNativeFoxDialog( igdeDialog &owner, FXWindow *parent,
const igdeGuiTheme &guitheme ) :
FXDialogBox( parent, owner.GetTitle().GetString(), DialogFlags( owner ), 0, 0, 0, 0,
	DialogPadLeft( guitheme ), DialogPadRight( guitheme ),
	DialogPadTop( guitheme ), DialogPadBottom( guitheme ),
	0, 0 ),
pOwner( &owner )
{
	UpdateIcon();
}

igdeNativeFoxDialog::~igdeNativeFoxDialog(){
}

igdeNativeFoxDialog *igdeNativeFoxDialog::CreateNativeWidget( igdeDialog &owner, igdeWidget *ownerOwner ){
	if( owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow *nativeOwner = NULL;
	if( ownerOwner ){
		nativeOwner = ( FXWindow* )ownerOwner->GetNativeWidget();
	}
	
	return new igdeNativeFoxDialog( owner, nativeOwner, *owner.GetGuiTheme() );
}

void igdeNativeFoxDialog::PostCreateNativeWidget(){
	create();
}

void igdeNativeFoxDialog::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxDialog::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxDialog::UpdatePosition(){
	if( pOwner->GetPosition().x == getX() && pOwner->GetPosition().y == getY() ){
		return;
	}
	
	move( pOwner->GetPosition().x, pOwner->GetPosition().y );
}

void igdeNativeFoxDialog::UpdateIcon(){
	FXIcon * const icon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( icon );
	setMiniIcon( icon );
}

void igdeNativeFoxDialog::UpdateTitle(){
	setTitle( pOwner->GetTitle().GetString() );
}

void igdeNativeFoxDialog::UpdateSize(){
	resize( pOwner->GetSize().x, pOwner->GetSize().y );
}

void igdeNativeFoxDialog::ShowDialog(){
	const decPoint &initialSize = pOwner->GetInitialSize();
	if( initialSize != decPoint() ){
		resize( decMath::max( getWidth(), initialSize.x ), decMath::max( getHeight(), initialSize.y ) );
	}
	show( PLACEMENT_OWNER );
}

void igdeNativeFoxDialog::CloseDialog( bool accepted ){
	if( accepted ){
		tryHandle( this, FXSEL( SEL_COMMAND, FXDialogBox::ID_ACCEPT ), NULL );
		
	}else{
		tryHandle( this, FXSEL( SEL_COMMAND, FXDialogBox::ID_CANCEL ), NULL );
	}
}



int igdeNativeFoxDialog::DialogFlags( const igdeDialog &owner ){
	int flags = DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE | DECOR_MENU;
	
	if( owner.GetCanResize() ){
		flags |=  DECOR_RESIZE; //DECOR_STRETCHABLE, DECOR_SHRINKABLE
	}
	
	return flags;
}

int igdeNativeFoxDialog::DialogPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingBottom, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadContent( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingContent, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadButtons( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingButtons, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxDialog::onClose( FXObject*, FXSelector, void* ){
	return pOwner->Cancel() ? 0 : 1;
}

long igdeNativeFoxDialog::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

long igdeNativeFoxDialog::onFrameUpdate( FXObject*, FXSelector, void* ){
	pOwner->OnFrameUpdate();
	return 1;
}
