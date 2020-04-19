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

#include "igdeContainerScroll.h"
#include "../native/toolkit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////

namespace {

class cNativeWidget : public FXScrollWindow{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
private:
	igdeContainerScroll *pOwner;
	
public:
	cNativeWidget( igdeContainerScroll &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeWidget();
	
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, cNativeWidget::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeWidget::onChildLayoutFlags )
};

FXIMPLEMENT( cNativeWidget, FXScrollWindow, cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget( igdeContainerScroll &owner,
	FXComposite *parent, int layoutFlags ) :
FXScrollWindow( parent, layoutFlags, 0, 0, 0, 0 ),
pOwner( &owner ){
}

cNativeWidget::~cNativeWidget(){
}

long cNativeWidget::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long cNativeWidget::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_SIDE_TOP;
	
	//if( ! pOwner->GetCanScrollX() ){
		clflags.flags |= LAYOUT_FILL_X;
	//}
	//if( ! pOwner->GetCanScrollY() ){
		clflags.flags |= LAYOUT_FILL_Y;
	//}
	
	return 1;
}

}



// Class igdeContainerScroll
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerScroll::igdeContainerScroll( igdeEnvironment &environment,
	bool canScrollX, bool canScrollY ) :
igdeContainer( environment ),
pCanScrollX( canScrollX ),
pCanScrollY( canScrollY ){
}

igdeContainerScroll::~igdeContainerScroll(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeContainerScroll::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}



void igdeContainerScroll::CreateNativeWidget(){
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
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this ) | SCROLLERS_NORMAL | SCROLLERS_TRACK;
	if( ! pCanScrollX ){
		layoutFlags |= HSCROLLING_OFF;
	}
	if( ! pCanScrollY ){
		layoutFlags |= VSCROLLING_OFF;
	}
	
	cNativeWidget * const container = new cNativeWidget( *this, foxParent, layoutFlags );
	
	SetNativeWidget( container );
	if( foxParent->id() ){
		container->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerScroll::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget * const native = ( cNativeWidget* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}
