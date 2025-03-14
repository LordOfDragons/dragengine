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

#include "dealGPMParameter.h"
#include "dealGPMParameterList.h"

#include "../../common/exceptions.h"



// Class dealGPMParameterList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPMParameterList::dealGPMParameterList(){
}

dealGPMParameterList::~dealGPMParameterList(){
	RemoveAllParameters();
}



// Management
///////////////

int dealGPMParameterList::GetParameterCount() const{
	return pParameters.GetCount();
}

dealGPMParameter *dealGPMParameterList::GetParameterAt( int index ) const{
	return ( dealGPMParameter* )pParameters.GetAt( index );
}

dealGPMParameter *dealGPMParameterList::GetParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pParameters.GetCount();
	dealGPMParameter *parameter;
	
	for( i=0; i<count; i++ ){
		parameter = ( dealGPMParameter* )pParameters.GetAt( i );
		if( parameter->GetName().Equals( name ) ){
			return parameter;
		}
	}
	
	return NULL;
}

bool dealGPMParameterList::HasParameter( dealGPMParameter *parameter ) const{
	return pParameters.Has( parameter );
}

bool dealGPMParameterList::HasParameterNamed( const char *name ) const{
	return GetParameterNamed( name ) != NULL;
}

int dealGPMParameterList::IndexOfParameter( dealGPMParameter *parameter ) const{
	return pParameters.IndexOf( parameter );
}

int dealGPMParameterList::IndexOfParameterNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pParameters.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( dealGPMParameter* )pParameters.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dealGPMParameterList::AddParameter( dealGPMParameter *parameter ){
	if( ! parameter || HasParameterNamed( parameter->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	pParameters.Add( parameter );
}

void dealGPMParameterList::RemoveParameter( dealGPMParameter *parameter ){
	const int index = IndexOfParameter( parameter );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.RemoveFrom( index );
}

void dealGPMParameterList::RemoveAllParameters(){
	pParameters.RemoveAll();
}
