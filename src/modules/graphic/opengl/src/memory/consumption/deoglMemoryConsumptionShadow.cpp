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

#include "deoglMemoryConsumptionShadow.h"


// Class deoglMemoryConsumptionShadow
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionShadow::deoglMemoryConsumptionShadow() :
all("Shadow"),
allStatic("Shadow Static", all),
allDynamic("Shadow Dynamic", all),

skyStatic("Shadow Sky Static", allStatic),
skyDynamic("Shadow Sky Dynamic", allDynamic),

solidStaticMap("Shadow Solid Static Map", allStatic),
solidStaticCube("Shadow Solid Static Cube", allStatic),
solidStaticArray("Shadow Solid Static Array", allStatic),
solidDynamicMap("Shadow Solid Dynamic Map", allDynamic),
solidDynamicCube("Shadow Solid Dynamic Cube", allDynamic),
solidDynamicArray("Shadow Solid Dynamic Array", allDynamic),

transparentStaticMapDepth("Shadow Transparent Static Map Depth", allStatic),
transparentStaticMapColor("Shadow Transparent Static Map Color", allStatic),
transparentStaticCubeDepth("Shadow Transparent Static Cube Depth", allStatic),
transparentStaticCubeColor("Shadow Transparent Static Cube Color", allStatic),
transparentDynamicMapDepth("Shadow Transparent Dynamic Map Depth", allDynamic),
transparentDynamicMapColor("Shadow Transparent Dynamic Map Color", allDynamic),
transparentDynamicCubeDepth("Shadow Transparent Dynamic Cube Depth", allDynamic),
transparentDynamicCubeColor("Shadow Transparent Dynamic Cube Color", allDynamic),

ambientStaticMap("Shadow Solid Static Ambient Map", allStatic),
ambientStaticCube("Shadow Solid Static Ambient Cube", allStatic),
ambientDynamicMap("Shadow Solid Dynamic Ambient Map", allDynamic),
ambientDynamicCube("Shadow Solid Dynamic Ambient Cube", allDynamic){
}

deoglMemoryConsumptionShadow::~deoglMemoryConsumptionShadow(){
}
