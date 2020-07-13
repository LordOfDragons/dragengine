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

#include "igdeProgressBar.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "native/toolkit.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeProgressBar
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeProgressBar::igdeProgressBar( igdeEnvironment &environment, int lower, int upper,
	eOrientation orientation, const char *description ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( lower ),
pUpper( decMath::max( upper, lower ) ),
pValue( lower ),
pDescription( description ){
}

igdeProgressBar::~igdeProgressBar(){
	igdeProgressBar::DestroyNativeWidget();
}



// Management
///////////////

void igdeProgressBar::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeProgressBar::SetValue( int value ){
	if( value == pValue ){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeProgressBar::SetRange( int lower, int upper ){
	if( lower == pLower && upper == pUpper ){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max( upper, lower );
	OnRangeChanged();
}



void igdeProgressBar::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeProgressBar * const native = igdeNativeProgressBar::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeProgressBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeProgressBar* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeProgressBar::OnRangeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeProgressBar* )GetNativeWidget() )->UpdateRange();
	}
}

void igdeProgressBar::OnValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeProgressBar* )GetNativeWidget() )->UpdateValue();
	}
}

void igdeProgressBar::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeProgressBar* )GetNativeWidget() )->UpdateDescription();
	}
}
