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

#include "igdeContainerSplitted.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>


// Class igdeContainerSplitted
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( -1 ),
pSplitPositionRelative( 0.5f ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement, int splitPosition ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( splitPosition ),
pSplitPositionRelative( 0.0f ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::igdeContainerSplitted( igdeEnvironment &environment,
	eSidePlacement sidePlacement, float splitPosition ) :
igdeContainer( environment ),
pSidePlacement( sidePlacement ),
pSplitPosition( -1 ),
pSplitPositionRelative( decMath::clamp( splitPosition, 0.0f, 1.0f ) ),
pWidgetSide( NULL ),
pWidgetCenter( NULL ){
}

igdeContainerSplitted::~igdeContainerSplitted(){
}



// Management
///////////////

void igdeContainerSplitted::SetSplitPosition( int splitPosition ){
	if( splitPosition == pSplitPosition ){
		return;
	}
	
	pSplitPosition = splitPosition;
	
	if( GetNativeWidget() ){
		( ( igdeNativeContainerSplitted* )GetNativeWidget() )->UpdateSplitValue();
	}
}

void igdeContainerSplitted::SetSplitPositionRelative( float splitPosition ){
	if( fabsf( splitPosition - pSplitPositionRelative ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSplitPositionRelative = splitPosition;
	pSplitPosition = -1;
	
	if( GetNativeWidget() ){
		( ( igdeNativeContainerSplitted* )GetNativeWidget() )->UpdateSplitValue();
	}
}



void igdeContainerSplitted::AddChild( igdeWidget *child, eArea area ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	if( child == pWidgetSide || child == pWidgetCenter ){
		DETHROW( deeInvalidParam );
	}
	
	switch( area ){
	case eaSide:
		if( pWidgetSide ){
			DETHROW( deeInvalidParam );
		}
		pWidgetSide = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetSide = NULL;
			throw;
		}
		break;
		
	case eaCenter:
		if( pWidgetCenter ){
			DETHROW( deeInvalidParam );
		}
		pWidgetCenter = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetCenter = NULL;
			throw;
		}
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	if( GetNativeWidget() ){
		( ( igdeNativeContainerSplitted* )GetNativeWidget() )->UpdateSplitValue();
	}
}

void igdeContainerSplitted::AddChild( igdeWidget* ){
	DETHROW( deeInvalidParam );
}

void igdeContainerSplitted::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( child == pWidgetSide ){
		pWidgetSide = NULL;
		
	}else if( child == pWidgetCenter ){
		pWidgetCenter = NULL;
	}
}

void igdeContainerSplitted::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	
	pWidgetSide = NULL;
	pWidgetCenter = NULL;
}

igdeWidget *igdeContainerSplitted::GetWidgetIn( eArea area ) const{
	switch( area ){
	case eaSide:
		return pWidgetSide;
		
	case eaCenter:
		return pWidgetCenter;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



void igdeContainerSplitted::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeContainerSplitted * const native = igdeNativeContainerSplitted::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->UpdateSplitValue();
}

void igdeContainerSplitted::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeContainerSplitted * const native = ( igdeNativeContainerSplitted* )GetNativeWidget();
	DropNativeWidget();
	native->DestroyNativeWidget();
}

void igdeContainerSplitted::CreateChildWidgetNativeWidgets(){
	switch( pSidePlacement ){
	case igdeContainerSplitted::espBottom:
	case igdeContainerSplitted::espRight:
		if( pWidgetCenter ){
			pWidgetCenter->CreateNativeWidget();
		}
		if( pWidgetSide ){
			pWidgetSide->CreateNativeWidget();
		}
		break;
		
	case igdeContainerSplitted::espTop:
	case igdeContainerSplitted::espLeft:
	default:
		if( pWidgetSide ){
			pWidgetSide->CreateNativeWidget();
		}
		if( pWidgetCenter ){
			pWidgetCenter->CreateNativeWidget();
		}
		break;
	}
}
