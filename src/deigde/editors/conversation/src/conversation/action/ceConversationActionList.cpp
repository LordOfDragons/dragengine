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

#include "ceConversationAction.h"
#include "ceConversationActionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationAction
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActionList::ceConversationActionList(){
}

ceConversationActionList::~ceConversationActionList(){
}



// Management
///////////////

int ceConversationActionList::GetCount() const{
	return pActions.GetCount();
}

ceConversationAction *ceConversationActionList::GetAt( int index ) const{
	return ( ceConversationAction* )pActions.GetAt( index );
}

int ceConversationActionList::IndexOf( ceConversationAction *action ) const{
	return pActions.IndexOf( action );
}

bool ceConversationActionList::Has( ceConversationAction *action ) const{
	return pActions.Has( action );
}

void ceConversationActionList::Add( ceConversationAction *action ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Add( action );
}

void ceConversationActionList::InsertAt( ceConversationAction *action, int index ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Insert( action, index );
}

void ceConversationActionList::MoveTo( ceConversationAction *action, int index ){
	pActions.Move( action, index );
}

void ceConversationActionList::Remove( ceConversationAction *action ){
	pActions.Remove( action );
}

void ceConversationActionList::RemoveAll(){
	pActions.RemoveAll();
}



ceConversationActionList &ceConversationActionList::operator=( const ceConversationActionList &list ){
	pActions = list.pActions;
	return *this;
}
