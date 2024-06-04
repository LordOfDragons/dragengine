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
#include <string.h>

#include "deoglRenderPlanDebug.h"



// Class deoglRenderPlanDebug
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanDebug::deoglRenderPlanDebug(){
	Reset();
}

deoglRenderPlanDebug::~deoglRenderPlanDebug(){
}



// Management
///////////////

void deoglRenderPlanDebug::Reset(){
	pViewObjects = 0;
	pViewTriangles = 0;
	pViewLights = 0;
	pViewEnvMaps = 0;
	
	pLODTriangles = 0;
	
	pCullPSObjects = 0;
	pCullPSTriangles = 0;
	pCullPSStrips = 0;
	pCullPSLights = 0;
	
	pOccMapObjects = 0;
	pOccMapTriangles = 0;
	pOccMapRenderTime = 0.0f;
	pOccTestTime = 0.0f;
	pOccTestCount = 0;
	
	pRenderedPasses = 0;
	pRenderedObjects = 0;
	pRenderedTriangles = 0;
	pRenderedLights = 0;
	pRenderedEnvMaps = 0;
	
	pSolidObjects = 0;
	pSolidTriangles = 0;
	
	pTransparentPasses = 0;
	pTransparentObjects = 0;
	pTransparentTriangles = 0;
}
