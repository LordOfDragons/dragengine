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

#include "native/toolkit.h"
#include "igdeToolBar.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

// NOTE this is done complicated here because of a peculiarity of the FOX toolkit. the toolbar
//      can be floating and docket. the floating window has to be delete always since we are
//      responsible for the reference. the toolbar we have to delete also *BUT ONLY* if not
//      parented to the floating window. this forces us to do some nasty book-keeping to
//      avoid crashes and other strange problems.
//      
//      a solution jumping quick to mind is the floating window is declared the native widget
//      while the toolbar is declared the native container. this is not correct since the
//      toolbar widget should be the toolbar and not an imaginary window. this causes segfaults
//      while deleting since IGDE UI expects the native widget to be parented to the container
//      not an arbitrary other widget.
//      
//      so no matter what the FXToolBar has to be the native widget. so the question is how
//      to solve the mess with the floating window. the biggest issue here is deleting the
//      toolbar widget and in the destructor being required to delete also the shell window.
//      
//      the solution used is to move the deletion into a method of the native class. this
//      method has to delete the floating toolbar (which has to be done always) and if the
//      toolbar itself has to be deleted too.

class cNativeIgdeToolBar : public FXToolBar{
	FXDECLARE( cNativeIgdeToolBar )
	
protected:
	cNativeIgdeToolBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXToolBar::ID_LAST,
	};
	
private:
	igdeToolBar *pOwner;
	FXToolBarShell *pWetDock;
	
public:
	cNativeIgdeToolBar( igdeToolBar &owner, FXComposite *parent, FXComposite *windowParent,
		int layoutFlags );
	virtual ~cNativeIgdeToolBar();
	
	virtual void create();
	
	virtual void SafeDestroy();
};


FXDEFMAP( cNativeIgdeToolBar ) cNativeIgdeToolBarMap[] = { };


FXIMPLEMENT( cNativeIgdeToolBar, FXToolBar, cNativeIgdeToolBarMap, ARRAYNUMBER( cNativeIgdeToolBarMap ) )

cNativeIgdeToolBar::cNativeIgdeToolBar(){ }

cNativeIgdeToolBar::cNativeIgdeToolBar( igdeToolBar &owner, FXComposite *parent,
	FXComposite *windowParent, int layoutFlags ) :
FXToolBar( parent, new FXToolBarShell( windowParent, FRAME_RAISED ),
	FRAME_RAISED | /* LAYOUT_DOCK_NEXT | */ layoutFlags ),
pOwner( &owner ),
pWetDock( ( FXToolBarShell* )getWetDock() )
{
	new FXToolBarGrip( this, this, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE );
}

cNativeIgdeToolBar::~cNativeIgdeToolBar(){
}


void cNativeIgdeToolBar::create(){
	const bool createOurself = getParent() != pWetDock;
	
	pWetDock->create();
	
	if( createOurself ){
		FXToolBar::create();
	}
}

void cNativeIgdeToolBar::SafeDestroy(){
	const bool deleteOurself = getParent() != pWetDock;
	
	delete pWetDock;
	
	if( deleteOurself ){
		delete this;
	}
}



// Class igdeToolBar
//////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBar::igdeToolBar( igdeEnvironment &environment ) :
igdeContainer( environment )
{
}

igdeToolBar::~igdeToolBar(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeToolBar::CreateNativeWidget(){
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
	cNativeIgdeToolBar * const foxWidget = new cNativeIgdeToolBar(
		*this, foxParent, foxParent, layoutFlags );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeToolBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( cNativeIgdeToolBar* )GetNativeWidget() )->SafeDestroy();
	DropNativeWidget();
}
