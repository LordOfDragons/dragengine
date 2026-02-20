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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "debpForceFieldFluctuation.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#define random rand
#endif // OS_W32


// Definitions
////////////////

static const float vRandomFactor = 1.0f / (float)RAND_MAX;



// Class debpForceFieldFluctuation
////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpForceFieldFluctuation::debpForceFieldFluctuation() :
pDirection(0.0f),
pStrength(0.0f),
pTimer(0.0f){
}

debpForceFieldFluctuation::~debpForceFieldFluctuation(){
}



// Management
///////////////

void debpForceFieldFluctuation::Reset(){
	pDirection = 0.0f;
	pStrength = 0.0f;
	pTimer = 0.0f;
}

void debpForceFieldFluctuation::Update(float elapsed){
	pTimer += elapsed;
	
	while(pTimer > 0.04f){
		pTimer -= 0.04f;
		pDirection = decMath::clamp(pDirection + ((float)random() * vRandomFactor * 2.0f - 1.0f) * 0.4f, -1.0f, 1.0f);
		pStrength = decMath::clamp(pStrength + ((float)random() * vRandomFactor * 2.0f - 1.0f) * 0.4f, -1.0f, 1.0f);
	}
}
