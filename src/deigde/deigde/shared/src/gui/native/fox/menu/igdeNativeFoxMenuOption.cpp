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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxMenuOption.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuOption.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuOption ) igdeNativeFoxMenuOptionMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuOption::ID_SELF, igdeNativeFoxMenuOption::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuOption::ID_SELF, igdeNativeFoxMenuOption::updateMenuAction )
};



// Class igdeNativeFoxMenuOption
/////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuOption, FXMenuRadio,
	igdeNativeFoxMenuOptionMap, ARRAYNUMBER( igdeNativeFoxMenuOptionMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuOption::igdeNativeFoxMenuOption(){ }

igdeNativeFoxMenuOption::igdeNativeFoxMenuOption( igdeMenuOption &powner, FXComposite *pparent ) :
FXMenuRadio( pparent, BuildConstrText( powner ), this, ID_SELF ),
pOwner( &powner )
{
	if( ! powner.GetEnabled() ){
		disable();
	}
	
	setCheck( powner.GetSelected() );
}

igdeNativeFoxMenuOption::~igdeNativeFoxMenuOption(){
}

igdeNativeFoxMenuOption *igdeNativeFoxMenuOption::CreateNativeWidget( igdeMenuOption &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuOption( powner, pparent );
}

void igdeNativeFoxMenuOption::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuOption::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuOption::UpdateSelected(){
	setCheck( pOwner->GetSelected() );
}


FXString igdeNativeFoxMenuOption::BuildConstrText( igdeMenuOption &powner ){
	return igdeUIFoxHelper::MnemonizeString( powner.GetText(), powner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( powner.GetHotKey() )
		+ "\t" + powner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuOption::onMenuAction( FXObject*, FXSelector, void* ){
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

long igdeNativeFoxMenuOption::updateMenuAction( FXObject*, FXSelector, void* ){
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

#endif
