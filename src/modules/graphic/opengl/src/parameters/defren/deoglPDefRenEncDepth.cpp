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

#include "deoglPDefRenEncDepth.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglPDefRenEncDepth
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPDefRenEncDepth::deoglPDefRenEncDepth( deGraphicOpenGl &ogl ) : deoglParameterBool( ogl ){
	SetName( "defRenEncDepth" );
	SetDescription( "Determines if encoded depth is used for deferred rendering." );
	SetCategory( ecExpert );
}

deoglPDefRenEncDepth::~deoglPDefRenEncDepth(){
}



// Management
///////////////

bool deoglPDefRenEncDepth::GetParameterBool(){
	return pOgl.GetConfiguration().GetDefRenEncDepth();
}

void deoglPDefRenEncDepth::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetDefRenEncDepth( value );
}
