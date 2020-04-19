/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPSCategory.h"
#include "gdeWPSCategoryListener.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSCategoryListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSCategoryListener::gdeWPSCategoryListener( gdeWPSCategory &panel ) :
pPanel( panel ){
}

gdeWPSCategoryListener::~gdeWPSCategoryListener(){
}



// Management
///////////////

void gdeWPSCategoryListener::ObjectClassCategoriesChanged( gdeGameDefinition* ){
	if( ! pPanel.GetCategory() || pPanel.GetCategoryType() == gdeUCategoryBase::ectObjectClass ){
		pPanel.UpdateCategory();
	}
}

void gdeWPSCategoryListener::SkinCategoriesChanged( gdeGameDefinition* ){
	if( ! pPanel.GetCategory() || pPanel.GetCategoryType() == gdeUCategoryBase::ectSkin ){
		pPanel.UpdateCategory();
	}
}

void gdeWPSCategoryListener::SkyCategoriesChanged( gdeGameDefinition* ){
	if( ! pPanel.GetCategory() || pPanel.GetCategoryType() == gdeUCategoryBase::ectSky ){
		pPanel.UpdateCategory();
	}
}

void gdeWPSCategoryListener::ParticleEmitterCategoriesChanged( gdeGameDefinition* ){
	if( ! pPanel.GetCategory() || pPanel.GetCategoryType() == gdeUCategoryBase::ectParticleEmitter ){
		pPanel.UpdateCategory();
	}
}

void gdeWPSCategoryListener::ActiveCategoryChanged( gdeGameDefinition* ){
	pPanel.UpdateCategory();
}



void gdeWPSCategoryListener::ObjectClassStructureChanged( gdeGameDefinition* ){
	if( pPanel.GetCategory() && pPanel.GetCategoryType() == gdeUCategoryBase::ectObjectClass ){
		pPanel.UpdateListElements();
	}
}

void gdeWPSCategoryListener::ObjectClassNameChanged( gdeGameDefinition*, gdeObjectClass* ){
	if( pPanel.GetCategory() && pPanel.GetCategoryType() == gdeUCategoryBase::ectObjectClass ){
		pPanel.UpdateListElements();
	}
}
