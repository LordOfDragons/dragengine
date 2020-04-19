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

#include "deoglPFrameRateLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPFrameRateLimit
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPFrameRateLimit::deoglPFrameRateLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "frameRateLimit" );
	SetDescription( "Set frame rate limit of render thread. Effects only asynchronous rendering. "
		"The game frame rate is not affected.\n"
		"\n"
		"By default the parameter is set to 0 indicating to use the refresh rate of the display as "
		"frame limit for rendering. The game frame rate is decoupled and usually higher.\n"
		"\n"
		"Values larger 0 override the frame limiter. A value between 30 and 60 Hz is recommended. "
		"Smaller values (like 30) reduce GPU load allowing the graphic module to do optimizations "
		"in the spare time. Higher values rarely improve visual perception. Values beyond the "
		"display refresh rate are pointless since they are not displayed anymore swandering only "
		"valuable GPU time and heating it up for no gain." );
	SetCategory( ecExpert );
	SetDisplayName( "Frame Rate Limit" );
}

deoglPFrameRateLimit::~deoglPFrameRateLimit(){
}



// Parameter Value
////////////////////

int deoglPFrameRateLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetFrameRateLimit();
}

void deoglPFrameRateLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetFrameRateLimit( value );
}
