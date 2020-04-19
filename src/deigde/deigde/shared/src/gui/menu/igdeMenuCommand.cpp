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

#include "igdeMenuCommand.h"
#include "../igdeContainer.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeWidgetReference.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeMenuCommand : public FXMenuCommand{
	FXDECLARE( cNativeIgdeMenuCommand )
	
protected:
	cNativeIgdeMenuCommand();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCommand::ID_LAST,
	};
	
private:
	igdeMenuCommand *pOwner;
	
public:
	cNativeIgdeMenuCommand( igdeMenuCommand &owner, FXComposite *parent );
	virtual ~cNativeIgdeMenuCommand();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onUpdate( FXObject *sender, FXSelector selector, void *data );
	
	static FXString BuildConstrText( igdeMenuCommand &owner );
};


FXDEFMAP( cNativeIgdeMenuCommand ) cNativeIgdeMenuCommandMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeMenuCommand::ID_SELF, cNativeIgdeMenuCommand::onCommand ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeMenuCommand::ID_SELF, cNativeIgdeMenuCommand::onUpdate )
};


FXIMPLEMENT( cNativeIgdeMenuCommand, FXMenuCommand, cNativeIgdeMenuCommandMap, ARRAYNUMBER( cNativeIgdeMenuCommandMap ) )

cNativeIgdeMenuCommand::cNativeIgdeMenuCommand(){ }

cNativeIgdeMenuCommand::cNativeIgdeMenuCommand( igdeMenuCommand &owner, FXComposite *parent ) :
FXMenuCommand( parent, BuildConstrText( owner ), owner.GetIcon()
	? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL, this, ID_SELF ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
}

cNativeIgdeMenuCommand::~cNativeIgdeMenuCommand(){
}


long cNativeIgdeMenuCommand::onCommand( FXObject *sender, FXSelector selector, void *data ){
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

long cNativeIgdeMenuCommand::onUpdate( FXObject *sender, FXSelector selector, void *data ){
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

FXString cNativeIgdeMenuCommand::BuildConstrText( igdeMenuCommand &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
}



// Class igdeMenuCommand
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCommand::igdeMenuCommand( igdeEnvironment &environment ) :
igdeWidget( environment ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ){
}

igdeMenuCommand::igdeMenuCommand( igdeEnvironment &environment, igdeAction *action ) :
igdeWidget( environment ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true )
{
	SetAction( action );
}

igdeMenuCommand::~igdeMenuCommand(){
	igdeMenuCommand::DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeMenuCommand::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeMenuCommand::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeMenuCommand::SetHotKey( const igdeHotKey &hotKey ){
	if( pHotKey == hotKey ){
		return;
	}
	
	pHotKey = hotKey;
	OnHotKeyChanged();
}

void igdeMenuCommand::SetMnemonic( deInputEvent::eKeyCodes mnemonic ){
	if( pMnemonic == mnemonic ){
		return;
	}
	
	pMnemonic = mnemonic;
	OnMnemonicChanged();
}

void igdeMenuCommand::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeMenuCommand::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeMenuCommand::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}


void igdeMenuCommand::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeMenuCommand::OnParameterChanged( igdeAction *action ){
	SetText( action->GetText() );
	SetDescription( action->GetDescription() );
	SetHotKey( action->GetHotKey() );
	SetMnemonic( action->GetMnemonic() );
	SetIcon( action->GetIcon() );
	SetEnabled( action->GetEnabled() );
}

void igdeMenuCommand::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeMenuCommand::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



void igdeMenuCommand::CreateNativeWidget(){
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
	
	cNativeIgdeMenuCommand * const foxWidget = new cNativeIgdeMenuCommand( *this, foxParent );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeMenuCommand::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeMenuCommand* )GetNativeWidget();
	DropNativeWidget();
}



void igdeMenuCommand::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXMenuCommand* )GetNativeWidget() )->setText( pText.GetString() );
}

void igdeMenuCommand::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXMenuCommand &command = *( ( FXMenuCommand* )GetNativeWidget() );
	command.setTipText( pText.GetString() );
	command.setHelpText( pText.GetString() );
}

void igdeMenuCommand::OnHotKeyChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXMenuCommand* )GetNativeWidget() )->setAccelText( igdeUIFoxHelper::AccelString( pHotKey ), true );
}

void igdeMenuCommand::OnMnemonicChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	//( ( FXMenuCommand* )GetNativeWidget() )->setKey( igdeUIFoxHelper::MnemonicKey( pMnemonic ) );
}

void igdeMenuCommand::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXIcon *icon = NULL;
	if( pIcon ){
		icon = ( FXIcon* )pIcon->GetNativeIcon();
	}
	
	( ( FXMenuCommand* )GetNativeWidget() )->setIcon( icon );
}

void igdeMenuCommand::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( pEnabled ){
		( ( FXMenuCommand* )GetNativeWidget() )->enable();
		
	}else{
		( ( FXMenuCommand* )GetNativeWidget() )->disable();
	}
}
