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

#include "igdeMenuBar.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeMenuBar : public FXMenuBar{
	FXDECLARE( cNativeIgdeMenuBar )
	
protected:
	cNativeIgdeMenuBar();
	
private:
	igdeMenuBar *pOwner;
	
public:
	cNativeIgdeMenuBar( igdeMenuBar &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeIgdeMenuBar();
	
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( cNativeIgdeMenuBar ) cNativeIgdeMenuBarMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, cNativeIgdeMenuBar::onChildLayoutFlags )
};

FXIMPLEMENT( cNativeIgdeMenuBar, FXMenuBar,
	cNativeIgdeMenuBarMap, ARRAYNUMBER( cNativeIgdeMenuBarMap ) )

cNativeIgdeMenuBar::cNativeIgdeMenuBar(){ }

cNativeIgdeMenuBar::cNativeIgdeMenuBar( igdeMenuBar &owner, FXComposite *parent, int layoutFlags ) :
FXMenuBar( parent, layoutFlags ),
pOwner( &owner ){
}

cNativeIgdeMenuBar::~cNativeIgdeMenuBar(){
}

long cNativeIgdeMenuBar::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}



// Class igdeMenuBar
//////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuBar::igdeMenuBar( igdeEnvironment &environment ) :
igdeContainer( environment ){
}

igdeMenuBar::~igdeMenuBar(){
}



// Management
///////////////

void igdeMenuBar::CreateNativeWidget(){
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
	cNativeIgdeMenuBar *foxWidget;
	
	foxWidget = new cNativeIgdeMenuBar( *this, foxParent, layoutFlags );
	
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeMenuBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeMenuBar* )GetNativeWidget();
	DropNativeWidget();
}
