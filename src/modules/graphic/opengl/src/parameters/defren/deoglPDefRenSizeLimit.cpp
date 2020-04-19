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

#include "deoglPDefRenSizeLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../rendering/defren/deoglDeferredRendering.h"

#include <dragengine/common/exceptions.h>



// Class deoglPDefRenSizeLimit
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPDefRenSizeLimit::deoglPDefRenSizeLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "defRenSizeLimit" );
	SetDescription( "Sets the size limit for deferred rendering. Some graphic cards or"
		" drivers have troubles with large render resolutions especially if used with"
		" deferred rendering. In this case limiting the internal rendering size can help."
		" To use the limit set defRenSizeLimit to a value larger than 0. If the rendering"
		" would be larger than this value the internal rendering is scaled proportionally"
		" so that neither the width nor the height is larger. Once finished rendering the"
		" image is scaled up to the desired size." );
	SetCategory( ecExpert );
}

deoglPDefRenSizeLimit::~deoglPDefRenSizeLimit(){
}



// Management
///////////////

int deoglPDefRenSizeLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetDefRenSizeLimit();
}

void deoglPDefRenSizeLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetDefRenSizeLimit( value );
}
