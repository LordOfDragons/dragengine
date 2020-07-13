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

#include "igdeNativeFoxContainerBox.h"
#include "../../../layout/igdeContainerBox.h"

#include <dragengine/common/exceptions.h>


namespace{

class igdeNativeFoxContainerBoxY : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxContainerBoxY )
	
protected:
	igdeNativeFoxContainerBoxY();
	
private:
	igdeContainerBox *pOwner;
	
public:
	igdeNativeFoxContainerBoxY( igdeContainerBox &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerBoxY();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerBoxY ) igdeNativeFoxContainerBoxYMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxY::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxY::onChildLayoutFlags )
};

FXIMPLEMENT( igdeNativeFoxContainerBoxY, FXVerticalFrame,
	igdeNativeFoxContainerBoxYMap, ARRAYNUMBER( igdeNativeFoxContainerBoxYMap ) )

igdeNativeFoxContainerBoxY::igdeNativeFoxContainerBoxY(){ }

igdeNativeFoxContainerBoxY::igdeNativeFoxContainerBoxY( igdeContainerBox &owner, FXComposite *parent, int layoutFlags ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

igdeNativeFoxContainerBoxY::~igdeNativeFoxContainerBoxY(){
}

long igdeNativeFoxContainerBoxY::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxY::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}



class igdeNativeFoxContainerBoxX : public FXHorizontalFrame{
	FXDECLARE( igdeNativeFoxContainerBoxX )
	
protected:
	igdeNativeFoxContainerBoxX();
	
private:
	igdeContainerBox *pOwner;
	
public:
	igdeNativeFoxContainerBoxX( igdeContainerBox &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerBoxX();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerBoxX ) igdeNativeFoxContainerBoxXMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxX::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxX::onChildLayoutFlags ),
};

FXIMPLEMENT( igdeNativeFoxContainerBoxX, FXHorizontalFrame,
	igdeNativeFoxContainerBoxXMap, ARRAYNUMBER( igdeNativeFoxContainerBoxXMap ) )

igdeNativeFoxContainerBoxX::igdeNativeFoxContainerBoxX(){ }

igdeNativeFoxContainerBoxX::igdeNativeFoxContainerBoxX( igdeContainerBox &owner,
	FXComposite *parent, int layoutFlags ) :
FXHorizontalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

igdeNativeFoxContainerBoxX::~igdeNativeFoxContainerBoxX(){
}

long igdeNativeFoxContainerBoxX::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxX::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

}



// Class igdeNativeFoxContainerBox
////////////////////////////////////

void *igdeNativeFoxContainerBox::CreateNativeWidget( igdeContainerBox &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &owner );
	
	if( owner.GetAxis() == igdeContainerBox::eaX ){
		return new igdeNativeFoxContainerBoxX( owner, parent, layoutFlags );
		
	}else{
		return new igdeNativeFoxContainerBoxY( owner, parent, layoutFlags );
	}
}

void igdeNativeFoxContainerBox::PostCreateNativeWidget( igdeContainerBox &owner, void *native ){
	FXComposite &parent = *( ( FXComposite* )owner.GetParent()->GetNativeContainer() );
	if( parent.id() ){
		( ( FXPacker* )native )->create();
	}
}

void igdeNativeFoxContainerBox::DestroyNativeWidget( igdeContainerBox &owner, void *native ){
	delete ( FXPacker* )native;
}
