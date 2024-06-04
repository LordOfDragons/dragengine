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

#include "deBaseVRModule.h"
#include "../../../common/exceptions.h"


// Class deBaseVRModule
/////////////////////////

deBaseVRModule::deBaseVRModule( deLoadableModule &loadableModule ) :
deBaseModule( loadableModule ){
}

deBaseVRModule::~deBaseVRModule(){
}



// Management
///////////////

void deBaseVRModule::RequestFeatureEyeGazeTracking( eFeatureSupportLevel level ){
	DEASSERT_TRUE( level != efslRequired )
}

void deBaseVRModule::RequestFeatureFacialTracking( eFeatureSupportLevel level ){
	DEASSERT_TRUE( level != efslRequired )
}

bool deBaseVRModule::SupportsPassthrough(){
	return false;
}

void deBaseVRModule::SetEnablePassthrough( bool ){
}

void deBaseVRModule::SetPassthroughTransparency( float ){
}

float deBaseVRModule::GetDeviceFaceExpression( int, int ){
	return 0.0f;
}

deBaseVRModule::eVRRenderFormat deBaseVRModule::GetRenderFormat(){
	DETHROW( deeInvalidParam );
}

int deBaseVRModule::GetEyeViewImages( eEye, int count, void* ){
	if( count == 0 ){
		return 0;
		
	}else{
		DETHROW_INFO( deeInvalidParam, "count > 0" );
	}
}

void deBaseVRModule::GetEyeViewRenderTexCoords( eEye, decVector2& tcFrom, decVector2& tcTo){
	tcFrom.Set( 0.0f, 0.0f );
	tcTo.Set( 1.0f, 1.0f );
}

int deBaseVRModule::AcquireEyeViewImage( eEye ){
	return -1;
}

void deBaseVRModule::ReleaseEyeViewImage( eEye ){
}
