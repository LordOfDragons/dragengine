/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deoglPVRForceFrameRate.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPVRForceFrameRate
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVRForceFrameRate::deoglPVRForceFrameRate( deGraphicOpenGl &ogl ) :
deoglParameter( ogl )
{
	SetName( "vrForceFrameRate" );
	SetDescription( "Force frame rate for VR rendering. "
		"By default the graphic module switches to suitable frame rates "
		"dynamically depending on the scene complexity. "
		"If switching happens too often the frame rate can be locked "
		" to get a smoother experience.");
	SetType( eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "VR Force Frame Rate" );
	
	const deModuleParameter::SelectionEntry entries[ 5 ] = {
		{ "0", "Automatic", "Graphic module selects frame rate dynamically." },
		{ "90", "90", "Force frame rate of 90. This is the native VR frame rate." },
		{ "45", "45", "Force frame rate of 45. Uses reprojection to fill in the missed frames." },
		{ "30", "30", "Force frame rate of 30. Uses more reprojection to fill in the missed frames." },
		{ "15", "15", "Force frame rate of 15. Uses lots of reprojection to fill in the missed frames." }
	};
	AddSelectionEntries( entries, 5 );
	SetDefaultValue( "0" );
}

deoglPVRForceFrameRate::~deoglPVRForceFrameRate(){
}



// Parameter Value
////////////////////

decString deoglPVRForceFrameRate::GetParameterValue(){
	decString value;
	value.Format( "%d", pOgl.GetConfiguration().GetVRForceFrameRate() );
	return value;
}

void deoglPVRForceFrameRate::SetParameterValue( const char *value ){
	pOgl.GetConfiguration().SetVRForceFrameRate( decString( value ).ToInt() );
}
