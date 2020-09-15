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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeOSCheckBox.h"
#include "../../igdeCheckBox.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeBeOSCheckBox
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSCheckBox::igdeNativeBeOSCheckBox( igdeCheckBox &owner, const igdeGuiTheme &guitheme ) :
BCheckBox( CheckBoxText( owner ), new BMessage( eeClicked ) ),
pOwner( &owner ),
pFont( CheckBoxFont( owner, guitheme ) )
{
	SetTarget( this );
	
	// padding
	// CheckBoxPadLeft( guitheme ), CheckBoxPadRight( guitheme ),
	// CheckBoxPadTop( guitheme ), CheckBoxPadBottom( guitheme ) ),
	SetIcon( CheckBoxIcon( owner ) );
	SetFont( ( BFont* )pFont->GetNativeFont() );
	SetEnabled( owner.GetEnabled() );
	SetToolTip( owner.GetDescription() );
}

igdeNativeBeOSCheckBox::~igdeNativeBeOSCheckBox(){
}

igdeNativeBeOSCheckBox *igdeNativeBeOSCheckBox::CreateNativeWidget( igdeCheckBox &owner ){
	return new igdeNativeBeOSCheckBox( owner, *owner.GetGuiTheme() );
}

void igdeNativeBeOSCheckBox::PostCreateNativeWidget(){
}

void igdeNativeBeOSCheckBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

bool igdeNativeBeOSCheckBox::GetChecked() const{
	return Value() == B_CONTROL_ON;
}

void igdeNativeBeOSCheckBox::SetChecked( bool checked ){
	SetValue( checked ? B_CONTROL_ON : B_CONTROL_OFF );
}

void igdeNativeBeOSCheckBox::UpdateChecked(){
	if( pOwner->GetChecked() != GetChecked() ){
		SetChecked( pOwner->GetChecked() );
	}
}

void igdeNativeBeOSCheckBox::UpdateStyle(){
	SetLabel( CheckBoxText( *pOwner ) );
	SetIcon( CheckBoxIcon( *pOwner ) );
}

void igdeNativeBeOSCheckBox::UpdateText(){
	SetLabel( CheckBoxText( *pOwner ) );
}

void igdeNativeBeOSCheckBox::UpdateDescription(){
	SetToolTip( pOwner->GetDescription() );
}

void igdeNativeBeOSCheckBox::UpdateIcon(){
	SetIcon( CheckBoxIcon( *pOwner ) );
}

void igdeNativeBeOSCheckBox::UpdateEnabled(){
	SetEnabled( pOwner->GetEnabled() );
}



void igdeNativeBeOSCheckBox::MessageReceived( BMessage *message ){
	if( message->what == eeClicked ){
		if( ! pOwner->GetEnabled() ){
			return;
		}
		
		pOwner->SetChecked( GetChecked() );
		
		try{
			pOwner->OnAction();
			
		}catch( const deException &e ){
			pOwner->GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( pOwner, e );
		}
		
	}else{
		BCheckBox::MessageReceived( message );
	}
}

void igdeNativeBeOSCheckBox::UpdateAction(){
	igdeAction * const action = pOwner->GetAction();
	if( ! action ){
		return;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
	}
}



const char *igdeNativeBeOSCheckBox::CheckBoxText( const igdeCheckBox &owner ){
	return owner.GetText();
}

BBitmap *igdeNativeBeOSCheckBox::CheckBoxIcon( const igdeCheckBox &owner ){
	if( owner.GetIcon() ){
		return ( BBitmap* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

igdeFont *igdeNativeBeOSCheckBox::CheckBoxFont( const igdeCheckBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::checkBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeBeOSCheckBox::CheckBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingLeft, 0 );
}

int igdeNativeBeOSCheckBox::CheckBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingRight, 0 );
}

int igdeNativeBeOSCheckBox::CheckBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingTop, 0 );
}

int igdeNativeBeOSCheckBox::CheckBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingBottom, 0 );
}

#endif
