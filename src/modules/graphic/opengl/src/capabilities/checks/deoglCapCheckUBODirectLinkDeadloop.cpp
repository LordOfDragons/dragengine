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

#include "deoglCapCheckUBODirectLinkDeadloop.h"
#include "../deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckUBODirectLinkDeadloop
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckUBODirectLinkDeadloop::deoglCapCheckUBODirectLinkDeadloop(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckUBODirectLinkDeadloop::Check( GLuint fbo ){
	// Summary:
	//    The UBO direct link deadloop bug is has been observed on smartphone devices so far. It is
	//    not known if it affects only OpenGL ES used in these devices or if it is a smartphone driver
	//    problem. This bug is a very nasty one causing GPU drivers to deadloop and hang while linking
	//    shaders causing the application to be frozen too with no chance to ever continue.
	//    
	// Background:
	//    The following example code causes the problem:
	//    
	//    layout (std140, column_major) uniform UboBlockData {
	//        vec3 pVec3Param;
	//        float pFloatParam;
	//    };
	//    void main(void){
	//        outValue = pow(pFloatParam, 2.0);
	//    }
	//    
	//    The shader should output the float parameter raised to the power of 2. This is a direct UBO
	//    parameter access since the parameter is used as function input parameter directly. This causes
	//    the driver to deadloop. On the other hand with a slight change the deadloop vanishes:
	//    
	//    void main(void){
	//        outValue = pow(float(pFloatParam), 2.0);
	//    }
	//    
	//    The UBO parameter is no more accessed directly. Instead if is first used in a float constructor
	//    call. This small change prevents the driver from dead-looping.
	//    
	//    Using such indirect calls on non-broken compilers is not a problem though since a good compiler
	//    should optimized away the float-constructor call while broken drivers are prevented from
	//    dead-looping.
	//    
	//    The cause of this problem is unknown. Compiling does not fail but linking. It could be the
	//    broken compilers can not handle interleaved UBO parameters (correct according to GLSL std140)
	//    well while optimizing resulting in problems.
	//    
	// Bug Behavior:
	//    In a bugged driver linking such a shader causes the driver to deadloop inside linking
	//    locking up the application.
	//    
	// Test Procedure:
	//    Since this is a deadloop bug no tests can be done to verify the presence. On Android
	//    platforms this bug is considered present. On all other platforms not.
	
	deoglRTLogger &logger = pCapabilities.GetRenderThread().GetLogger();
	
	#ifdef ANDROID
		pWorking = false;
		logger.LogWarn( "Capabilities: UBO Direct Link Deadloop: Driver Bug (Android)!" );
		
	#else
		logger.LogInfo( "Capabilities: UBO Direct Link Deadloop: Working" );
	#endif
}
