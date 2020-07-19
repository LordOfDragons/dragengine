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

#include "igdeContainerBorder.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeContainerBorder
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBorder::igdeContainerBorder( igdeEnvironment &environment, int spacing ) :
igdeContainer( environment ),
pSpacing ( spacing ),
pWidgetTop( NULL ),
pWidgetBottom( NULL ),
pWidgetLeft( NULL ),
pWidgetRight( NULL ),
pWidgetCenter( NULL )
{
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerBorder::~igdeContainerBorder(){
}



// Management
///////////////

void igdeContainerBorder::AddChild( igdeWidget *child, eArea area ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	if( child == pWidgetTop || child == pWidgetBottom || child == pWidgetLeft
	|| child == pWidgetRight || child == pWidgetCenter ){
		DETHROW( deeInvalidParam );
	}
	
	switch( area ){
	case eaTop:
		if( pWidgetTop ){
			DETHROW( deeInvalidParam );
		}
		pWidgetTop = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetTop = NULL;
			throw;
		}
		break;
		
	case eaBottom:
		if( pWidgetBottom ){
			DETHROW( deeInvalidParam );
		}
		pWidgetBottom = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetBottom = NULL;
			throw;
		}
		break;
		
	case eaLeft:
		if( pWidgetLeft ){
			DETHROW( deeInvalidParam );
		}
		pWidgetLeft = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetLeft = NULL;
			throw;
		}
		break;
		
	case eaRight:
		if( pWidgetRight ){
			DETHROW( deeInvalidParam );
		}
		pWidgetRight = child;
		
		try{
			igdeContainer::AddChild( child );
			
		}catch( const deException & ){
			pWidgetRight = NULL;
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
}

void igdeContainerBorder::AddChild( igdeWidget *child ){
	DETHROW( deeInvalidParam );
}

void igdeContainerBorder::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( child == pWidgetTop ){
		pWidgetTop = NULL;
		
	}else if( child == pWidgetBottom ){
		pWidgetBottom = NULL;
		
	}else if( child == pWidgetLeft ){
		pWidgetLeft = NULL;
		
	}else if( child == pWidgetRight ){
		pWidgetRight = NULL;
		
	}else if( child == pWidgetCenter ){
		pWidgetCenter = NULL;
	}
}

void igdeContainerBorder::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	
	pWidgetTop = NULL;
	pWidgetBottom = NULL;
	pWidgetLeft = NULL;
	pWidgetRight = NULL;
	pWidgetCenter = NULL;
}

igdeWidget *igdeContainerBorder::GetWidgetIn( eArea area ) const{
	switch( area ){
	case eaTop:
		return pWidgetTop;
		
	case eaBottom:
		return pWidgetBottom;
		
	case eaLeft:
		return pWidgetLeft;
		
	case eaRight:
		return pWidgetRight;
		
	case eaCenter:
		return pWidgetCenter;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



void igdeContainerBorder::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeContainerBorder * const native = igdeNativeContainerBorder::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerBorder::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeContainerBorder * const native = ( igdeNativeContainerBorder* )GetNativeWidget();
	DropNativeWidget();
	native->DestroyNativeWidget();
}
