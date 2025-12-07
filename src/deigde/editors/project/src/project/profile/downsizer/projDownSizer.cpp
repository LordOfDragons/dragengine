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
#include <string.h>
#include <stdio.h>

#include "projDownSizer.h"
#include "../projProfile.h"
#include "../../projProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class projDownSizer
////////////////////////

// Constructor, destructor
////////////////////////////

projDownSizer::projDownSizer() :
pProfile(NULL),
pQuality(100),
pScale(0){
}

projDownSizer::projDownSizer(const projDownSizer &downsizer) :
pProfile(NULL),
pDescription(downsizer.pDescription),
pPatterns(downsizer.pPatterns),
pQuality(downsizer.pQuality),
pScale(downsizer.pScale){
}

projDownSizer::~projDownSizer(){
	pCleanUp();
}



// Management
///////////////

void projDownSizer::SetProfile(projProfile *profile){
	pProfile = profile;
}

void projDownSizer::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	NotifyChanged();
}

void projDownSizer::SetPatterns(const decStringSet &patterns){
	if(patterns == pPatterns){
		return;
	}
	
	pPatterns = patterns;
	NotifyChanged();
}

void projDownSizer::SetQuality(int quality){
	quality = decMath::clamp(quality, 0, 100);
	if(quality == pQuality){
		return;
	}
	
	pQuality = quality;
	NotifyChanged();
}

void projDownSizer::SetScale(int scale){
	if(scale < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(scale == pScale){
		return;
	}
	
	pScale = scale;
	NotifyChanged();
}



void projDownSizer::NotifyChanged(){
	if(pProfile && pProfile->GetProject()){
		//pProfile->GetProject()->NotifyDownSizerChanged( pProfile, this );
	}
}



// Private Functions
//////////////////////

void projDownSizer::pCleanUp(){
}
