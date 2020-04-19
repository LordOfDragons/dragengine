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

#include "igdeContainerBoxAlternate.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

namespace{

class cNativeWidgetY : public FXVerticalFrame{
	FXDECLARE( cNativeWidgetY )
	
protected:
	cNativeWidgetY();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	cNativeWidgetY( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidgetY();
	
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

cNativeWidgetY::cNativeWidgetY( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ){
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
	
	const bool oddIndex = index % 2 == 1;
	if( pOwner->GetStretchOdd() == oddIndex ){
		clflags.flags |= LAYOUT_FILL_Y;
	}
	
	return 1;
}



class cNativeWidgetX : public FXHorizontalFrame{
	FXDECLARE( cNativeWidgetX )
	
protected:
	cNativeWidgetX();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	cNativeWidgetX( igdeContainerBoxAlternate &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidgetX();
	
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

cNativeWidgetX::cNativeWidgetX( igdeContainerBoxAlternate &owner,
	FXComposite *parent, int layoutFlags ) :
FXHorizontalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

cNativeWidgetX::~cNativeWidgetX(){
}

long cNativeWidgetX::onResize( FXObject *sender, FXSelector selector, void *data ){
	pOwner->OnResize();
	return 1;
}

long cNativeWidgetX::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
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



// Class igdeContainerBoxAlternate
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBoxAlternate::igdeContainerBoxAlternate( igdeEnvironment &environment,
	eAxis axis, bool stretchOdd, int spacing ) :
igdeContainerBox( environment, axis, spacing ),
pStretchOdd( stretchOdd ){
}

igdeContainerBoxAlternate::~igdeContainerBoxAlternate(){
}



// Management
///////////////

void igdeContainerBoxAlternate::CreateNativeWidget(){
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
	FXPacker *foxWidget;
	
	if( GetAxis() == eaX ){
		foxWidget = new cNativeWidgetX( *this, foxParent, layoutFlags );
		
	}else{
		foxWidget = new cNativeWidgetY( *this, foxParent, layoutFlags );
	}
	
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerBoxAlternate::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXPacker * const native = ( FXPacker* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}
