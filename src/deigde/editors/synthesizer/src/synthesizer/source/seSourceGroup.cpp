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
	const int sourceCount = copy.pSources.GetCount();
	seSource *source = NULL;
	int i;
	
	try{
		for(i=0; i<sourceCount; i++){
			source = copy.pSources.GetAt(i)->CreateCopy();
			pSources.Add(source);
			source->SetParentGroup(this);
			source->FreeReference();
			source = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
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
	
	source->SetParentGroup(NULL);
	source->SetSynthesizer(NULL);
	
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
		source->SetParentGroup(NULL);
		source->SetSynthesizer(NULL);
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
		((deSynthesizerSourceGroup*)GetEngineSource())->SetApplicationType(type);
		NotifySourceChanged();
	}
}



void seSourceGroup::UpdateTargets(){
	seSource::UpdateTargets();
	
	deSynthesizerSourceGroup * const source = (deSynthesizerSourceGroup*)GetEngineSource();
	if(source){
		pTargetSelect.UpdateEngineTarget(GetSynthesizer(), source->GetTargetSelect());
	}
	
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSource*)pSources.GetAt(i))->UpdateTargets();
	}
}

int seSourceGroup::CountLinkUsage(seLink *link) const{
	int i, usageCount = seSource::CountLinkUsage(link);
	const int count = pSources.GetCount();
	
	if(pTargetSelect.HasLink(link)){
		usageCount++;
	}
	
	for(i=0; i<count; i++){
		usageCount += pSources.GetAt(i)->CountLinkUsage(link);
	}
	
	return usageCount;
}

void seSourceGroup::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	if(pTargetSelect.HasLink(link)){
		pTargetSelect.RemoveLink(link);
	}
	
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->RemoveLinkFromTargets(link);
	}
	
	seSource::UpdateTargets();
}

void seSourceGroup::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSelect.RemoveAllLinks();
	
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->RemoveLinksFromAllTargets();
	}
	
	seSource::UpdateTargets();
}



deSynthesizerSource *seSourceGroup::CreateEngineSource(){
	deSynthesizerSourceGroup *engSource = NULL;
	deSynthesizerSource *subEngSource = NULL;
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SetEngineSource(NULL);
	}
	
	try{
		engSource = new deSynthesizerSourceGroup;
		
		InitEngineSource(engSource);
		
		for(i=0; i<count; i++){
			seSource * const source = pSources.GetAt(i);
			
			subEngSource = source->CreateEngineSource();
			engSource->AddSource(subEngSource);
			source->SetEngineSource(subEngSource);
			subEngSource->FreeReference();
			subEngSource = NULL;
		}
		
		engSource->SetApplicationType(pApplicationType);
		
		pTargetSelect.UpdateEngineTarget(GetSynthesizer(), engSource->GetTargetSelect());
		
	}catch(const deException &){
		throw;
	}
	
	// finished
	return engSource;
}



seSource *seSourceGroup::CreateCopy() const{
	return new seSourceGroup(*this);
}

void seSourceGroup::ListLinks(seLinkList &list){
	const int count = pSources.GetCount();
	int i;
	
	seSource::ListLinks(list);
	
	pTargetSelect.AddLinksToList(list);
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->ListLinks(list);
	}
}



void seSourceGroup::SetTreeListExpanded(bool expanded){
	pTreeListExpanded = expanded;
}

void seSourceGroup::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SynthesizerChanged();
	}
}

void seSourceGroup::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SynthesizerDirectoryChanged();
	}
}



// Operators
//////////////

seSourceGroup &seSourceGroup::operator=(const seSourceGroup &copy){
	SetApplicationType(copy.pApplicationType);
	pTargetSelect = copy.pTargetSelect;
	
	const int sourceCount = copy.pSources.GetCount();
	seSource *source = NULL;
	int i;
	
	RemoveAllSources();
	try{
		for(i=0; i<sourceCount; i++){
			source = copy.pSources.GetAt(i)->CreateCopy();
			AddSource(source);
			source->SetParentGroup(this);
			source->FreeReference();
			source = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
	
	SetTreeListExpanded(copy.pTreeListExpanded);
	
	seSource::operator=(copy);
	return *this;
}



// Private Functions
//////////////////////

void seSourceGroup::pCleanUp(){
	RemoveAllSources();
}
