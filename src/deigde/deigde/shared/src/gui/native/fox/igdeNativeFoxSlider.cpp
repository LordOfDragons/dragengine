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

#include "igdeNativeFoxSlider.h"
#include "foxIcons.h"
#include "../../igdeSlider.h"
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

FXDEFMAP( igdeNativeFoxSlider ) igdeNativeFoxSliderMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxSlider::ID_SELF, igdeNativeFoxSlider::onCommand ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxSlider::ID_SELF, igdeNativeFoxSlider::onChanged )
};



// Class igdeNativeFoxSlider
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxSlider, FXSlider, igdeNativeFoxSliderMap, ARRAYNUMBER( igdeNativeFoxSliderMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxSlider::igdeNativeFoxSlider(){ }

igdeNativeFoxSlider::igdeNativeFoxSlider( igdeSlider &owner, FXComposite *parent, int layoutFlags ) :
FXSlider( parent, this, ID_SELF, layoutFlags | SliderFlags( owner ) ),
pOwner( &owner )
{
	UpdateScale();
	UpdateRange();
	UpdateValue();
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

igdeNativeFoxSlider::~igdeNativeFoxSlider(){
}

igdeNativeFoxSlider *igdeNativeFoxSlider::CreateNativeWidget( igdeSlider &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxSlider( owner, parent, igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxSlider::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxSlider::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSlider::Focus(){
	setFocus();
}

void igdeNativeFoxSlider::UpdateScale(){
	pScale = powf( 10.0f, ( float )pOwner->GetPrecision() );
	pInvScale = 1.0f / pScale;
}

void igdeNativeFoxSlider::UpdateRange(){
	setRange( ( FXint )( pOwner->GetLower() * pScale + 0.5f ),
		( FXint )( pOwner->GetUpper() * pScale + 0.5f ) );
	setTickDelta( ( FXint )( pOwner->GetTickSpacing() * pScale + 0.5f ) );
}

void igdeNativeFoxSlider::UpdateValue(){
	setValue( ( FXint )( pOwner->GetValue() * pScale + 0.5f ) );
}

void igdeNativeFoxSlider::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxSlider::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}



int igdeNativeFoxSlider::SliderFlags( const igdeSlider &owner ){
	switch( owner.GetOrientation() ){
	case igdeSlider::eoHorizontal:
		return SLIDER_HORIZONTAL | SLIDER_ARROW_DOWN | SLIDER_TICKS_BOTTOM;
		
	case igdeSlider::eoVertical:
		return SLIDER_VERTICAL | SLIDER_ARROW_RIGHT | SLIDER_TICKS_RIGHT;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxSlider::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	// we have to send this notification always even if nothing changed. this is because
	// a SEL_CHANGED can be send right before the SEL_COMMAND and then the value would
	// be already set and the SEL_COMMAND would be ignore. ignoring the SEL_COMMAND though
	// breaks listeners only listening for the "changed" event.
	// 
	// we can re-enable this code if we start tracking ourself if the value changed by a
	// previous SEL_CHANGED. but this would need first a true gain to be obvious
	const float value = ( float )getValue() * pInvScale;
	/*if( fabsf( pOwner->GetValue() - value ) < FLOAT_SAFE_EPSILON ){
		return 0;
	}*/
	
	try{
		pOwner->SetValue( value );
		pOwner->NotifyValueChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxSlider::onChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	const float value = ( float )getValue() * pInvScale;
	if( fabsf( pOwner->GetValue() - value ) < FLOAT_SAFE_EPSILON ){
		return 0;
	}
	
	try{
		pOwner->SetValue( value );
		pOwner->NotifyValueChanging();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

#endif
