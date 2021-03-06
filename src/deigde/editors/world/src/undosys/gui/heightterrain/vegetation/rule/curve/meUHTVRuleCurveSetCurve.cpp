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

#include "meUHTVRuleCurveSetCurve.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRuleCurve.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleCurveSetCurve
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleCurveSetCurve::meUHTVRuleCurveSetCurve( meHTVegetationLayer *vlayer,
	meHTVRuleCurve *rule, const decCurveBezier &newCurve ) :
pVLayer( NULL ),
pRule( NULL ),
pNewCurve( newCurve )
{
	if( ! vlayer || ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Vegetation Layer Rule Curve Set Curve" );
	
	pOldCurve = rule->GetCurve();
	
	pVLayer = vlayer;
	vlayer->AddReference();
	
	pRule = rule;
	rule->AddReference();
}

meUHTVRuleCurveSetCurve::~meUHTVRuleCurveSetCurve(){
	if( pRule ){
		pRule->FreeReference();
	}
	if( pVLayer ){
		pVLayer->FreeReference();
	}
}



// Management
///////////////

void meUHTVRuleCurveSetCurve::SetNewCurve( const decCurveBezier &curve ){
	pNewCurve = curve;
}

void meUHTVRuleCurveSetCurve::Undo(){
	pRule->SetCurve( pOldCurve );
	pVLayer->NotifyRuleChanged( pRule );
}

void meUHTVRuleCurveSetCurve::Redo(){
	pRule->SetCurve( pNewCurve );
	pVLayer->NotifyRuleChanged( pRule );
}
