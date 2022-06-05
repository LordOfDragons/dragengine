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

#include "igdeNativeFoxToolBarDock.h"
#include "foxIcons.h"
#include "../../igdeToolBarDock.h"
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

FXDEFMAP( igdeNativeFoxToolBarDock ) igdeNativeFoxToolBarDockMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxToolBarDock::onChildLayoutFlags )
};



// Class igdeNativeFoxToolBarDock
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxToolBarDock, FXDockSite,
	igdeNativeFoxToolBarDockMap, ARRAYNUMBER( igdeNativeFoxToolBarDockMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxToolBarDock::igdeNativeFoxToolBarDock(){ }

igdeNativeFoxToolBarDock::igdeNativeFoxToolBarDock( igdeToolBarDock &powner, FXComposite *pparent ) :
FXDockSite( pparent, LayoutFlags( powner ) ),
pOwner( &powner ){
}

igdeNativeFoxToolBarDock::~igdeNativeFoxToolBarDock(){
}

igdeNativeFoxToolBarDock *igdeNativeFoxToolBarDock::CreateNativeWidget( igdeToolBarDock &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxToolBarDock( powner, pparent );
}

void igdeNativeFoxToolBarDock::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxToolBarDock::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

int igdeNativeFoxToolBarDock::LayoutFlags( const igdeToolBarDock &powner ){
	switch( powner.GetSide() ){
	case igdeToolBarDock::esTop:
		return LAYOUT_SIDE_TOP | LAYOUT_FILL_X;
		
	case igdeToolBarDock::esLeft:
		return LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esRight:
		return LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esBottom:
		return LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxToolBarDock::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	
	switch( pOwner->GetSide() ){
	case igdeToolBarDock::esTop:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
		
	case igdeToolBarDock::esLeft:
		clflags.flags = LAYOUT_SIDE_LEFT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esRight:
		clflags.flags = LAYOUT_SIDE_RIGHT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esBottom:
		clflags.flags = LAYOUT_SIDE_BOTTOM; // | LAYOUT_FILL_X;
		break;
		
	default:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
	}
	
// 	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}

#endif
