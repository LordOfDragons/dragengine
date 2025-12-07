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

#include "dethInfos.h"

#include <dragengine/common/exceptions.h>



// Class dethInfos
////////////////////

// Constructor, destructor
////////////////////////////

dethInfos::dethInfos() :
pSetupInfo(NULL),
pHeaderFinished(false),
pAudioHeaderFinished(false)
{
	th_info_init(&pInfo);
	th_comment_init(&pComment);
	
	vorbis_info_init(&pAudioInfo);
	vorbis_comment_init(&pAudioComment);
}

dethInfos::~dethInfos(){
	if(pSetupInfo){
		th_setup_free(pSetupInfo);
	}
	th_comment_clear(&pComment);
	th_info_clear(&pInfo);
	
	vorbis_comment_clear(&pAudioComment);
	vorbis_info_clear(&pAudioInfo);
}



// Management
///////////////

void dethInfos::SetSetupInfo(th_setup_info *info){
	pSetupInfo = info;
}

void dethInfos::SetHeaderFinished(bool headerFinished){
	pHeaderFinished = headerFinished;
}

void dethInfos::SetAudioHeaderFinished(bool headerFinished){
	pAudioHeaderFinished = headerFinished;
}
