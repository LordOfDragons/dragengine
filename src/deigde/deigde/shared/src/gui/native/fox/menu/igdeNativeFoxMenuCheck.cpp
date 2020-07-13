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

#include "igdeNativeFoxMenuCheck.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuCheck.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuCheck ) igdeNativeFoxMenuCheckMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCheck::ID_SELF, igdeNativeFoxMenuCheck::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCheck::ID_SELF, igdeNativeFoxMenuCheck::updateMenuAction )
};



// Class igdeNativeFoxMenuCheck
/////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuCheck, FXMenuCheck,
	igdeNativeFoxMenuCheckMap, ARRAYNUMBER( igdeNativeFoxMenuCheckMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuCheck::igdeNativeFoxMenuCheck(){ }

igdeNativeFoxMenuCheck::igdeNativeFoxMenuCheck( igdeMenuCheck &owner, FXComposite *parent ) :
FXMenuCheck( parent, BuildConstrText( owner ), this, ID_SELF ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setCheck( owner.GetChecked() );
}

igdeNativeFoxMenuCheck::~igdeNativeFoxMenuCheck(){
}

igdeNativeFoxMenuCheck *igdeNativeFoxMenuCheck::CreateNativeWidget( igdeMenuCheck &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuCheck( owner, parent );
}

void igdeNativeFoxMenuCheck::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxMenuCheck::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuCheck::UpdateChecked(){
	setCheck( pOwner->GetChecked() );
}


FXString igdeNativeFoxMenuCheck::BuildConstrText( igdeMenuCheck &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuCheck::onMenuAction( FXObject *sender, FXSelector selector, void *data ){
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

long igdeNativeFoxMenuCheck::updateMenuAction( FXObject *sender, FXSelector selector, void *data ){
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
