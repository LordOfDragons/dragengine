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

#include "syneSourceGroup.h"
#include "../syneSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Class syneSourceGroup
////////////////////////

// Constructor, destructor
////////////////////////////

syneSourceGroup::syneSourceGroup() :
syneSource(deSynthesizerSourceVisitorIdentify::estGroup),
pApplicationType(deSynthesizerSourceGroup::eatAll),
pTargetSelect(syneControllerTarget::Ref::New()),
pTreeListExpanded(false){
}

syneSourceGroup::syneSourceGroup(const syneSourceGroup &copy) :
syneSource(copy),
pApplicationType(copy.pApplicationType),
pTargetSelect(syneControllerTarget::Ref::New(copy.pTargetSelect)),
pTreeListExpanded(copy.pTreeListExpanded)
{
	copy.pSources.Visit([&](const syneSource &s){
		const syneSource::Ref source(s.CreateCopy());
		pSources.Add(source);
		source->SetParentGroup(this);
	});
}

syneSourceGroup::~syneSourceGroup(){
	pCleanUp();
}



// Management
///////////////

void syneSourceGroup::AddSource(syneSource *source){
	pSources.Add(source);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	
	source->SetParentGroup(this);
	source->SetSynthesizer(synthesizer);
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void syneSourceGroup::InsertSourceAt(syneSource *source, int index){
	pSources.Insert(source, index);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	
	source->SetParentGroup(this);
	source->SetSynthesizer(synthesizer);
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void syneSourceGroup::MoveSourceTo(syneSource *source, int index){
	pSources.Move(source, index);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}

void syneSourceGroup::RemoveSource(syneSource *source){
	if(!pSources.Has(source)){
		DETHROW(deeInvalidParam);
	}
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	
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

void syneSourceGroup::RemoveAllSources(){
	const int count = pSources.GetCount();
	int i;
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	
	if(synthesizer && pSources.Has(synthesizer->GetActiveSource())){
		synthesizer->SetActiveSource(this);
	}
	
	for(i=0; i<count; i++){
		syneSource * const source = pSources.GetAt(i);
		source->SetParentGroup(nullptr);
		source->SetSynthesizer(nullptr);
	}
	
	pSources.RemoveAll();
	
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifySourceStructureChanged();
	}
}



void syneSourceGroup::SetApplicationType(deSynthesizerSourceGroup::eApplicationTypes type){
	if(type == pApplicationType){
		return;
	}
	
	pApplicationType = type;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceGroup>()->SetApplicationType(type);
		NotifySourceChanged();
	}
}



void syneSourceGroup::UpdateTargets(){
	syneSource::UpdateTargets();
	
	deSynthesizerSourceGroup * const source = GetEngineSource().DynamicCast<deSynthesizerSourceGroup>();
	if(source){
		syneSynthesizer *synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer)
		
		pTargetSelect->UpdateEngineTarget(*synthesizer, source->GetTargetSelect());
	}
	
	pSources.Visit([](syneSource &s){
		s.UpdateTargets();
	});
}

int syneSourceGroup::CountLinkUsage(syneLink *link) const{
	int usageCount = syneSource::CountLinkUsage(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	
	pSources.Visit([&](const syneSource &source){
		usageCount += source.CountLinkUsage(link);
	});
	
	return usageCount;
}

void syneSourceGroup::RemoveLinkFromTargets(syneLink *link){
	syneSource::RemoveLinkFromTargets(link);
	
	pTargetSelect->RemoveLink(link);
	
	pSources.Visit([&](syneSource &source){
		source.RemoveLinkFromTargets(link);
	});
	
	syneSource::UpdateTargets();
}

void syneSourceGroup::RemoveLinksFromAllTargets(){
	syneSource::RemoveLinksFromAllTargets();
	
	pTargetSelect->RemoveAllLinks();
	
	pSources.Visit([](syneSource &source){
		source.RemoveLinksFromAllTargets();
	});
	
	syneSource::UpdateTargets();
}



deSynthesizerSource::Ref syneSourceGroup::CreateEngineSource(){
	pSources.Visit([&](syneSource &source){
		source.SetEngineSource(nullptr);
	});
	
	const deSynthesizerSourceGroup::Ref engSource(deSynthesizerSourceGroup::Ref::New());
	InitEngineSource(engSource);
	
	pSources.Visit([&](syneSource &source){
		const deSynthesizerSource::Ref subEngSource(source.CreateEngineSource());
		engSource->AddSource(subEngSource);
		source.SetEngineSource(subEngSource);
	});
	
	engSource->SetApplicationType(pApplicationType);
	
	syneSynthesizer *synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	pTargetSelect->UpdateEngineTarget(*synthesizer, engSource->GetTargetSelect());
	
	return engSource;
}



syneSource::Ref syneSourceGroup::CreateCopy() const{
	return syneSourceGroup::Ref::New(*this);
}

void syneSourceGroup::ListLinks(syneLink::List &list){
	syneSource::ListLinks(list);
	
	pTargetSelect->AddLinksToList(list);
	
	pSources.Visit([&](syneSource &source){
		source.ListLinks(list);
	});
}



void syneSourceGroup::SetTreeListExpanded(bool expanded){
	pTreeListExpanded = expanded;
}

void syneSourceGroup::SynthesizerChanged(){
	syneSource::SynthesizerChanged();
	
	pSources.Visit([&](syneSource &source){
		source.SynthesizerChanged();
	});
}

void syneSourceGroup::SynthesizerDirectoryChanged(){
	syneSource::SynthesizerDirectoryChanged();
	
	pSources.Visit([&](syneSource &source){
		source.SynthesizerDirectoryChanged();
	});
}



// Operators
//////////////

syneSourceGroup &syneSourceGroup::operator=(const syneSourceGroup &copy){
	SetApplicationType(copy.pApplicationType);
	pTargetSelect = copy.pTargetSelect;
	
	RemoveAllSources();
	
	copy.pSources.Visit([&](const syneSource &s){
		const syneSource::Ref source(s.CreateCopy());
		AddSource(source);
		source->SetParentGroup(this);
	});
	
	SetTreeListExpanded(copy.pTreeListExpanded);
	
	syneSource::operator=(copy);
	return *this;
}



// Private Functions
//////////////////////

void syneSourceGroup::pCleanUp(){
	RemoveAllSources();
}
