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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seSourceGroup.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Class seSourceGroup
////////////////////////

// Constructor, destructor
////////////////////////////

seSourceGroup::seSourceGroup() :
seSource(deSynthesizerSourceVisitorIdentify::estGroup),
pApplicationType(deSynthesizerSourceGroup::eatAll),
pTreeListExpanded(false){
}

seSourceGroup::seSourceGroup(const seSourceGroup &copy) :
seSource(copy),
pApplicationType(copy.pApplicationType),
pTargetSelect(copy.pTargetSelect),
pTreeListExpanded(copy.pTreeListExpanded)
{
	copy.pSources.Visit([&](const seSource &s){
		const seSource::Ref source(s.CreateCopy());
		pSources.Add(source);
		source->SetParentGroup(this);
	});
}

seSourceGroup::~seSourceGroup(){
	pCleanUp();
}



// Management
///////////////

void seSourceGroup::AddSource(seSource *source){
	pSources.Add(source);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	source->SetParentGroup(this);
	source->SetSynthesizer(synthesizer);
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void seSourceGroup::InsertSourceAt(seSource *source, int index){
	pSources.Insert(source, index);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	source->SetParentGroup(this);
	source->SetSynthesizer(synthesizer);
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void seSourceGroup::MoveSourceTo(seSource *source, int index){
	pSources.Move(source, index);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void seSourceGroup::RemoveSource(seSource *source){
	if(!pSources.Has(source)){
		DETHROW(deeInvalidParam);
	}
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	if(synthesizer && source == synthesizer->GetActiveSource()){
		const int index = pSources.IndexOf(source);
		const int sourceCount = pSources.GetCount();
		
		if(sourceCount == 1){
			synthesizer->SetActiveSource(this);
			
		}else if(index < sourceCount - 1){
			synthesizer->SetActiveSource(pSources.GetAt(index + 1));
			
		}else{
			synthesizer->SetActiveSource(pSources.GetAt(index - 1));
		}
	}
	
	source->SetParentGroup(nullptr);
	source->SetSynthesizer(nullptr);
	
	pSources.Remove(source);
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void seSourceGroup::RemoveAllSources(){
	const int count = pSources.GetCount();
	int i;
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	
	if(synthesizer && pSources.Has(synthesizer->GetActiveSource())){
		synthesizer->SetActiveSource(this);
	}
	
	for(i=0; i<count; i++){
		seSource * const source = pSources.GetAt(i);
		source->SetParentGroup(nullptr);
		source->SetSynthesizer(nullptr);
	}
	
	pSources.RemoveAll();
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}



void seSourceGroup::SetApplicationType(deSynthesizerSourceGroup::eApplicationTypes type){
	if(type == pApplicationType){
		return;
	}
	
	pApplicationType = type;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceGroup>()->SetApplicationType(type);
		NotifySourceChanged();
	}
}



void seSourceGroup::UpdateTargets(){
	seSource::UpdateTargets();
	
	deSynthesizerSourceGroup * const source = GetEngineSource().DynamicCast<deSynthesizerSourceGroup>();
	if(source){
		seSynthesizer *synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer)
		
		pTargetSelect.UpdateEngineTarget(*synthesizer, source->GetTargetSelect());
	}
	
	pSources.Visit([](seSource &source){
		source.UpdateTargets();
	});
}

int seSourceGroup::CountLinkUsage(seLink *link) const{
	int usageCount = seSource::CountLinkUsage(link);
	
	if(pTargetSelect.GetLinks().Has(link)){
		usageCount++;
	}
	
	pSources.Visit([&](const seSource &source){
		usageCount += source.CountLinkUsage(link);
	});
	
	return usageCount;
}

void seSourceGroup::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	pTargetSelect.RemoveLink(link);
	
	pSources.Visit([&](seSource &source){
		source.RemoveLinkFromTargets(link);
	});
	
	seSource::UpdateTargets();
}

void seSourceGroup::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSelect.RemoveAllLinks();
	
	pSources.Visit([](seSource &source){
		source.RemoveLinksFromAllTargets();
	});
	
	seSource::UpdateTargets();
}



deSynthesizerSource::Ref seSourceGroup::CreateEngineSource(){
	pSources.Visit([&](seSource &source){
		source.SetEngineSource(nullptr);
	});
	
	const deSynthesizerSourceGroup::Ref engSource(deSynthesizerSourceGroup::Ref::New());
	InitEngineSource(engSource);
	
	pSources.Visit([&](seSource &source){
		const deSynthesizerSource::Ref subEngSource(source.CreateEngineSource());
		engSource->AddSource(subEngSource);
		source.SetEngineSource(subEngSource);
	});
	
	engSource->SetApplicationType(pApplicationType);
	
	seSynthesizer *synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	pTargetSelect.UpdateEngineTarget(*synthesizer, engSource->GetTargetSelect());
	
	return engSource;
}



seSource::Ref seSourceGroup::CreateCopy() const{
	return seSourceGroup::Ref::New(*this);
}

void seSourceGroup::ListLinks(seLink::List &list){
	seSource::ListLinks(list);
	
	pTargetSelect.AddLinksToList(list);
	
	pSources.Visit([&](seSource &source){
		source.ListLinks(list);
	});
}



void seSourceGroup::SetTreeListExpanded(bool expanded){
	pTreeListExpanded = expanded;
}

void seSourceGroup::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	
	pSources.Visit([&](seSource &source){
		source.SynthesizerChanged();
	});
}

void seSourceGroup::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	
	pSources.Visit([&](seSource &source){
		source.SynthesizerDirectoryChanged();
	});
}



// Operators
//////////////

seSourceGroup &seSourceGroup::operator=(const seSourceGroup &copy){
	SetApplicationType(copy.pApplicationType);
	pTargetSelect = copy.pTargetSelect;
	
	RemoveAllSources();
	
	copy.pSources.Visit([&](const seSource &s){
		const seSource::Ref source(s.CreateCopy());
		AddSource(source);
		source->SetParentGroup(this);
	});
	
	SetTreeListExpanded(copy.pTreeListExpanded);
	
	seSource::operator=(copy);
	return *this;
}



// Private Functions
//////////////////////

void seSourceGroup::pCleanUp(){
	RemoveAllSources();
}
