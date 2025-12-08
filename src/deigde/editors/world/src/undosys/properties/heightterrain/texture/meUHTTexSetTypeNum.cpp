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

#include "meUHTTexSetTypeNum.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUHTTexSetTypeNum
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTTexSetTypeNum::meUHTTexSetTypeNum(meHeightTerrainTexture *texture, int newTypeNumber){
	if(!texture) DETHROW(deeInvalidParam);
	
	pTexture = texture;
	
	pOldTypeNumber = texture->GetTypeNumber();
	pNewTypeNumber = newTypeNumber;
	
	SetShortInfo("Set Height Terrain Texture Type Number");
	
	texture->AddReference();
}

meUHTTexSetTypeNum::~meUHTTexSetTypeNum(){
}



// Management
///////////////

void meUHTTexSetTypeNum::Undo(){
	pTexture->SetTypeNumber(pOldTypeNumber);
}

void meUHTTexSetTypeNum::Redo(){
	pTexture->SetTypeNumber(pNewTypeNumber);
}
