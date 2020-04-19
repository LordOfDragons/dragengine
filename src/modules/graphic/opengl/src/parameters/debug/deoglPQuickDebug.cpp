/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoglPQuickDebug.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>



// Class deoglPQuickDebug
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglPQuickDebug::deoglPQuickDebug( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "quickDebug" );
	decString description = "Quick Debug.";
	description += "\n10: disable shadeless pass";
	description += "\n11: disable tone mapping pass";
	description += "\n12: disable transparency passes";
	description += "\n13: disable solid light pass";
	description += "\n14: disable solid geometry pass";
	description += "\n15: disable solid depth pass";
	description += "\n16: disable sky pass";
	description += "\n20: disable rendering lights";
	description += "\n21: render spot lights using light volume instead of shape";
	description += "\n40: deoglRenderPlan.PlanVisibility";
	description += "\n70: deoglCollideListComponent.AddOutsideFaceStrips";
	description += "\n80: deoglRenderPlan.PlanVisibility";
	
	SetDescription( description );
	SetCategory( ecExpert );
}

deoglPQuickDebug::~deoglPQuickDebug(){
}



// Management
///////////////

int deoglPQuickDebug::GetParameterInt(){
	return pOgl.GetConfiguration().GetQuickDebug();
}

void deoglPQuickDebug::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetQuickDebug( value );
	pOgl.LogInfoFormat( "Parameter: QuickDebug = %i", pOgl.GetConfiguration().GetQuickDebug() );
}
