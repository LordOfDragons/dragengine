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

#include "igdeContainerFlow.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////

namespace{

class cNativeWidgetY : public FXVerticalFrame{
	FXDECLARE( cNativeWidgetY )
	
protected:
	cNativeWidgetY();
	
private:
	igdeContainerFlow *pOwner;
	FXVerticalFrame *pContainer;
	
public:
	cNativeWidgetY( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidgetY();
	
	inline FXVerticalFrame *GetContainer() const{ return pContainer; }
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( cNativeWidgetY ) cNativeWidgetYMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, cNativeWidgetY::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidgetY::onChildLayoutFlags )
};

FXIMPLEMENT( cNativeWidgetY, FXVerticalFrame,
	cNativeWidgetYMap, ARRAYNUMBER( cNativeWidgetYMap ) )

cNativeWidgetY::cNativeWidgetY(){ }

cNativeWidgetY::cNativeWidgetY( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags ) :
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

cNativeWidgetY::~cNativeWidgetY(){
}

long cNativeWidgetY::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long cNativeWidgetY::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
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



class cNativeWidgetX : public FXHorizontalFrame{
	FXDECLARE( cNativeWidgetX )
	
protected:
	cNativeWidgetX();
	
private:
	igdeContainerFlow *pOwner;
	FXHorizontalFrame *pContainer;
	
public:
	cNativeWidgetX( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidgetX();
	
	inline FXHorizontalFrame *GetContainer() const{ return pContainer; }
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( cNativeWidgetX ) cNativeWidgetXMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, cNativeWidgetX::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidgetX::onChildLayoutFlags ),
};

FXIMPLEMENT( cNativeWidgetX, FXHorizontalFrame,
	cNativeWidgetXMap, ARRAYNUMBER( cNativeWidgetXMap ) )

cNativeWidgetX::cNativeWidgetX(){ }

cNativeWidgetX::cNativeWidgetX( igdeContainerFlow &owner, FXComposite *parent, int layoutFlags ) :
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

cNativeWidgetX::~cNativeWidgetX(){
}

long cNativeWidgetX::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long cNativeWidgetX::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
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



// Class igdeContainerFlow
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerFlow::igdeContainerFlow( igdeEnvironment &environment, eAxis axis,
	eStretching stretching, int spacing ) :
igdeContainer( environment ),
pAxis( axis ),
pStretching( stretching ),
pSpacing( spacing )
{
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerFlow::~igdeContainerFlow(){
}



// Management
///////////////

void igdeContainerFlow::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( GetNativeContainer() && pStretching == esLast && GetChildCount() > 0 ){
		igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( GetChildCount() - 1 ) );
		( ( FXPacker* )GetNativeContainer() )->recalc();
	}
}



void igdeContainerFlow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeContainer * const parent = GetParent();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )parent->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	FXPacker *native;
	
	switch( pAxis ){
	case eaX:
	case eaXReverse:
	case eaXCenter:
		native = new cNativeWidgetX( *this, nativeParent, layoutFlags );
		break;
		
	case eaY:
	case eaYReverse:
	case eaYCenter:
		native = new cNativeWidgetY( *this, nativeParent, layoutFlags );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	SetNativeWidget( native );
	if( nativeParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerFlow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXPacker * const native = ( FXPacker* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}

void *igdeContainerFlow::GetNativeContainer() const{
	if( ! GetNativeWidget() ){
		return NULL;
	}
	
	switch( pAxis ){
	case eaX:
	case eaXReverse:
	case eaXCenter:
		return ( ( cNativeWidgetX* )GetNativeWidget() )->GetContainer();
		
	case eaY:
	case eaYReverse:
	case eaYCenter:
		return ( ( cNativeWidgetY* )GetNativeWidget() )->GetContainer();
		
	default:
		return NULL;
	}
}
