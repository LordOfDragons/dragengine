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

#include "delGame.h"
#include "delGameList.h"

#include <dragengine/common/exceptions.h>



// Class delGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameList::delGameList(){
}

delGameList::~delGameList(){
	RemoveAll();
}



// Management
///////////////

int delGameList::GetCount() const{
	return pGames.GetCount();
}

delGame *delGameList::GetAt(int index) const{
	return (delGame*)pGames.GetAt(index);
}

delGame *delGameList::GetWithID(const decUuid &id) const{
	if(! id){
		DETHROW_INFO(deeInvalidParam, "id is 0 uuid");
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delGame * const game = (delGame*)pGames.GetAt(i);
		if(game->GetIdentifier() == id){
			return game;
		}
	}
	
	return NULL;
}

delGameList delGameList::GetWithAlias(const char *alias) const{
	const int count = pGames.GetCount();
	delGameList list;
	int i;
	
	for(i=0; i<count; i++){
		delGame * const game = (delGame*)pGames.GetAt(i);
		if(game->GetAliasIdentifier().Equals(alias)){
			list.Add(game);
		}
	}
	
	return list;
}

bool delGameList::Has(delGame *game) const{
	return pGames.Has(game);
}

bool delGameList::HasWithID(const decUuid &id) const{
	if(! id){
		DETHROW_INFO(deeInvalidParam, "id is 0 uuid");
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGame * const game = (delGame*)pGames.GetAt(i);
		if(game->GetIdentifier() == id){
			return true;
		}
	}
	
	return false;
}

int delGameList::IndexOf(delGame *game) const{
	return pGames.IndexOf(game);
}

int delGameList::IndexOfWithID(const decUuid &id) const{
	if(! id){
		DETHROW_INFO(deeInvalidParam, "id is 0 uuid");
	}
	
	const int count = pGames.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGame * const game = (delGame*)pGames.GetAt(i);
		if(game->GetIdentifier() == id){
			return i;
		}
	}
	
	return -1;
}

void delGameList::Add(delGame *game){
	if(! game){
		DETHROW_INFO(deeNullPointer, "game");
	}
	if(HasWithID(game->GetIdentifier())){
		DETHROW(deeInvalidParam);
	}
	
	pGames.Add(game);
}

void delGameList::Remove(delGame *game){
	const int index = IndexOf(game);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "game is absent");
	}
	
	pGames.RemoveFrom(index);
}

void delGameList::RemoveAll(){
	pGames.RemoveAll();
}
