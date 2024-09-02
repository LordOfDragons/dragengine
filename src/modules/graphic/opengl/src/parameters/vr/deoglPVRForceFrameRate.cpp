/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	SetDescription( "Force frame rate for VR rendering."
		" By default the graphic module switches to suitable frame rates depending on scene complexity. "
		" If switching happens too often frame rate can be locked for smoother experience.");
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
