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

#include "deMCCommon.h"

#include <dragengine/common/math/decMath.h>


// Class deMCCommon
/////////////////////

uint32_t deMCCommon::UInt32( const deServiceObject &so ){
	return UInt32( so.GetString() );
}

deServiceObject::Ref deMCCommon::UInt32( uint32_t value ){
	return deServiceObject::NewString( UInt32ToString( value ) );
}

uint32_t deMCCommon::UInt32( const decString &string ){
	return ( uint32_t )string.ToLongValid();
}

decString deMCCommon::UInt32ToString( uint32_t value ){
	decString string;
	string.AppendValue( ( unsigned long long )value );
	return string;
}

uint64_t deMCCommon::UInt64( const deServiceObject &so ){
	return UInt64( so.GetString() );
}

deServiceObject::Ref deMCCommon::UInt64( uint64_t value ){
	return deServiceObject::NewString( UInt64ToString( value ) );
}

uint64_t deMCCommon::UInt64( const decString &string ){
	return ( uint64_t )string.ToLongValid();
}

decString deMCCommon::UInt64ToString( uint64_t value ){
	decString string;
	string.AppendValue( ( long long )value );
	return string;
}

decStringList deMCCommon::StringList(const deServiceObject &so){
	const int count = so.GetChildCount();
	decStringList list;
	int i;
	
	for( i=0; i<count; i++ ){
		list.Add(so.GetChildAt( i )->GetString());
	}
	
	return list;
}

deServiceObject::Ref deMCCommon::StringList( const decStringList &list ){
	const deServiceObject::Ref so(deServiceObject::NewList());
	
	const int count = list.GetCount();
	int i;
	for(i=0; i<count; i++){
		so->AddStringChild(list.GetAt(i));
	}
	
	return so;
}
