/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPSObjectShape.h"
#include "meWPSObjectShapeUpdater.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>



// Class meWPSObjectShapeUpdater
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meWPSObjectShapeUpdater::meWPSObjectShapeUpdater( meWPSObjectShape &panel ) :
pPanel( panel ){
}

meWPSObjectShapeUpdater::~meWPSObjectShapeUpdater(){
}



// Management
///////////////

void meWPSObjectShapeUpdater::VisitShape( decShape& ){
	pPanel.UpdateShapeNone();
}

void meWPSObjectShapeUpdater::VisitShapeSphere( decShapeSphere &sphere ){
	pPanel.UpdateShapeSphere( sphere );
}

void meWPSObjectShapeUpdater::VisitShapeBox( decShapeBox &box ){
	pPanel.UpdateShapeBox( box );
}

void meWPSObjectShapeUpdater::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pPanel.UpdateShapeCylinder( cylinder );
}

void meWPSObjectShapeUpdater::VisitShapeCapsule( decShapeCapsule &capsule ){
	pPanel.UpdateShapeCapsule( capsule );
}

void meWPSObjectShapeUpdater::VisitShapeHull( decShapeHull &hull ){
	VisitShape( hull );
}
