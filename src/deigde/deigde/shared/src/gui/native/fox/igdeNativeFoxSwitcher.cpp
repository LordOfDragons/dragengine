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

#include "igdeNativeFoxSwitcher.h"
#include "foxIcons.h"
#include "../../igdeSwitcher.h"
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

FXDEFMAP( igdeNativeFoxSwitcher ) igdeNativeFoxSwitcherMap[] = {
};



// Class igdeNativeFoxSwitcher
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxSwitcher, FXSwitcher, igdeNativeFoxSwitcherMap, ARRAYNUMBER( igdeNativeFoxSwitcherMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxSwitcher::igdeNativeFoxSwitcher(){ }

igdeNativeFoxSwitcher::igdeNativeFoxSwitcher( igdeSwitcher &powner, FXComposite *pparent, int layoutFlags ) :
FXSwitcher( pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ){
}

igdeNativeFoxSwitcher::~igdeNativeFoxSwitcher(){
}

igdeNativeFoxSwitcher *igdeNativeFoxSwitcher::CreateNativeWidget( igdeSwitcher &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxSwitcher( powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxSwitcher::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxSwitcher::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSwitcher::UpdateCurrent(){
	setCurrent( pOwner->GetCurrent() );
}

#endif
