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

#include "deoglPSSREnable.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSREnable
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSREnable::deoglPSSREnable( deGraphicOpenGl &ogl ) :
deoglParameterBool( ogl )
{
	SetName( "ssrEnable" );
	SetDescription( "Sets if screen space reflections are enabled."
		" Screen space reflections add local reflections between objects using the rendered image."
		" This works though only for information present in the rendered image."
		" In general many reflections can not be calculated in screen space."
		" For these global reflections are always used."
		" Local reflections improve though the spatial perception of rendered scenes."
		" As a downside though screen space reflections are heavy on the graphic card."
		" Disabling screen space reflections improves rendering speed a lot on weaker cards."
		" To fine tune the screen space reflections see the ssr* module parameters." );
	SetCategory( ecBasic );
	SetDisplayName( "SSR" );
}

deoglPSSREnable::~deoglPSSREnable(){
}



// Parameter Value
////////////////////

bool deoglPSSREnable::GetParameterBool(){
	return pOgl.GetConfiguration().GetSSREnable();
}

void deoglPSSREnable::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetSSREnable( value );
}
