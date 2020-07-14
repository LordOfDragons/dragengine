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

#include "igdeNativeFoxContainerBoxAlternate.h"
#include "../../../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/exceptions.h>


namespace{

class igdeNativeFoxContainerBoxAlternateY : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxContainerBoxAlternateY )
	
protected:
	igdeNativeFoxContainerBoxAlternateY();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	igdeNativeFoxContainerBoxAlternateY( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerBoxAlternateY();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerBoxAlternateY ) igdeNativeFoxContainerBoxAlternateYMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxAlternateY::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxAlternateY::onChildLayoutFlags )
};

FXIMPLEMENT( igdeNativeFoxContainerBoxAlternateY, FXVerticalFrame,
	igdeNativeFoxContainerBoxAlternateYMap, ARRAYNUMBER( igdeNativeFoxContainerBoxAlternateYMap ) )

igdeNativeFoxContainerBoxAlternateY::igdeNativeFoxContainerBoxAlternateY(){ }

igdeNativeFoxContainerBoxAlternateY::igdeNativeFoxContainerBoxAlternateY( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ){
}

igdeNativeFoxContainerBoxAlternateY::~igdeNativeFoxContainerBoxAlternateY(){
}

long igdeNativeFoxContainerBoxAlternateY::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxAlternateY::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	const bool oddIndex = index % 2 == 1;
	if( pOwner->GetStretchOdd() == oddIndex ){
		clflags.flags |= LAYOUT_FILL_Y;
	}
	
	return 1;
}



class igdeNativeFoxContainerBoxAlternateX : public FXHorizontalFrame{
	FXDECLARE( igdeNativeFoxContainerBoxAlternateX )
	
protected:
	igdeNativeFoxContainerBoxAlternateX();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	igdeNativeFoxContainerBoxAlternateX( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerBoxAlternateX();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerBoxAlternateX ) igdeNativeFoxContainerBoxAlternateXMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxAlternateX::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxAlternateX::onChildLayoutFlags ),
};

FXIMPLEMENT( igdeNativeFoxContainerBoxAlternateX, FXHorizontalFrame,
	igdeNativeFoxContainerBoxAlternateXMap, ARRAYNUMBER( igdeNativeFoxContainerBoxAlternateXMap ) )

igdeNativeFoxContainerBoxAlternateX::igdeNativeFoxContainerBoxAlternateX(){ }

igdeNativeFoxContainerBoxAlternateX::igdeNativeFoxContainerBoxAlternateX( igdeContainerBoxAlternate &owner,
	FXComposite *parent, int layoutFlags ) :
FXHorizontalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

igdeNativeFoxContainerBoxAlternateX::~igdeNativeFoxContainerBoxAlternateX(){
}

long igdeNativeFoxContainerBoxAlternateX::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxAlternateX::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_Y;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	const bool oddIndex = index % 2 == 1;
	if( pOwner->GetStretchOdd() == oddIndex ){
		clflags.flags |= LAYOUT_FILL_X;
	}
	
	return 1;
}

}



// Class igdeNativeFoxContainerBoxAlternate
/////////////////////////////////////////////

void *igdeNativeFoxContainerBoxAlternate::CreateNativeWidget( igdeContainerBoxAlternate &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &owner );
	
	if( owner.GetAxis() == igdeContainerBoxAlternate::eaX ){
		return new igdeNativeFoxContainerBoxAlternateX( owner, parent, layoutFlags );
		
	}else{
		return new igdeNativeFoxContainerBoxAlternateY( owner, parent, layoutFlags );
	}
}

void igdeNativeFoxContainerBoxAlternate::PostCreateNativeWidget( igdeContainerBoxAlternate &owner, void *native ){
	FXComposite &parent = *( ( FXComposite* )owner.GetParent()->GetNativeContainer() );
	if( parent.id() ){
		( ( FXPacker* )native )->create();
	}
}

void igdeNativeFoxContainerBoxAlternate::DestroyNativeWidget( igdeContainerBoxAlternate &owner, void *native ){
	delete ( FXPacker* )native;
}

#endif
