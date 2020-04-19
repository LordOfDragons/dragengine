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

#include "igdeContainerBox.h"
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
	igdeContainerBox *pOwner;
	
public:
	cNativeWidgetY( igdeContainerBox &owner, FXComposite *parent, int layoutFlags );
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

cNativeWidgetY::cNativeWidgetY( igdeContainerBox &owner, FXComposite *parent, int layoutFlags ) :
FXVerticalFrame( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetSpacing(), owner.GetSpacing() ),
pOwner( &owner ){
}

cNativeWidgetY::~cNativeWidgetY(){
}

long cNativeWidgetY::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long cNativeWidgetY::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}



class cNativeWidgetX : public FXHorizontalFrame{
	FXDECLARE( cNativeWidgetX )
	
protected:
	cNativeWidgetX();
	
private:
	igdeContainerBox *pOwner;
	
public:
	cNativeWidgetX( igdeContainerBox &owner, FXComposite *parent, int layoutFlags );
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

cNativeWidgetX::cNativeWidgetX( igdeContainerBox &owner,
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
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

}



// Class igdeContainerBox
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBox::igdeContainerBox( igdeEnvironment &environment, eAxis axis, int spacing ) :
igdeContainer( environment ),
pAxis( axis ),
pSpacing( spacing )
{
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerBox::~igdeContainerBox(){
}



// Management
///////////////

void igdeContainerBox::CreateNativeWidget(){
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
	
	if( pAxis == eaX ){
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

void igdeContainerBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXPacker * const native = ( FXPacker* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}
