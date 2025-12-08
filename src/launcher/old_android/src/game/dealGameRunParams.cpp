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

#include "dealGameRunParams.h"
#include "profile/dealGameProfile.h"

#include "../common/exceptions.h"



// Class dealGameRunParams
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameRunParams::dealGameRunParams(){
	pGameProfile = NULL;
	pWidth = 0;
	pHeight = 0;
	pFullScreen = false;
}

dealGameRunParams::~dealGameRunParams(){
	SetGameProfile(NULL);
}



// Management
///////////////

void dealGameRunParams::SetGameProfile(dealGameProfile *profile){
	if(profile != pGameProfile){
		pGameProfile = profile;
		if(profile) profile->AddReference();
	}
}

void dealGameRunParams::SetRunArguments(const char *arguments){
	if(!arguments) DETHROW(deeInvalidParam);
	pRunArguments = arguments;
}

void dealGameRunParams::SetWidth(int width){
	if(width < 1) DETHROW(deeInvalidParam);
	
	pWidth = width;
}

void dealGameRunParams::SetHeight(int height){
	if(height < 1) DETHROW(deeInvalidParam);
	
	pHeight = height;
}

void dealGameRunParams::SetFullScreen(bool fullScreen){
	pFullScreen = fullScreen;
}
