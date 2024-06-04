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

#include "ceUndoCAction.h"
#include "ceUndoCActionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceUndoCAction
////////////////////////

// Constructor, destructor
////////////////////////////

ceUndoCActionList::ceUndoCActionList(){
}

ceUndoCActionList::~ceUndoCActionList(){
}



// Management
///////////////

int ceUndoCActionList::GetCount() const{
	return pActions.GetCount();
}

ceUndoCAction *ceUndoCActionList::GetAt( int index ) const{
	return ( ceUndoCAction* )pActions.GetAt( index );
}

int ceUndoCActionList::IndexOf( ceUndoCAction *action ) const{
	return pActions.IndexOf( action );
}

bool ceUndoCActionList::Has( ceUndoCAction *action ) const{
	return pActions.Has( action );
}

void ceUndoCActionList::Add( ceUndoCAction *action ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Add( action );
}

void ceUndoCActionList::Remove( ceUndoCAction *action ){
	pActions.Remove( action );
}

void ceUndoCActionList::RemoveAll(){
	pActions.RemoveAll();
}



ceUndoCActionList &ceUndoCActionList::operator=( const ceUndoCActionList &list ){
	pActions = list.pActions;
	return *this;
}
