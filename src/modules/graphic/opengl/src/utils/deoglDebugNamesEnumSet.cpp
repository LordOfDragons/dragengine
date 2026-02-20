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

#include "deoglDebugNamesEnumSet.h"


// Class deoglDebugNamesEnumSet
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugNamesEnumSet::deoglDebugNamesEnumSet(const char *name, const sEntry *entries) :
deoglDebugNamesEnum(name, entries){
}



// Management
///////////////

decString deoglDebugNamesEnumSet::SetName(int value, bool full) const{
	decString name;
	if(full){
		name = pName;
	}
	name.AppendCharacter('{');
	
	const char *separator = "";
	const sEntry *entry = pEntries;
	
	while(entry->name){
		if(value & entry->value){
			name.AppendFormat("%s%s", separator, entry->name);
			separator = ", ";
			value &= ~entry->value;
		}
		entry++;
	}
	
	int i;
	for(i=0; i<31; i++){
		if(value & (1 << i)){
			name.AppendFormat("%s%x", separator, 1 << i);
			separator = ", ";
		}
	}
	
	name.AppendCharacter('}');
	return name;
}
