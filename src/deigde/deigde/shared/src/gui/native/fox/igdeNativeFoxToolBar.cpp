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

#include "igdeNativeFoxToolBar.h"
#include "foxIcons.h"
#include "../../igdeToolBar.h"
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

FXDEFMAP( igdeNativeFoxToolBar ) igdeNativeFoxToolBarMap[] = { };



// Class igdeNativeFoxToolBar
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxToolBar, FXToolBar, igdeNativeFoxToolBarMap, ARRAYNUMBER( igdeNativeFoxToolBarMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxToolBar::igdeNativeFoxToolBar(){ }

igdeNativeFoxToolBar::igdeNativeFoxToolBar( igdeToolBar &powner, FXComposite *pparent,
	FXComposite *windowParent, int layoutFlags ) :
FXToolBar( pparent, new FXToolBarShell( windowParent, FRAME_RAISED ),
	FRAME_RAISED | /* LAYOUT_DOCK_NEXT | */ layoutFlags ),
pOwner( &powner ),
pWetDock( ( FXToolBarShell* )getWetDock() )
{
	new FXToolBarGrip( this, this, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE );
}

igdeNativeFoxToolBar::~igdeNativeFoxToolBar(){
}

igdeNativeFoxToolBar *igdeNativeFoxToolBar::CreateNativeWidget( igdeToolBar &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxToolBar( powner, pparent, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxToolBar::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( ! pparent.id() ){
		return;
	}
	
	const bool createOurself = getParent() != pWetDock;
	
	pWetDock->create();
	
	if( createOurself ){
		create();
	}
}

void igdeNativeFoxToolBar::DestroyNativeWidget(){
	const bool deleteOurself = getParent() != pWetDock;
	
	delete pWetDock;
	
	if( deleteOurself ){
		delete this;
	}
}

#endif
