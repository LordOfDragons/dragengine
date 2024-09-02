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
	
	#ifdef OS_ANDROID
		pWorking = false;
		logger.LogWarn( "Capabilities: UBO Direct Link Deadloop: Driver Bug (Android)!" );
		
	#else
		logger.LogInfo( "Capabilities: UBO Direct Link Deadloop: Working" );
	#endif
}
