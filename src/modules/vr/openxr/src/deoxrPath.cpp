/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoxrPath.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"


// class deoxrPath
////////////////////

deoxrPath::deoxrPath() :
pPath( XR_NULL_PATH ){
}

deoxrPath::deoxrPath( const deoxrInstance &instance, XrPath path ) :
pPath( path )
{
	if( path == XR_NULL_PATH ){
		return;
	}
	
	uint32_t bufferCountOutput;
	
	OXR_CHECK( instance.GetOxr(), instance.xrPathToString( instance.GetInstance(),
		path, 0, &bufferCountOutput, nullptr ) );
	
	pName.Set( ' ', bufferCountOutput );
	
	OXR_CHECK( instance.GetOxr(), instance.xrPathToString( instance.GetInstance(),
		path, bufferCountOutput, &bufferCountOutput, ( char* )pName.GetString() ) );
}

deoxrPath::deoxrPath( const deoxrInstance &instance, const char *name ) :
pPath( XR_NULL_PATH ),
pName( name )
{
	OXR_CHECK( instance.GetOxr(), instance.xrStringToPath( instance.GetInstance(), name, &pPath ) );
}

deoxrPath::deoxrPath( const deoxrPath &path ) :
pPath( path.pPath ),
pName( path.pName ){
}

deoxrPath::~deoxrPath(){
}



// Management
///////////////

void deoxrPath::Empty(){
	pPath = XR_NULL_PATH;
	pName.Empty();
}



// Operators
//////////////

deoxrPath &deoxrPath::operator=( const deoxrPath &path ){
	pPath = path.pPath;
	pName = path.pName;
	return *this;
}
