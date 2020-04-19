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

#include "igdeScrollBar.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeScrollBarListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeScrollBar : public FXScrollBar{
	FXDECLARE( cNativeIgdeScrollBar )
	
protected:
	cNativeIgdeScrollBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXScrollBar::ID_LAST,
	};
	
private:
	igdeScrollBar *pOwner;
	
public:
	cNativeIgdeScrollBar( igdeScrollBar &owner, FXComposite *parent, int layoutFlags );
	virtual ~cNativeIgdeScrollBar();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	virtual FXbool canFocus() const;
	
	static int ScrollBarFlags( const igdeScrollBar &owner );
	void UpdateRange();
	void UpdateValue();
};


FXDEFMAP( cNativeIgdeScrollBar ) cNativeIgdeScrollBarMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeScrollBar::ID_SELF, cNativeIgdeScrollBar::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeScrollBar::ID_SELF, cNativeIgdeScrollBar::onCommand )
};


FXIMPLEMENT( cNativeIgdeScrollBar, FXScrollBar, cNativeIgdeScrollBarMap, ARRAYNUMBER( cNativeIgdeScrollBarMap ) )

cNativeIgdeScrollBar::cNativeIgdeScrollBar(){ }

cNativeIgdeScrollBar::cNativeIgdeScrollBar( igdeScrollBar &owner, FXComposite *parent, int layoutFlags ) :
FXScrollBar( parent, this, ID_SELF, layoutFlags | ScrollBarFlags( owner ) ),
pOwner( &owner )
{
	UpdateRange();
	UpdateValue();
	if( ! owner.GetEnabled() ){
		disable();
	}
}

cNativeIgdeScrollBar::~cNativeIgdeScrollBar(){
}

long cNativeIgdeScrollBar::onCommand( FXObject *sender, FXSelector selector, void *data ){
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

FXbool cNativeIgdeScrollBar::canFocus() const{
	return false;
	//return FXScrollBar::canFocus();
}

int cNativeIgdeScrollBar::ScrollBarFlags( const igdeScrollBar &owner ){
	switch( owner.GetOrientation() ){
	case igdeScrollBar::eoHorizontal:
		return SCROLLBAR_HORIZONTAL | SCROLLBAR_WHEELJUMP;
		
	case igdeScrollBar::eoVertical:
		return SCROLLBAR_VERTICAL | SCROLLBAR_WHEELJUMP;
		
	default:
		return 0;
	}
}

void cNativeIgdeScrollBar::UpdateRange(){
	setRange( decMath::max( pOwner->GetUpper() - pOwner->GetLower(), 0 ) );
	setPage( pOwner->GetPageSize() );
}

void cNativeIgdeScrollBar::UpdateValue(){
	setPosition( decMath::clamp( pOwner->GetValue() - pOwner->GetLower(), 0, getRange() ) );
}



// Class igdeScrollBar
////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBar::igdeScrollBar( igdeEnvironment &environment, eOrientation orientation ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( 0 ),
pUpper( 100 ),
pPageSize( 10),
pValue( 0 ),
pEnabled( true ){
}

igdeScrollBar::igdeScrollBar( igdeEnvironment &environment, eOrientation orientation,
	int lower, int upper, int pageSize, int value ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( lower ),
pUpper( upper ),
pPageSize( pageSize ),
pValue( value ),
pEnabled( true )
{
	if( pageSize < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeScrollBar::~igdeScrollBar(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeScrollBar::SetLower( int lower ){
	if( pLower == lower ){
		return;
	}
	
	pLower = lower;
	OnRangeChanged();
}

void igdeScrollBar::SetUpper( int upper ){
	if( pUpper == upper ){
		return;
	}
	
	pUpper = upper;
	OnRangeChanged();
}

void igdeScrollBar::SetPageSize( int pageSize ){
	if( pPageSize == pageSize ){
		return;
	}
	
	pPageSize = pageSize;
	OnRangeChanged();
}

void igdeScrollBar::SetValue( int value ){
	if( pValue == value ){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeScrollBar::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



void igdeScrollBar::AddListener( igdeScrollBarListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeScrollBar::RemoveListener( igdeScrollBarListener *listener ){
	pListeners.Remove( listener );
}

void igdeScrollBar::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeScrollBarListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}



void igdeScrollBar::CreateNativeWidget(){
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
	cNativeIgdeScrollBar * const foxWidget = new cNativeIgdeScrollBar( *this, foxParent, layoutFlags );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeScrollBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeScrollBar* )GetNativeWidget();
	DropNativeWidget();
}



void igdeScrollBar::OnRangeChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( cNativeIgdeScrollBar* )GetNativeWidget() )->UpdateRange();
}

void igdeScrollBar::OnValueChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( cNativeIgdeScrollBar* )GetNativeWidget() )->UpdateValue();
}

void igdeScrollBar::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( pEnabled ){
		( ( cNativeIgdeScrollBar* )GetNativeWidget() )->enable();
		
	}else{
		( ( cNativeIgdeScrollBar* )GetNativeWidget() )->disable();
	}
}
