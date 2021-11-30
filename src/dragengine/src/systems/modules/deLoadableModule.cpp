/* 
 * Drag[en]gine Game Engine
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

#include "deLoadableModule.h"
#include "deInternalModule.h"
#include "deLibraryModule.h"
#include "deLoadableModuleVisitor.h"
#include "deBaseModule.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deLoadableModule
///////////////////////////

// Constructor, destructor
////////////////////////////

deLoadableModule::deLoadableModule( deModuleSystem *system ) :
pSystem( system ),
	
pName( "Unnamed" ),
pType( deModuleSystem::emtUnknown ),

pDescription( "No description" ),
pAuthor( "Unknown" ),
pVersion( "1.0" ),
	
pFallback( false ),
pNoSaving( false ),

pEnabled( true ),

pModule( NULL ),
pLockCount( 0 ),

pErrorCode( eecSuccess )
{
	if( ! system ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetDefaultLoggingName();
		
	}catch( const deException & ){
		pLMCleanUp();
		throw;
	}
}

deLoadableModule::~deLoadableModule(){
	pLMCleanUp();
}



// Module management
//////////////////////

void deLoadableModule::SetLoggingName( const char *name ){
	pLoggingName = name;
}

void deLoadableModule::SetDefaultLoggingName(){
	pLoggingName = pName;
}



void deLoadableModule::SetName( const char *name ){
	pName = name;
}

void deLoadableModule::SetType( deModuleSystem::eModuleTypes type ){
	pType = type;
}

void deLoadableModule::SetDescription( const char *description ){
	pDescription = description;
}

void deLoadableModule::SetAuthor( const char *author ){
	pAuthor = author;
}

void deLoadableModule::SetVersion( const char *version ){
	pVersion = version;
}

void deLoadableModule::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}

void deLoadableModule::SetDirectoryName( const char *dirName ){
	pDirName = dirName;
}

void deLoadableModule::SetIsFallback( bool fallback ){
	pFallback = fallback;
}

void deLoadableModule::SetNoSaving( bool noSaving ){
	pNoSaving = noSaving;
}

void deLoadableModule::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



// Module Management
//////////////////////

void deLoadableModule::SetModule( deBaseModule *module ){
	if( pModule ){
		delete pModule;
	}
	pModule = module;
}

bool deLoadableModule::IsLoaded() const{
	return pModule != NULL;
}

void deLoadableModule::SetErrorCode( int code ){
	pErrorCode = code;
}

void deLoadableModule::Lock(){
	pLockCount++;
}

void deLoadableModule::Unlock(){
	if( pLockCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	pLockCount--;
}



// Type Testing and Safe Casting
//////////////////////////////////

bool deLoadableModule::IsInternalModule() const{
	return false;
}

bool deLoadableModule::IsLibraryModule() const{
	return false;
}

deInternalModule *deLoadableModule::CastToInternalModule(){
	DETHROW( deeInvalidParam );
}

deLibraryModule *deLoadableModule::CastToLibraryModule(){
	DETHROW( deeInvalidParam );
}



// visiting
/////////////

void deLoadableModule::Visit( deLoadableModuleVisitor &visitor ){
	visitor.VisitLoadableModule( *this );
}



// Private functions
//////////////////////

void deLoadableModule::pLMCleanUp(){
	if( pModule ){
		delete pModule;
	}
}
