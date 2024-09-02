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

igdeNativeFoxDialog::igdeNativeFoxDialog( igdeDialog &powner, FXWindow *pparent,
const igdeGuiTheme &guitheme ) :
FXDialogBox( pparent, powner.GetTitle().GetString(), DialogFlags( powner ), 0, 0, 0, 0,
	DialogPadLeft( guitheme ), DialogPadRight( guitheme ),
	DialogPadTop( guitheme ), DialogPadBottom( guitheme ),
	0, 0 ),
pOwner( &powner )
{
	UpdateIcon();
}

igdeNativeFoxDialog::~igdeNativeFoxDialog(){
}

igdeNativeFoxDialog *igdeNativeFoxDialog::CreateNativeWidget( igdeDialog &powner, igdeWidget *ownerOwner ){
	if( powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow *nativeOwner = NULL;
	if( ownerOwner ){
		nativeOwner = ( FXWindow* )ownerOwner->GetNativeWidget();
	}
	
	return new igdeNativeFoxDialog( powner, nativeOwner, *powner.GetGuiTheme() );
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
	FXIcon * const iicon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( iicon );
	setMiniIcon( iicon );
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



int igdeNativeFoxDialog::DialogFlags( const igdeDialog &powner ){
	int fflags = DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE | DECOR_MENU;
	
	if( powner.GetCanResize() ){
		fflags |=  DECOR_RESIZE; //DECOR_STRETCHABLE, DECOR_SHRINKABLE
	}
	
	return fflags;
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

long igdeNativeFoxDialog::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

long igdeNativeFoxDialog::onFrameUpdate( FXObject*, FXSelector, void* ){
	pOwner->OnFrameUpdate();
	return 1;
}

#endif
