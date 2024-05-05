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

#include <stdlib.h>

#include "deoxrDPHTCVivePro.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHTCVivePro
////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHTCVivePro::deoxrDPHTCVivePro( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_pro" ),
	"HTC VIVE Pro HMD" )
{
}

deoxrDPHTCVivePro::~deoxrDPHTCVivePro(){
}



// Management
///////////////

void deoxrDPHTCVivePro::SuggestBindings(){
	// Valid for user paths:
	// - /user/head
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/volume_up/click
	// - /input/volume_down/click
	// - /input/mute_mic/click
	
#if 0
	const int bindingCount = 1;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePath( "/user/head" );
	
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/system/click" );
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
#endif
}
