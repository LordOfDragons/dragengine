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

#include "delGPModule.h"
#include "delGPModuleList.h"
#include "delGPMParameter.h"

#include <dragengine/common/exceptions.h>



// Class delGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPModuleList::delGPModuleList(){
}

delGPModuleList::~delGPModuleList(){
	RemoveAll();
}



// Management
///////////////

int delGPModuleList::GetCount() const{
	return pModules.GetCount();
}

delGPModule *delGPModuleList::GetAt( int index ) const{
	return ( delGPModule* )pModules.GetAt( index );
}

delGPModule *delGPModuleList::GetNamed ( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGPModule * const module = ( delGPModule* )pModules.GetAt( i );
		if( module->GetName() == name ){
			return module;
		}
	}
	
	return nullptr;
}

bool delGPModuleList::Has( delGPModule *module ) const{
	return pModules.Has( module );
}

bool delGPModuleList::HasNamed( const char *name ) const{
	return GetNamed ( name );
}

int delGPModuleList::IndexOf( delGPModule *module ) const{
	return pModules.IndexOf( module );
}

int delGPModuleList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delGPModule* )pModules.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delGPModuleList::Add( delGPModule *module ){
	if( ! module ){
		DETHROW_INFO( deeNullPointer, "module" );
	}
	if( HasNamed ( module->GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "named module is present" );
	}
	
	pModules.Add( module );
}

void delGPModuleList::Remove ( delGPModule *module ){
	const int index = IndexOf ( module );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "module is absent" );
	}
	
	pModules.RemoveFrom( index );
}

void delGPModuleList::RemoveAll(){
	pModules.RemoveAll();
}

void delGPModuleList::Update( const delGPModuleList &list ){
	const int moduleCount = list.GetCount();
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const delGPModule &moduleChanges = *list.GetAt ( i );
		delGPModule * const module = GetNamed( moduleChanges.GetName() );
		
		if( module ){
			delGPMParameterList &parameters = module->GetParameters();
			const delGPMParameterList &paramsChanges = moduleChanges.GetParameters();
			const int paramCount = paramsChanges.GetCount();
			
			for( j=0; j<paramCount; j++ ){
				const delGPMParameter &paramChanges = *paramsChanges.GetAt ( j );
				delGPMParameter * const parameter = parameters.GetNamed ( paramChanges.GetName() );
				
				if( parameter ){
					parameter->SetValue( paramChanges.GetValue() );
					
				}else{
					parameters.Add ( delGPMParameter::Ref::NewWith(paramChanges) );
				}
			}
			
		}else{
			Add( delGPModule::Ref::NewWith(moduleChanges) );
		}
	}
}
