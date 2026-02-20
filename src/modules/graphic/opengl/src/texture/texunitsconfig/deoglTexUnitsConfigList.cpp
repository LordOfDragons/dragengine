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
#include <string.h>

#include "deoglTexUnitConfig.h"
#include "deoglTexUnitsConfig.h"
#include "deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexUnitsConfigList
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexUnitsConfigList::deoglTexUnitsConfigList(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pTUCEmpty(nullptr)
{
	pTUCEmpty = GetWith(nullptr, 0, nullptr);
	pTUCEmpty->EnsureRTSTexture();
}

deoglTexUnitsConfigList::~deoglTexUnitsConfigList() = default;



// Management
///////////////

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetWith(const deoglTexUnitConfig *units,
int unitCount, deoglShaderParameterBlock *paramBlock){
	// if there exists already a texture units configuration with these properties add a usage and return it
	auto found = pConfigs.FindOrNull([&](deoglTexUnitsConfig *tuc){
		return tuc->Equals(units, unitCount, paramBlock);
	});
	
	if(found){
		found->AddUsage();
		return found;
	}
	
	// otherwise create a new texture units configuration with these parameters and return it
	auto tuc = deoglTexUnitsConfig::Ref::New(pRenderThread);
	deoglTexUnitsConfig *tucPtr = tuc;
	
	tuc->SetUnits(units, unitCount);
	tuc->SetParameterBlock(paramBlock);
	tuc->CalcUnitsHashCode();
	pConfigs.Add(&tuc->GetLLConfigs(), std::move(tuc));
	
	return tucPtr;
}

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetEmpty(){
	pTUCEmpty->AddUsage();
	return pTUCEmpty;
}

deoglTexUnitsConfig *deoglTexUnitsConfigList::GetEmptyNoUsage() const{
	return pTUCEmpty;
}



void deoglTexUnitsConfigList::Remove(deoglTexUnitsConfig *config){
	if(!config){
		DETHROW(deeInvalidParam);
	}
	
	pConfigs.Remove(&config->GetLLConfigs());
}