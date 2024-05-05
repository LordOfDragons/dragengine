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

#include "ceConversationCondition.h"
#include "ceConversationConditionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationCondition
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationConditionList::ceConversationConditionList(){
}

ceConversationConditionList::~ceConversationConditionList(){
}



// Management
///////////////

int ceConversationConditionList::GetCount() const{
	return pConditions.GetCount();
}

ceConversationCondition *ceConversationConditionList::GetAt( int index ) const{
	return ( ceConversationCondition* )pConditions.GetAt( index );
}

int ceConversationConditionList::IndexOf( ceConversationCondition *condition ) const{
	return pConditions.IndexOf( condition );
}

bool ceConversationConditionList::Has( ceConversationCondition *condition ) const{
	return pConditions.Has( condition );
}

void ceConversationConditionList::Add( ceConversationCondition *condition ){
	if( ! condition ) DETHROW( deeInvalidParam );
	
	pConditions.Add( condition );
}

void ceConversationConditionList::InsertAt( ceConversationCondition *condition, int index ){
	if( ! condition ) DETHROW( deeInvalidParam );
	
	pConditions.Insert( condition, index );
}

void ceConversationConditionList::MoveTo( ceConversationCondition *condition, int index ){
	pConditions.Move( condition, index );
}

void ceConversationConditionList::Remove( ceConversationCondition *condition ){
	pConditions.Remove( condition );
}

void ceConversationConditionList::RemoveAll(){
	pConditions.RemoveAll();
}



ceConversationConditionList &ceConversationConditionList::operator=( const ceConversationConditionList &list ){
	pConditions = list.pConditions;
	return *this;
}
