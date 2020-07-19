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

#include "igdeNativeFoxScrollBar.h"
#include "foxIcons.h"
#include "../../igdeScrollBar.h"
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

FXDEFMAP( igdeNativeFoxScrollBar ) igdeNativeFoxScrollBarMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxScrollBar::ID_SELF, igdeNativeFoxScrollBar::onCommand ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxScrollBar::ID_SELF, igdeNativeFoxScrollBar::onCommand )
};



// Class igdeNativeFoxScrollBar
/////////////////////////////////

FXIMPLEMENT( igdeNativeFoxScrollBar, FXScrollBar, igdeNativeFoxScrollBarMap, ARRAYNUMBER( igdeNativeFoxScrollBarMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxScrollBar::igdeNativeFoxScrollBar(){ }

igdeNativeFoxScrollBar::igdeNativeFoxScrollBar( igdeScrollBar &owner, FXComposite *parent, int layoutFlags ) :
FXScrollBar( parent, this, ID_SELF, layoutFlags | ScrollBarFlags( owner ) ),
pOwner( &owner )
{
	UpdateRange();
	UpdateValue();
	if( ! owner.GetEnabled() ){
		disable();
	}
}

igdeNativeFoxScrollBar::~igdeNativeFoxScrollBar(){
}

igdeNativeFoxScrollBar *igdeNativeFoxScrollBar::CreateNativeWidget( igdeScrollBar &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxScrollBar( owner, parent, igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxScrollBar::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxScrollBar::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxScrollBar::canFocus() const{
	return false;
	//return FXScrollBar::canFocus();
}

void igdeNativeFoxScrollBar::UpdateRange(){
	setRange( decMath::max( pOwner->GetUpper() - pOwner->GetLower(), 0 ) );
	setPage( pOwner->GetPageSize() );
}

void igdeNativeFoxScrollBar::UpdateValue(){
	setPosition( decMath::clamp( pOwner->GetValue() - pOwner->GetLower(), 0, getRange() ) );
}

void igdeNativeFoxScrollBar::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}



int igdeNativeFoxScrollBar::ScrollBarFlags( const igdeScrollBar &owner ){
	switch( owner.GetOrientation() ){
	case igdeScrollBar::eoHorizontal:
		return SCROLLBAR_HORIZONTAL | SCROLLBAR_WHEELJUMP;
		
	case igdeScrollBar::eoVertical:
		return SCROLLBAR_VERTICAL | SCROLLBAR_WHEELJUMP;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxScrollBar::onCommand( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetValue( pOwner->GetLower() + getPosition() );
		pOwner->NotifyValueChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

#endif
