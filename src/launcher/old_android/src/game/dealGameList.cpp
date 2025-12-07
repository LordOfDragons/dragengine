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

#include "dealGame.h"
#include "dealGameList.h"

#include "../common/exceptions.h"



// Class dealGameList
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameList::dealGameList(){
}

dealGameList::~dealGameList(){
	RemoveAll();
}



// Management
///////////////

int dealGameList::GetCount() const{
	return pGames.GetCount();
}

dealGame *dealGameList::GetAt(int index) const{
	return (dealGame*)pGames.GetAt(index);
}

dealGame *dealGameList::GetWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for(i=0; i<count; i++){
		game = (dealGame*)pGames.GetAt(i);
		
		if(game->GetIdentifier().Equals(id)){
			return game;
		}
	}
	
	return NULL;
}

bool dealGameList::Has(dealGame *game) const{
	return pGames.Has(game);
}

bool dealGameList::HasWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for(i=0; i<count; i++){
		game = (dealGame*)pGames.GetAt(i);
		
		if(game->GetIdentifier().Equals(id)){
			return true;
		}
	}
	
	return false;
}

int dealGameList::IndexOf(dealGame *game) const{
	return pGames.IndexOf(game);
}

int dealGameList::IndexOfWithID(const char *id) const{
	if(! id){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pGames.GetCount();
	dealGame *game;
	int i;
	
	for(i=0; i<count; i++){
		game = (dealGame*)pGames.GetAt(i);
		
		if(game->GetIdentifier().Equals(id)){
			return i;
		}
	}
	
	return -1;
}

void dealGameList::Add(dealGame *game){
	if(! game || HasWithID(game->GetIdentifier().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pGames.Add(game);
}

void dealGameList::Remove(dealGame *game){
	const int index = IndexOf(game);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pGames.RemoveFrom(index);
}

void dealGameList::RemoveAll(){
	pGames.RemoveAll();
}
