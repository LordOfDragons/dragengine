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

#include "aeRule.h"
#include "aeRuleList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeRule
/////////////////

// Constructor, destructor
////////////////////////////

aeRuleList::aeRuleList(){
}

aeRuleList::aeRuleList( const aeRuleList &copy ) :
pRules( copy.pRules ){
}

aeRuleList::~aeRuleList(){
}



// Management
///////////////

int aeRuleList::GetCount() const{
	return pRules.GetCount();
}

aeRule *aeRuleList::GetAt( int index ) const{
	return ( aeRule* )pRules.GetAt( index );
}

int aeRuleList::IndexOf( aeRule *rule ) const{
	return pRules.IndexOf( rule );
}

bool aeRuleList::Has( aeRule *rule ) const{
	return pRules.Has( rule );
}

void aeRuleList::Add( aeRule *rule ){
	if( Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	pRules.Add( rule );
}

void aeRuleList::Insert( aeRule *rule, int index ){
	if( Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	pRules.Insert( rule, index );
}

void aeRuleList::Move( aeRule *rule, int index ){
	pRules.Move( rule, index );
}

void aeRuleList::Remove( aeRule *rule ){
	pRules.Remove( rule );
}

void aeRuleList::RemoveAll(){
	pRules.RemoveAll();
}



// Operators
//////////////

aeRuleList &aeRuleList::operator=( const aeRuleList &list ){
	pRules = list.pRules;
	return *this;
}
