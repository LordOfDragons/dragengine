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

#include "igdeNativeFoxContainerFlow.h"
#include "../../../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>


namespace{

class igdeNativeFoxContainerFlowY : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxContainerFlowY )
	
protected:
	igdeNativeFoxContainerFlowY();
	
private:
	igdeContainerFlow *pOwner;
	FXVerticalFrame *pContainer;
	
public:
	igdeNativeFoxContainerFlowY( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerFlowY();
	
	inline FXVerticalFrame *GetContainer() const{ return pContainer; }
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerFlowY ) igdeNativeFoxContainerFlowYMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerFlowY::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerFlowY::onChildLayoutFlags )
};

FXIMPLEMENT( igdeNativeFoxContainerFlowY, FXVerticalFrame,
	igdeNativeFoxContainerFlowYMap, ARRAYNUMBER( igdeNativeFoxContainerFlowYMap ) )

igdeNativeFoxContainerFlowY::igdeNativeFoxContainerFlowY(){ }

igdeNativeFoxContainerFlowY::igdeNativeFoxContainerFlowY( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ),
pContainer( this )
{
	switch( owner.GetAxis() ){
	case igdeContainerFlow::eaYReverse:
		setHSpacing( 0 );
		setVSpacing( 0 );
		new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		pContainer = new FXVerticalFrame( this, LAYOUT_FILL_X,
			0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() );
		break;
		
	case igdeContainerFlow::eaYCenter:
		setHSpacing( 0 );
		setVSpacing( 0 );
		new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		pContainer = new FXVerticalFrame( this, LAYOUT_FILL_X,
			0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() );
		new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		break;
		
	default:
		break;
	}
}

igdeNativeFoxContainerFlowY::~igdeNativeFoxContainerFlowY(){
}

long igdeNativeFoxContainerFlowY::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerFlowY::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	
	switch( pOwner->GetStretching() ){
	case igdeContainerFlow::esFirst:
		if( index == 0 ){
			clflags.flags |= LAYOUT_FILL_Y;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerFlow::esLast:
		if( index == pOwner->GetChildCount() - 1 ){
			clflags.flags |= LAYOUT_FILL_Y;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerFlow::esNone:
	default:
		break;
	}
	
	return 1;
}



class igdeNativeFoxContainerFlowX : public FXHorizontalFrame{
	FXDECLARE( igdeNativeFoxContainerFlowX )
	
protected:
	igdeNativeFoxContainerFlowX();
	
private:
	igdeContainerFlow *pOwner;
	FXHorizontalFrame *pContainer;
	
public:
	igdeNativeFoxContainerFlowX( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxContainerFlowX();
	
	inline FXHorizontalFrame *GetContainer() const{ return pContainer; }
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxContainerFlowX ) igdeNativeFoxContainerFlowXMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerFlowX::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerFlowX::onChildLayoutFlags ),
};

FXIMPLEMENT( igdeNativeFoxContainerFlowX, FXHorizontalFrame,
	igdeNativeFoxContainerFlowXMap, ARRAYNUMBER( igdeNativeFoxContainerFlowXMap ) )

igdeNativeFoxContainerFlowX::igdeNativeFoxContainerFlowX(){ }

igdeNativeFoxContainerFlowX::igdeNativeFoxContainerFlowX( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags ) :
FXHorizontalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ),
pContainer( this )
{
	switch( owner.GetAxis() ){
	case igdeContainerFlow::eaYReverse:
		setHSpacing( 0 );
		setVSpacing( 0 );
		new FXHorizontalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		pContainer = new FXHorizontalFrame( this, LAYOUT_FILL_Y,
			0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() );
		break;
		
	case igdeContainerFlow::eaYCenter:
		setHSpacing( 0 );
		setVSpacing( 0 );
		new FXHorizontalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		pContainer = new FXHorizontalFrame( this, LAYOUT_FILL_Y,
			0, 0, 0, 0, 0, 0, 0, 0, owner.GetSpacing(), owner.GetSpacing() );
		new FXHorizontalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		break;
		
	default:
		break;
	}
}

igdeNativeFoxContainerFlowX::~igdeNativeFoxContainerFlowX(){
}

long igdeNativeFoxContainerFlowX::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerFlowX::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_Y;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	
	switch( pOwner->GetStretching() ){
	case igdeContainerFlow::esFirst:
		if( index == 0 ){
			clflags.flags |= LAYOUT_FILL_X;
			clflags.canResizeHorizontal = false;
		}
		break;
		
	case igdeContainerFlow::esLast:
		if( index == pOwner->GetChildCount() - 1 ){
			clflags.flags |= LAYOUT_FILL_X;
			clflags.canResizeHorizontal = false;
		}
		break;
		
	case igdeContainerFlow::esNone:
	default:
		break;
	}
	
	return 1;
}

}



// Class igdeNativeFoxContainerFlow
////////////////////////////////////

void *igdeNativeFoxContainerFlow::CreateNativeWidget( igdeContainerFlow &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &owner );
	
	switch( owner.GetAxis() ){
	case igdeContainerFlow::eaX:
	case igdeContainerFlow::eaXReverse:
	case igdeContainerFlow::eaXCenter:
		return new igdeNativeFoxContainerFlowX( owner, parent, layoutFlags );
		
	case igdeContainerFlow::eaY:
	case igdeContainerFlow::eaYReverse:
	case igdeContainerFlow::eaYCenter:
		return new igdeNativeFoxContainerFlowY( owner, parent, layoutFlags );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void igdeNativeFoxContainerFlow::PostCreateNativeWidget( igdeContainerFlow &owner, void *native ){
	FXComposite &parent = *( ( FXComposite* )owner.GetParent()->GetNativeContainer() );
	if( parent.id() ){
		( ( FXPacker* )native )->create();
	}
}

void igdeNativeFoxContainerFlow::DestroyNativeWidget( igdeContainerFlow &owner, void *native ){
	delete ( FXPacker* )native;
}

void igdeNativeFoxContainerFlow::ChildRemoved( igdeContainerFlow &owner, void *native ){
	if( owner.GetNativeContainer()
	&& owner.GetStretching() == igdeContainerFlow::esLast
	&& owner.GetChildCount() > 0 ){
		igdeUIFoxHelper::UpdateLayoutFlags( owner.GetChildAt( owner.GetChildCount() - 1 ) );
		( ( FXPacker* )native )->recalc();
	}
}

void *igdeNativeFoxContainerFlow::GetNativeContainer( const igdeContainerFlow &owner, void *native ){
	switch( owner.GetAxis() ){
	case igdeContainerFlow::eaX:
	case igdeContainerFlow::eaXReverse:
	case igdeContainerFlow::eaXCenter:
		return ( ( const igdeNativeFoxContainerFlowX* )native )->GetContainer();
		
	case igdeContainerFlow::eaY:
	case igdeContainerFlow::eaYReverse:
	case igdeContainerFlow::eaYCenter:
		return ( ( const igdeNativeFoxContainerFlowY* )native )->GetContainer();
		
	default:
		DETHROW( deeInvalidParam );
	}
}
