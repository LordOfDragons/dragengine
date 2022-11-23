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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPLODMaxPixelError.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPLODMaxPixelError
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPLODMaxPixelError::deoglPLODMaxPixelError( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "lodMaxPixelError" );
	SetDescription( "Maximum allowed error in pixels for LOD calculation."
		" Higher values chooses LOD geometry quicker improving performance but can increase popping." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecAdvanced );
	SetDisplayName( "LOD Max Pixel Error" );
	
	const deModuleParameter::SelectionEntry entries[ 6 ] = {
		{ "1", "1 Pixel", "Allow 1 Pixel error" },
		{ "2", "2 Pixel", "Allow 2 Pixel error" },
		{ "4", "4 Pixel", "Allow 4 Pixel error" },
		{ "8", "8 Pixel", "Allow 8 Pixel error" },
		{ "16", "16 Pixel", "Allow 16 Pixel error" },
		{ "32", "32 Pixel", "Allow 32 Pixel error" }
	};
	AddSelectionEntries( entries, 6 );
	SetDefaultValue( "2" );
}

deoglPLODMaxPixelError::~deoglPLODMaxPixelError(){
}



// Parameter Value
////////////////////

int deoglPLODMaxPixelError::GetParameterInt(){
	return pOgl.GetConfiguration().GetLODMaxPixelError();
}

void deoglPLODMaxPixelError::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetLODMaxPixelError( value );
}
