/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskOwner.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskPool
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskPool::deoglPersistentRenderTaskPool() :
pOwners( 20000 ),
pShaders( 100 ),
pTextures( 200 ),
pVAOs( 2000 ),
pInstances( 5000 )
{
	try{
		pCreateInitial();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPersistentRenderTaskPool::~deoglPersistentRenderTaskPool(){
	pCleanUp();
}



// Management
///////////////

deoglPersistentRenderTaskOwner *deoglPersistentRenderTaskPool::GetOwner(){
	deoglPersistentRenderTaskOwner *owner;
	
	const int index = pOwners.GetCount() - 1;
	if( index > -1 ){
		owner = ( deoglPersistentRenderTaskOwner* )pOwners.GetAt( index );
		pOwners.RemoveFrom( index );
		
	}else{
		owner = new deoglPersistentRenderTaskOwner;
	}
	
	return owner;
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTaskPool::GetShader(){
	deoglPersistentRenderTaskShader *shader;
	
	const int index = pShaders.GetCount() - 1;
	if( index > -1 ){
		shader = ( deoglPersistentRenderTaskShader* )pShaders.GetAt( index );
		pShaders.RemoveFrom( index );
		
	}else{
		shader = new deoglPersistentRenderTaskShader( *this );
	}
	
	return shader;
}

deoglPersistentRenderTaskTexture *deoglPersistentRenderTaskPool::GetTexture(){
	deoglPersistentRenderTaskTexture *texture;
	
	const int index = pTextures.GetCount() - 1;
	if( index > -1 ){
		texture = ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( index );
		pTextures.RemoveFrom( index );
		
	}else{
		texture = new deoglPersistentRenderTaskTexture( *this );
	}
	
	return texture;
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskPool::GetVAO(){
	deoglPersistentRenderTaskVAO *vao;
	
	const int index = pVAOs.GetCount() - 1;
	if( index > -1 ){
		vao = ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( index );
		pVAOs.RemoveFrom( index );
		
	}else{
		vao = new deoglPersistentRenderTaskVAO( *this );
	}
	
	return vao;
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskPool::GetInstance(){
	deoglPersistentRenderTaskInstance *instance;
	
	const int index = pInstances.GetCount() - 1;
	if( index > -1 ){
		instance = ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( index );
		pInstances.RemoveFrom( index );
		
	}else{
		instance = new deoglPersistentRenderTaskInstance( *this );
	}
	
	return instance;
}



void deoglPersistentRenderTaskPool::ReturnOwner( deoglPersistentRenderTaskOwner *owner ){
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
	
	owner->Clear();
	pOwners.Add( owner );
}

void deoglPersistentRenderTaskPool::ReturnShader( deoglPersistentRenderTaskShader *shader ){
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	shader->Clear();
	pShaders.Add( shader );
}

void deoglPersistentRenderTaskPool::ReturnTexture( deoglPersistentRenderTaskTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	texture->Clear();
	pTextures.Add( texture );
}

void deoglPersistentRenderTaskPool::ReturnVAO( deoglPersistentRenderTaskVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	vao->Clear();
	pVAOs.Add( vao );
}

void deoglPersistentRenderTaskPool::ReturnInstance( deoglPersistentRenderTaskInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	instance->Clear();
	pInstances.Add( instance );
}



// Private Functions
//////////////////////

void deoglPersistentRenderTaskPool::pCleanUp(){
	int i, count = pInstances.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i );
	}
	
	count = pVAOs.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i );
	}
	
	count = pTextures.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i );
	}
	
	count = pShaders.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i );
	}
	
	count = pOwners.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskOwner* )pOwners.GetAt( i );
	}
}

void deoglPersistentRenderTaskPool::pCreateInitial(){
	// create some initial elements. this ensures the typical amount of elements created
	// are located close together in memory to improve performance.
	// 
	// typical values for sky shadow render plan:
	// - owners: 8000
	// - shaders: 2
	// - textures: 20
	// - vaos: 600
	// - instances: 2500
	// - subinstances: 50000
	// 
	// camera render plans are similar but have less since they can cull better.
	// we allocate here elements for roughly 2 sky shadow maps. that should be
	// a good starting value.
	// 
	// the arrays use the amounts also as capacities in the constructors. this allocates
	// the memory ahead of time so it is faster to build the initial elements
	int i;
	
	for( i=0; i<20000; i++ ){
		pOwners.Add( new deoglPersistentRenderTaskOwner );
	}
	
	for( i=0; i<100; i++ ){
		pShaders.Add( new deoglPersistentRenderTaskShader( *this ) );
	}
	
	for( i=0; i<200; i++ ){
		pTextures.Add( new deoglPersistentRenderTaskTexture( *this ) );
	}
	
	for( i=0; i<2000; i++ ){
		pVAOs.Add( new deoglPersistentRenderTaskVAO( *this ) );
	}
	
	for( i=0; i<5000; i++ ){
		pInstances.Add( new deoglPersistentRenderTaskInstance( *this ) );
	}
}
