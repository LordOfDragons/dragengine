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

#include "meHTVInstance.h"



// Class meHTVInstance
////////////////////////

// Constructor, destructor
////////////////////////////

meHTVInstance::meHTVInstance(){
	pVLayer = 0;
	pVariation = 0;
}

meHTVInstance::~meHTVInstance(){
}



// Management
///////////////

void meHTVInstance::SetVLayer(int vlayer, int variation){
	pVLayer = (short)vlayer;
	pVariation = (short)variation;
}

void meHTVInstance::SetPosition(const decVector &position){
	pPosition = position;
}

void meHTVInstance::SetRotation(const decVector &rotation){
	pRotation = rotation;
}

void meHTVInstance::SetScaling(float scaling){
	pScaling = scaling;
}

void meHTVInstance::SetParameters(int vlayer, int variation, const decVector &position, const decVector &rotation, float scaling){
	pVLayer = (short)vlayer;
	pVariation = (short)variation;
	pPosition = position;
	pRotation = rotation;
	pScaling = scaling;
}
