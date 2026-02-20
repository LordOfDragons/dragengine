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

#include "demdlWeightSet.h"

#include <dragengine/common/exceptions.h>



// Class demdlWeightSet
/////////////////////////

// Constructor, destructor
////////////////////////////

demdlWeightSet::demdlWeightSet() :
pGroupedIndex(-1){
}

demdlWeightSet::~demdlWeightSet() = default;



// Management
///////////////

float demdlWeightSet::GetWeightFor(int bone) const{
	const sWeight *found = nullptr;
	return pWeights.Find(found, [&](const sWeight &weight){
		return weight.bone == bone;
	}) ? found->weight : 0.0f;
}

void demdlWeightSet::Set(int bone, float weight){
	const int index = pWeights.IndexOfMatching([&](const sWeight &w){
		return w.bone == bone;
	});
	
	if(weight == 0){
		if(index != -1){
			pWeights.RemoveFrom(index);
		}
		
	}else{
		if(index != -1){
			pWeights[index].weight = weight;
			
		}else{
			pWeights.Add({bone, weight});
		}
	}
}

void demdlWeightSet::SetGroupedIndex(int index){
	pGroupedIndex = index;
}

void demdlWeightSet::Normalize(){
	if(pWeights.IsEmpty()){
		return;
	}
	
	const float sum = pWeights.Inject(0.0f, [](float acc, const sWeight &w){
		return acc + w.weight;
	});
	if(sum < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pWeights.Visit([&](sWeight &w){
		w.weight /= sum;
	});
}

bool demdlWeightSet::Equals(const demdlWeightSet &weight) const{
	return pWeights.GetCount() == weight.pWeights.GetCount()
		&& pWeights.AllMatching([&](const sWeight &w){
			return fabsf(w.weight - weight.GetWeightFor(w.bone)) <= 0.001f;
		});
}
