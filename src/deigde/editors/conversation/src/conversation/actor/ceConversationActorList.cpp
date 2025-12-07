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

#include "ceConversationActor.h"
#include "ceConversationActorList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationActor
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActorList::ceConversationActorList(){
}

ceConversationActorList::~ceConversationActorList(){
}



// Management
///////////////

int ceConversationActorList::GetCount() const{
	return pActors.GetCount();
}

ceConversationActor *ceConversationActorList::GetAt(int index) const{
	return (ceConversationActor*)pActors.GetAt(index);
}

ceConversationActor *ceConversationActorList::GetWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	ceConversationActor *actor;
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		actor = (ceConversationActor*)pActors.GetAt(i);
		
		if(actor->GetID().Equals(id)){
			return actor;
		}
	}
	
	return NULL;
}

ceConversationActor *ceConversationActorList::GetWithAliasID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	ceConversationActor *actor;
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		actor = (ceConversationActor*)pActors.GetAt(i);
		
		if(! actor->GetAliasID().IsEmpty() && actor->GetAliasID().Equals(id)){
			return actor;
		}
	}
	
	return NULL;
}

ceConversationActor *ceConversationActorList::GetWithIDOrAliasID(const char *id) const{
	ceConversationActor *actor = GetWithAliasID(id);
	
	if(! actor){
		actor = GetWithID(id);
	}
	
	return actor;
}

int ceConversationActorList::IndexOf(ceConversationActor *actor) const{
	return pActors.IndexOf(actor);
}

int ceConversationActorList::IndexWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ceConversationActor &actor = *((ceConversationActor*)pActors.GetAt(i));
		
		if(actor.GetID().Equals(id)){
			return i;
		}
	}
	
	return -1;
}

int ceConversationActorList::IndexWithAliasID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ceConversationActor &actor = *((ceConversationActor*)pActors.GetAt(i));
		
		if(! actor.GetAliasID().IsEmpty() && actor.GetAliasID().Equals(id)){
			return i;
		}
	}
	
	return -1;
}

int ceConversationActorList::IndexWithIDOrAliasID(const char *id) const{
	int index = IndexWithAliasID(id);
	
	if(index == -1){
		index = IndexWithID(id);
	}
	
	return index;
}

bool ceConversationActorList::Has(ceConversationActor *actor) const{
	return pActors.Has(actor);
}

bool ceConversationActorList::HasWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ceConversationActor &actor = *((ceConversationActor*)pActors.GetAt(i));
		
		if(actor.GetID().Equals(id)){
			return true;
		}
	}
	
	return false;
}

bool ceConversationActorList::HasWithAliasID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ceConversationActor &actor = *((ceConversationActor*)pActors.GetAt(i));
		
		if(! actor.GetAliasID().IsEmpty() && actor.GetAliasID().Equals(id)){
			return true;
		}
	}
	
	return false;
}

bool ceConversationActorList::HasWithIDOrAliasID(const char *id) const{
	return HasWithAliasID(id) || HasWithID(id);
}

void ceConversationActorList::Add(ceConversationActor *actor){
	if(! actor) DETHROW(deeInvalidParam);
	
	pActors.Add(actor);
}

void ceConversationActorList::InsertAt(ceConversationActor *actor, int index){
	if(! actor) DETHROW(deeInvalidParam);
	
	pActors.Insert(actor, index);
}

void ceConversationActorList::MoveTo(ceConversationActor *actor, int index){
	pActors.Move(actor, index);
}

void ceConversationActorList::Remove(ceConversationActor *actor){
	pActors.Remove(actor);
}

void ceConversationActorList::RemoveAll(){
	pActors.RemoveAll();
}



ceConversationActorList &ceConversationActorList::operator=(const ceConversationActorList &list){
	pActors = list.pActors;
	return *this;
}
