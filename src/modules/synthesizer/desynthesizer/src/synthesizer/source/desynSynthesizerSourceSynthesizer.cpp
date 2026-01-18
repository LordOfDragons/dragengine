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

#include "desynSynthesizerSourceSynthesizer.h"
#include "../desynSynthesizer.h"
#include "../desynCreateSynthesizerSource.h"
#include "../desynSynthesizerLink.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"
#include "../../buffer/desynSharedBuffer.h"
#include "../../buffer/desynSharedBufferList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>



// Class desynSynthesizerSourceSynthesizer
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceSynthesizer::desynSynthesizerSourceSynthesizer(desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceSynthesizer &source) :
desynSynthesizerSource(synthesizer, firstLink, source),
pChildSynthesizer(nullptr)
{
	SetSilent(!source.GetEnabled());
	if(GetSilent()){
		return;
	}
	
	pCreateSources(synthesizer, firstLink, source);
}



// Management
///////////////

int desynSynthesizerSourceSynthesizer::StateDataSizeSource(int offset){
	return pSources.Inject(0, [&](int count, desynSynthesizerSource &s){
		return count + s.StateDataSize(offset + count);
	});
}

void desynSynthesizerSourceSynthesizer::InitStateDataSource(char *stateData){
	pSources.Visit([&](desynSynthesizerSource &s){
		s.InitStateData(stateData);
	});
}



void desynSynthesizerSourceSynthesizer::GenerateSourceSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	GenerateSilence(instance, buffer, samples);
	pSources.Visit([&](desynSynthesizerSource &s){
		s.GenerateSound(instance, stateData, buffer, samples, curveOffset, curveFactor);
	});
}

void desynSynthesizerSourceSynthesizer::SkipSourceSound(const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor){
	pSources.Visit([&](desynSynthesizerSource &s){
		s.SkipSound(instance, stateData, samples, curveOffset, curveFactor);
	});
}



// Private Functions
//////////////////////

void desynSynthesizerSourceSynthesizer::pCreateSources(desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceSynthesizer &source){
	pSources.RemoveAll();
	
	const deSynthesizer * const childSynthesizer = source.GetSynthesizer();
	if(!childSynthesizer){
		SetSilent(true);
		return;
	}
	
	if(childSynthesizer->GetSources().IsEmpty()){
		SetSilent(true);
		return;
	}
	
	// create controller mapping
	const int controllerCount = childSynthesizer->GetControllers().GetCount();
	const int connectionCount = source.GetConnectionCount();
	decTList<int> controllerMapping;
	int i;
	
	for(i=0; i<controllerCount; i++){
		if(i >= connectionCount){
			controllerMapping.Add(-1);
			continue;
		}
		
		const int localController = source.GetConnectionAt(i);
		if(localController == -1){
			controllerMapping.Add(-1);
			continue;
		}
		
		controllerMapping.Add(localController);
	}
	
	// add sub links
	const int childFirstLink = synthesizer.GetLinkCount();
	
	childSynthesizer->GetLinks().Visit([&](const deSynthesizerLink &l){
		synthesizer.AddLink(desynSynthesizerLink::Ref::New(l, controllerMapping));
	});
	
	// create rules
	desynCreateSynthesizerSource createSource(synthesizer, childFirstLink);
	childSynthesizer->GetSources().Visit([&](deSynthesizerSource &s){
		createSource.Reset();
		s.Visit(createSource);
		pSources.Add(std::move(createSource.GetSource()));
	});
}
