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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglPWireframeMode.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "dragengine/common/exceptions.h"



// class deoglPWireframeMode
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPWireframeMode::deoglPWireframeMode( deGraphicOpenGl &ogl ) : deoglParameterBool( ogl ){
	SetName( "wireframeMode" );
	SetDescription( "Renders in wireframe mode" );
	SetCategory( ecExpert );
	SetDisplayName( "Wireframe Mode" );
}

deoglPWireframeMode::~deoglPWireframeMode(){
}



// Parameter Value
////////////////////

bool deoglPWireframeMode::GetParameterBool(){
	return pOgl.GetConfiguration().GetDebugWireframe();
}

void deoglPWireframeMode::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetDebugWireframe( value );
}
