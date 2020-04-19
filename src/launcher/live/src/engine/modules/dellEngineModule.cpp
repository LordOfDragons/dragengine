/* 
 * Drag[en]gine Live Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dellEngineModule.h"
#include "parameter/dellEMParameter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deModuleParameter.h>



// Class dellEngineModule
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEngineModule::dellEngineModule(){
	pType = 0;
	pIsFallback = false;
	pStatus = emsNotTested;
	pErrorCode = 0;
	
	pLibFileSizeShould = 0;
	pLibFileSizeIs = 0;
	
	pLoadableModule = NULL;
}

dellEngineModule::~dellEngineModule(){
}



// Management
///////////////

void dellEngineModule::SetType( int type ){
	if( type < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pType = type;
}

void dellEngineModule::SetName( const char *name ){
	pName = name;
}

void dellEngineModule::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void dellEngineModule::SetAuthor( const decUnicodeString &author ){
	pAuthor = author;
}

void dellEngineModule::SetVersion( const char *version ){
	pVersion = version;
}

void dellEngineModule::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void dellEngineModule::SetIsFallback( bool isFallback ){
	pIsFallback = isFallback;
}

void dellEngineModule::SetStatus( int status ){
	pStatus = status;
}

void dellEngineModule::SetErrorCode( int errorCode ){
	pErrorCode = errorCode;
}



void dellEngineModule::SetLibFileName( const char *name ){
	pLibFileName = name;
}

void dellEngineModule::SetLibFileSizeShould( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pLibFileSizeShould = size;
}

void dellEngineModule::SetLibFileSizeIs( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pLibFileSizeIs = size;
}

void dellEngineModule::SetLibFileHashShould( const char *hash ){
	pLibFileHashShould = hash;
}

void dellEngineModule::SetLibFileHashIs( const char *hash ){
	pLibFileHashIs = hash;
}

void dellEngineModule::SetLibFileEntryPoint( const char *name ){
	pLibFileEntryPoint = name;
}



void dellEngineModule::SetLoadableModule( deLoadableModule *loadableModule ){
	pLoadableModule = loadableModule;
}



void dellEngineModule::GetParamListFromEngineModule(){
	if( pLoadableModule ){
		deBaseModule *baseModule = pLoadableModule->GetModule();
		
		if( baseModule ){
			const int paramCount = baseModule->GetParameterCount();
			dellEMParameter *parameter = NULL;
			deModuleParameter moduleParameter;
			int p;
			
			for( p=0; p<paramCount; p++ ){
				baseModule->GetParameterInfo( p, moduleParameter );
				
				try{
					parameter = new dellEMParameter;
					parameter->SetIndex( p );
					parameter->SetName( moduleParameter.GetName() );
					parameter->SetDescription( moduleParameter.GetDescription() );
					parameter->SetValue( baseModule->GetParameterValue( moduleParameter.GetName() ) );
					
					pParameterList.Add( parameter );
					parameter->FreeReference();
					parameter = NULL;
					
				}catch( const deException &e ){
					if( parameter ){
						parameter->FreeReference();
					}
					throw;
				}
			}
		}
	}
}
