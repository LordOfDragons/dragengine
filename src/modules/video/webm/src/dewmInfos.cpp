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

#include "dewmInfos.h"

#include <dragengine/common/exceptions.h>



// Class dewmInfos
////////////////////

// Constructor, destructor
////////////////////////////

dewmInfos::dewmInfos() :
pDuration(0.0),
pVideoCodec(evcUnsupported),
pVideoTrackNumber(0),
pAudioCodec(eacUnsupported),
pAudioTrackNumber(0){
}

dewmInfos::~dewmInfos(){
}



// Management
///////////////

void dewmInfos::SetDuration(double duration){
	pDuration = decMath::max(duration, 0.0);
}



void dewmInfos::SetVideoCodec(dewmInfos::eVideoCodec codec){
	pVideoCodec = codec;
}

void dewmInfos::SetVideoTrackNumber(uint64_t number){
	pVideoTrackNumber = number;
}



void dewmInfos::SetAudioCodec(dewmInfos::eAudioCodec codec){
	pAudioCodec = codec;
}

void dewmInfos::SetAudioTrackNumber(uint64_t number){
	pAudioTrackNumber = number;
}
