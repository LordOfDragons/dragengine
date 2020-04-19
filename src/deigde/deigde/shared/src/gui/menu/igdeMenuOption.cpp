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

#include "igdeMenuOption.h"
#include "igdeMenuCascade.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeMenuOption : public FXMenuRadio{
	FXDECLARE( cNativeIgdeMenuOption )
	
protected:
	cNativeIgdeMenuOption();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuRadio::ID_LAST,
	};
	
private:
	igdeMenuOption *pOwner;
	
public:
	cNativeIgdeMenuOption( igdeMenuOption &owner, FXComposite *parent );
	virtual ~cNativeIgdeMenuOption();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
	
	static FXString BuildConstrText( igdeMenuCommand &owner );
};


FXDEFMAP( cNativeIgdeMenuOption ) cNativeIgdeMenuOptionMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeMenuOption::ID_SELF, cNativeIgdeMenuOption::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeMenuOption::ID_SELF, cNativeIgdeMenuOption::updateMenuAction )
};


FXIMPLEMENT( cNativeIgdeMenuOption, FXMenuRadio, cNativeIgdeMenuOptionMap, ARRAYNUMBER( cNativeIgdeMenuOptionMap ) )

cNativeIgdeMenuOption::cNativeIgdeMenuOption(){ }

cNativeIgdeMenuOption::cNativeIgdeMenuOption( igdeMenuOption &owner, FXComposite *parent ) :
FXMenuRadio( parent, BuildConstrText( owner ), this, ID_SELF ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setCheck( owner.GetSelected() );
}

cNativeIgdeMenuOption::~cNativeIgdeMenuOption(){
}

long cNativeIgdeMenuOption::onMenuAction( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	pOwner->SetSelected( true );
	// TODO set others in group unselected
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeMenuOption::updateMenuAction( FXObject *sender, FXSelector selector, void *data ){
	igdeAction * const action = pOwner->GetAction();
	if( ! action ){
		return 0;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
	}
	
	return 0;
}

FXString cNativeIgdeMenuOption::BuildConstrText( igdeMenuCommand &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
}



// Class igdeMenuOption
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuOption::igdeMenuOption( igdeEnvironment &environment ) :
igdeMenuCommand( environment ),
pSelected( false ){
}

igdeMenuOption::igdeMenuOption( igdeEnvironment &environment, igdeAction *action ) :
igdeMenuCommand( environment, action ),
pSelected( false ){
}

igdeMenuOption::~igdeMenuOption(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuOption::SetSelected( bool selected ){
	if( pSelected == selected ){
		return;
	}
	
	pSelected = selected;
	OnSelectedChanged();
}



void igdeMenuOption::OnParameterChanged( igdeAction *action ){
	igdeMenuCommand::OnParameterChanged( action );
	
	SetSelected( action->GetSelected() );
}



void igdeMenuOption::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	cNativeIgdeMenuOption * const native = new cNativeIgdeMenuOption( *this, foxParent );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeMenuOption::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeMenuOption* )GetNativeWidget();
	DropNativeWidget();
}



void igdeMenuOption::OnSelectedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXMenuRadio &native = *( ( FXMenuRadio* )GetNativeWidget() );
	native.setCheck( pSelected );
}
