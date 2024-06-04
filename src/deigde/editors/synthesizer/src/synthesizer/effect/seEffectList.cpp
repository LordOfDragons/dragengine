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

#include "seEffect.h"
#include "seEffectList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seEffect
///////////////////

// Constructor, destructor
////////////////////////////

seEffectList::seEffectList(){
}

seEffectList::seEffectList( const seEffectList &copy ) :
pEffects( copy.pEffects ){
}

seEffectList::~seEffectList(){
}



// Management
///////////////

int seEffectList::GetCount() const{
	return pEffects.GetCount();
}

seEffect *seEffectList::GetAt( int index ) const{
	return ( seEffect* )pEffects.GetAt( index );
}

int seEffectList::IndexOf( seEffect *effect ) const{
	return pEffects.IndexOf( effect );
}

bool seEffectList::Has( seEffect *effect ) const{
	return pEffects.Has( effect );
}

void seEffectList::Add( seEffect *effect ){
	if( ! effect || Has( effect ) ){
		DETHROW( deeInvalidParam );
	}
	pEffects.Add( effect );
}

void seEffectList::Insert( seEffect *effect, int index ){
	if( ! effect || Has( effect ) ){
		DETHROW( deeInvalidParam );
	}
	pEffects.Insert( effect, index );
}

void seEffectList::Move( seEffect *effect, int index ){
	pEffects.Move( effect, index );
}

void seEffectList::Remove( seEffect *effect ){
	pEffects.Remove( effect );
}

void seEffectList::RemoveAll(){
	pEffects.RemoveAll();
}



// Operators
//////////////

seEffectList &seEffectList::operator=( const seEffectList &list ){
	pEffects = list.pEffects;
	return *this;
}

seEffectList &seEffectList::operator+=( const seEffectList &list ){
	pEffects += list.pEffects;
	return *this;
}
