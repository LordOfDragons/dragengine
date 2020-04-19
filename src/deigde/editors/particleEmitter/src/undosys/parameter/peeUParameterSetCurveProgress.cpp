/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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
#include <string.h>
#include <stdlib.h>

#include "peeUParameterSetCurveProgress.h"
#include "../../emitter/peeType.h"
#include "../../emitter/peeParameter.h"

#include <dragengine/common/exceptions.h>



// Class peeUParameterSetCurveProgress
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUParameterSetCurveProgress::peeUParameterSetCurveProgress( peeType *type,
	peeParameter *parameter, const decCurveBezier &newCurve ) :
peeUParameterSetCurve( type, parameter, newCurve )
{
	SetShortInfo( "Set Parameter Progress Curve" );
	pOldCurve = parameter->GetCurveProgress();
}

peeUParameterSetCurveProgress::~peeUParameterSetCurveProgress(){
}



// Management
///////////////

void peeUParameterSetCurveProgress::Undo(){
	pParameter->SetCurveProgress( pOldCurve );
}

void peeUParameterSetCurveProgress::Redo(){
	pParameter->SetCurveProgress( pNewCurve );
}
