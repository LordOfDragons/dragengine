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

#include "igdeNativeFoxCheckBox.h"
#include "foxIcons.h"
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



// Events
///////////

FXDEFMAP( igdeNativeFoxCheckBox ) igdeNativeFoxCheckBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxCheckBox::ID_SELF, igdeNativeFoxCheckBox::onCommand ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxCheckBox::ID_SELF, igdeNativeFoxCheckBox::onUpdate )
};



// Class igdeNativeFoxCheckBox
////////////////////////////////

FXIMPLEMENT( igdeNativeFoxCheckBox, FXCheckButton, igdeNativeFoxCheckBoxMap, ARRAYNUMBER( igdeNativeFoxCheckBoxMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxCheckBox::igdeNativeFoxCheckBox(){ }

igdeNativeFoxCheckBox::igdeNativeFoxCheckBox( igdeCheckBox &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXCheckButton( pparent, CheckBoxText( powner ), this, ID_SELF, layoutFlags | CheckBoxFlags( powner ),
	0, 0, 0, 0,
	CheckBoxPadLeft( guitheme ), CheckBoxPadRight( guitheme ),
	CheckBoxPadTop( guitheme ), CheckBoxPadBottom( guitheme ) ),
pOwner( &powner ),
pFont( CheckBoxFont( powner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( ! powner.GetEnabled() ){
		disable();
	}
	setCheck( powner.GetChecked() );
	setIcon( CheckBoxIcon( powner ) );
	setTipText( powner.GetDescription().GetString() );
	setHelpText( powner.GetDescription().GetString() );
}

igdeNativeFoxCheckBox::~igdeNativeFoxCheckBox(){
}

igdeNativeFoxCheckBox *igdeNativeFoxCheckBox::CreateNativeWidget( igdeCheckBox &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxCheckBox( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxCheckBox::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxCheckBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxCheckBox::canFocus() const{
	if( pOwner->GetStyle() == igdeCheckBox::ebsToolBar ){
		return false;
		
	}else{
		return FXCheckButton::canFocus();
	}
}

void igdeNativeFoxCheckBox::UpdateChecked(){
	if( pOwner->GetChecked() != ( bool )getCheck() ){
		setCheck( pOwner->GetChecked() );
	}
}

void igdeNativeFoxCheckBox::UpdateStyle(){
	setText( igdeNativeFoxCheckBox::CheckBoxText( *pOwner ) );
	setIcon( igdeNativeFoxCheckBox::CheckBoxIcon( *pOwner ) );
	setCheckButtonStyle( igdeNativeFoxCheckBox::CheckBoxFlags( *pOwner ) );
}

void igdeNativeFoxCheckBox::UpdateText(){
	setText( igdeNativeFoxCheckBox::CheckBoxText( *pOwner ) );
}

void igdeNativeFoxCheckBox::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}

void igdeNativeFoxCheckBox::UpdateIcon(){
	setIcon( igdeNativeFoxCheckBox::CheckBoxIcon( *pOwner ) );
}

void igdeNativeFoxCheckBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}



const char *igdeNativeFoxCheckBox::CheckBoxText( const igdeCheckBox &powner ){
	return powner.GetText();
}

FXIcon *igdeNativeFoxCheckBox::CheckBoxIcon( const igdeCheckBox &powner ){
	if( powner.GetIcon() ){
		return ( FXIcon* ) powner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int igdeNativeFoxCheckBox::CheckBoxFlags( const igdeCheckBox &powner ){
	if( powner.GetStyle() == igdeCheckBox::ebsToolBar ){
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT | FRAME_RAISED;
		
	}else{
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT;
	}
}

igdeFont *igdeNativeFoxCheckBox::CheckBoxFont( const igdeCheckBox &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::checkBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxCheckBox::CheckBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxCheckBox::CheckBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxCheckBox::CheckBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxCheckBox::CheckBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxCheckBox::onCommand( FXObject*, FXSelector, void* ){
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

long igdeNativeFoxCheckBox::onUpdate( FXObject*, FXSelector, void* ){
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
