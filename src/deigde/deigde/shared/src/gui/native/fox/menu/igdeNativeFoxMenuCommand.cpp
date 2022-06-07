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

#include "igdeNativeFoxMenuCommand.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuCommand.h"
#include "../../../menu/igdeMenuCommand.h"
#include "../../../resources/igdeIcon.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuCommand ) igdeNativeFoxMenuCommandMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCommand::ID_SELF, igdeNativeFoxMenuCommand::onCommand ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCommand::ID_SELF, igdeNativeFoxMenuCommand::onUpdate )
};



// Class igdeNativeFoxMenuCommand
///////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuCommand, FXMenuCommand,
	igdeNativeFoxMenuCommandMap, ARRAYNUMBER( igdeNativeFoxMenuCommandMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuCommand::igdeNativeFoxMenuCommand(){ }

igdeNativeFoxMenuCommand::igdeNativeFoxMenuCommand( igdeMenuCommand &powner, FXComposite *pparent ) :
FXMenuCommand( pparent, BuildConstrText( powner ), powner.GetIcon()
	? ( FXIcon* ) powner.GetIcon()->GetNativeIcon() : NULL, this, ID_SELF ),
pOwner( &powner )
{
	if( ! powner.GetEnabled() ){
		disable();
	}
}

igdeNativeFoxMenuCommand::~igdeNativeFoxMenuCommand(){
}

igdeNativeFoxMenuCommand *igdeNativeFoxMenuCommand::CreateNativeWidget( igdeMenuCommand &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuCommand( powner, pparent );
}

void igdeNativeFoxMenuCommand::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuCommand::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuCommand::UpdateText(){
	setText( pOwner->GetText().GetString() );
}

void igdeNativeFoxMenuCommand::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}

void igdeNativeFoxMenuCommand::UpdateHotKey(){
	setAccelText( igdeUIFoxHelper::AccelString( pOwner->GetHotKey() ), true );
}

/*
UpdateMnemonic:
	//( ( FXMenuCommand* )GetNativeWidget() )->setKey( igdeUIFoxHelper::MnemonicKey( pMnemonic ) );
*/

void igdeNativeFoxMenuCommand::UpdateIcon(){
	FXIcon *iicon = NULL;
	if( pOwner->GetIcon() ){
		iicon = ( FXIcon* )pOwner->GetIcon()->GetNativeIcon();
	}
	
	setIcon( iicon );
}

void igdeNativeFoxMenuCommand::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}


FXString igdeNativeFoxMenuCommand::BuildConstrText( igdeMenuCommand &powner ){
	return igdeUIFoxHelper::MnemonizeString( powner.GetText(), powner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( powner.GetHotKey() )
		+ "\t" + powner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuCommand::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxMenuCommand::onUpdate( FXObject*, FXSelector, void* ){
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
