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

#include "igdeMenuCheck.h"
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

class cNativeIgdeMenuCheck : public FXMenuCheck{
	FXDECLARE( cNativeIgdeMenuCheck )
	
protected:
	cNativeIgdeMenuCheck();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCheck::ID_LAST,
	};
	
private:
	igdeMenuCheck *pOwner;
	
public:
	cNativeIgdeMenuCheck( igdeMenuCheck &owner, FXComposite *parent );
	virtual ~cNativeIgdeMenuCheck();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
	
	static FXString BuildConstrText( igdeMenuCommand &owner );
};


FXDEFMAP( cNativeIgdeMenuCheck ) cNativeIgdeMenuCheckMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeMenuCheck::ID_SELF, cNativeIgdeMenuCheck::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeMenuCheck::ID_SELF, cNativeIgdeMenuCheck::updateMenuAction )
};


FXIMPLEMENT( cNativeIgdeMenuCheck, FXMenuCheck, cNativeIgdeMenuCheckMap, ARRAYNUMBER( cNativeIgdeMenuCheckMap ) )

cNativeIgdeMenuCheck::cNativeIgdeMenuCheck(){ }

cNativeIgdeMenuCheck::cNativeIgdeMenuCheck( igdeMenuCheck &owner, FXComposite *parent ) :
FXMenuCheck( parent, BuildConstrText( owner ), this, ID_SELF ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setCheck( owner.GetChecked() );
}

cNativeIgdeMenuCheck::~cNativeIgdeMenuCheck(){
}

long cNativeIgdeMenuCheck::onMenuAction( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	pOwner->SetChecked( getCheck() );
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeMenuCheck::updateMenuAction( FXObject *sender, FXSelector selector, void *data ){
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

FXString cNativeIgdeMenuCheck::BuildConstrText( igdeMenuCommand &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
}



// Class igdeMenuCheck
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCheck::igdeMenuCheck( igdeEnvironment &environment ) :
igdeMenuCommand( environment ),
pChecked( false ){
}

igdeMenuCheck::igdeMenuCheck( igdeEnvironment &environment, igdeAction *action ) :
igdeMenuCommand( environment, action ),
pChecked( false ){
}

igdeMenuCheck::~igdeMenuCheck(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuCheck::SetChecked( bool checked ){
	if( pChecked == checked ){
		return;
	}
	
	pChecked = checked;
	OnCheckedChanged();
}



void igdeMenuCheck::OnParameterChanged( igdeAction *action ){
	igdeMenuCommand::OnParameterChanged( action );
	
	SetChecked( action->GetSelected() );
}



void igdeMenuCheck::CreateNativeWidget(){
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
	
	cNativeIgdeMenuCheck * const native = new cNativeIgdeMenuCheck( *this, foxParent );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeMenuCheck::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeMenuCheck* )GetNativeWidget();
	DropNativeWidget();
}



void igdeMenuCheck::OnCheckedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXMenuCheck &native = *( ( FXMenuCheck* )GetNativeWidget() );
	native.setCheck( pChecked );
}
