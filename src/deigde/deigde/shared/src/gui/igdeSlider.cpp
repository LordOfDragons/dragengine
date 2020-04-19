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

#include "igdeSlider.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeSliderListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeSlider : public FXSlider{
	FXDECLARE( cNativeIgdeSlider )
	
protected:
	cNativeIgdeSlider();
	
public:
	enum eFoxIDs{
		ID_SELF = FXSlider::ID_LAST,
	};
	
private:
	igdeSlider *pOwner;
	float pScale;
	float pInvScale;
	
public:
	cNativeIgdeSlider( igdeSlider &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeIgdeSlider();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChanged( FXObject *sender, FXSelector selector, void *data );
	
	static int SliderFlags( const igdeSlider &owner );
	void UpdateScale();
	void UpdateRange();
	void UpdateValue();
	virtual void Focus();
};


FXDEFMAP( cNativeIgdeSlider ) cNativeIgdeSliderMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeSlider::ID_SELF, cNativeIgdeSlider::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeSlider::ID_SELF, cNativeIgdeSlider::onChanged )
};


FXIMPLEMENT( cNativeIgdeSlider, FXSlider, cNativeIgdeSliderMap, ARRAYNUMBER( cNativeIgdeSliderMap ) )

cNativeIgdeSlider::cNativeIgdeSlider(){ }

cNativeIgdeSlider::cNativeIgdeSlider( igdeSlider &owner, FXComposite *parent, int layoutFlags ) :
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

cNativeIgdeSlider::~cNativeIgdeSlider(){
}

long cNativeIgdeSlider::onCommand( FXObject*, FXSelector, void* ){
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

long cNativeIgdeSlider::onChanged( FXObject*, FXSelector, void* ){
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

int cNativeIgdeSlider::SliderFlags( const igdeSlider &owner ){
	switch( owner.GetOrientation() ){
	case igdeSlider::eoHorizontal:
		return SLIDER_HORIZONTAL | SLIDER_ARROW_DOWN | SLIDER_TICKS_BOTTOM;
		
	case igdeSlider::eoVertical:
		return SLIDER_VERTICAL | SLIDER_ARROW_RIGHT | SLIDER_TICKS_RIGHT;
		
	default:
		return 0;
	}
}

void cNativeIgdeSlider::UpdateScale(){
	pScale = powf( 10.0f, ( float )pOwner->GetPrecision() );
	pInvScale = 1.0f / pScale;
}

void cNativeIgdeSlider::UpdateRange(){
	setRange( ( FXint )( pOwner->GetLower() * pScale + 0.5f ),
		( FXint )( pOwner->GetUpper() * pScale + 0.5f ) );
	setTickDelta( ( FXint )( pOwner->GetTickSpacing() * pScale + 0.5f ) );
}

void cNativeIgdeSlider::UpdateValue(){
	setValue( ( FXint )( pOwner->GetValue() * pScale + 0.5f ) );
}

void cNativeIgdeSlider::Focus(){
	setFocus();
}



// Class igdeSlider
/////////////////////

// Constructor, destructor
////////////////////////////

igdeSlider::igdeSlider( igdeEnvironment &environment, eOrientation orientation,
	const char *description ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( 0.0f ),
pUpper( 10.0f ),
pTickSpacing( 1.0f ),
pValue( 0.0f ),
pPrecision( 0 ),
pEnabled( true ),
pDescription( description ){
}

igdeSlider::igdeSlider( igdeEnvironment &environment, eOrientation orientation,
	float lower, float upper, int precision, float tickSpacing, const char *description ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( lower ),
pUpper( decMath::max( upper, lower ) ),
pTickSpacing( decMath::max( tickSpacing, 0.0f ) ),
pValue( lower ),
pPrecision( precision ),
pEnabled( true ),
pDescription( description )
{
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeSlider::~igdeSlider(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSlider::SetRange( float lower, float upper ){
	if( fabsf( pLower - lower ) < FLOAT_SAFE_EPSILON
	&& fabsf( pUpper - upper ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max( upper, lower );
	OnRangeChanged();
}

void igdeSlider::SetTickSpacing( float spacing ){
	if( fabsf( pTickSpacing - spacing ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pTickSpacing = spacing;
	OnRangeChanged();
}

void igdeSlider::SetValue( float value ){
	if( fabsf( pValue - value ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = decMath::clamp( value, pLower, pUpper );
	OnValueChanged();
}

void igdeSlider::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( precision == pPrecision ){
		return;
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeSlider::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeSlider::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeSlider::Focus(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeSlider* )GetNativeWidget() )->Focus();
	}
}



void igdeSlider::AddListener( igdeSliderListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeSlider::RemoveListener( igdeSliderListener *listener ){
	pListeners.Remove( listener );
}

void igdeSlider::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSliderListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}

void igdeSlider::NotifyValueChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSliderListener* )listeners.GetAt( i ) )->OnValueChanging( this );
	}
}



void igdeSlider::CreateNativeWidget(){
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
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeIgdeSlider * const native = new cNativeIgdeSlider( *this, foxParent, layoutFlags );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeSlider::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeSlider* )GetNativeWidget();
	DropNativeWidget();
}



void igdeSlider::OnRangeChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSlider &native = *( ( cNativeIgdeSlider* )GetNativeWidget() );
	native.UpdateRange();
}

void igdeSlider::OnValueChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSlider &native = *( ( cNativeIgdeSlider* )GetNativeWidget() );
	native.UpdateValue();
}

void igdeSlider::OnPrecisionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSlider &native = *( ( cNativeIgdeSlider* )GetNativeWidget() );
	native.UpdateScale();
	native.UpdateRange();
	native.UpdateValue();
}

void igdeSlider::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSlider &native = *( ( cNativeIgdeSlider* )GetNativeWidget() );
	
	if( pEnabled ){
		native.enable();
		
	}else{
		native.disable();
	}
}

void igdeSlider::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSlider &native = *( ( cNativeIgdeSlider* )GetNativeWidget() );
	native.setTipText( pDescription.GetString() );
	native.setHelpText( pDescription.GetString() );
}
