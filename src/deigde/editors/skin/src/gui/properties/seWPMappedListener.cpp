/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seWPMapped.h"
#include "seWPMappedListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/mapped/seMapped.h"

#include <dragengine/common/exceptions.h>


// Class seWPMappedListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWPMappedListener::seWPMappedListener( seWPMapped &panel ) :
pPanel( panel ){
}

seWPMappedListener::~seWPMappedListener(){
}



// Management
///////////////

void seWPMappedListener::MappedStructureChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateMappedList();
}

void seWPMappedListener::MappedChanged( seSkin *skin, seMapped *mapped ){
	if( skin != pPanel.GetSkin() || ! mapped->GetActive() ){
		return;
	}
	
	pPanel.UpdateMapped();
}

void seWPMappedListener::MappedNameChanged( seSkin *skin, seMapped* ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateMappedList();
}

void seWPMappedListener::ActiveMappedChanged( seSkin *skin ){
	if( skin != pPanel.GetSkin() ){
		return;
	}
	
	pPanel.UpdateMapped();
}
