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

#include "igdeToolBarDock.h"
#include "native/toolkit.h"
#include "igdeWidget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeToolBarDock : public FXDockSite{
	FXDECLARE( cNativeIgdeToolBarDock )
	
protected:
	cNativeIgdeToolBarDock();
	
private:
	igdeToolBarDock *pOwner;
	
public:
	cNativeIgdeToolBarDock( igdeToolBarDock &owner, FXComposite *parent );
	virtual ~cNativeIgdeToolBarDock();
	
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	
	static int LayoutFlags( const igdeToolBarDock &owner );
};


FXDEFMAP( cNativeIgdeToolBarDock ) cNativeIgdeToolBarDockMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeIgdeToolBarDock::onChildLayoutFlags )
};

FXIMPLEMENT( cNativeIgdeToolBarDock, FXDockSite,
	cNativeIgdeToolBarDockMap, ARRAYNUMBER( cNativeIgdeToolBarDockMap ) )

cNativeIgdeToolBarDock::cNativeIgdeToolBarDock(){ }

cNativeIgdeToolBarDock::cNativeIgdeToolBarDock( igdeToolBarDock &owner, FXComposite *parent ) :
FXDockSite( parent, LayoutFlags( owner ) ),
pOwner( &owner ){
}

cNativeIgdeToolBarDock::~cNativeIgdeToolBarDock(){
}

long cNativeIgdeToolBarDock::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	
	switch( pOwner->GetSide() ){
	case igdeToolBarDock::esTop:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
		
	case igdeToolBarDock::esLeft:
		clflags.flags = LAYOUT_SIDE_LEFT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esRight:
		clflags.flags = LAYOUT_SIDE_RIGHT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esBottom:
		clflags.flags = LAYOUT_SIDE_BOTTOM; // | LAYOUT_FILL_X;
		break;
		
	default:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
	}
	
// 	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}

int cNativeIgdeToolBarDock::LayoutFlags( const igdeToolBarDock &owner ){
	switch( owner.GetSide() ){
	case igdeToolBarDock::esTop:
		return LAYOUT_SIDE_TOP | LAYOUT_FILL_X;
		
	case igdeToolBarDock::esLeft:
		return LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esRight:
		return LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esBottom:
		return LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X;
		
	default:
		return 0;
	}
}



// Class igdeToolBarDock
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBarDock::igdeToolBarDock( igdeEnvironment &environment, eSide side ) :
igdeContainer( environment ),
pSide( side ){
}

igdeToolBarDock::~igdeToolBarDock(){
}



// Management
///////////////

void igdeToolBarDock::CreateNativeWidget(){
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
	
	cNativeIgdeToolBarDock * const foxWidget = new cNativeIgdeToolBarDock( *this, foxParent );
	
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeToolBarDock::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeToolBarDock* )GetNativeWidget();
	DropNativeWidget();
}
