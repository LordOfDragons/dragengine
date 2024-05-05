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
#include <string.h>
#include <stdlib.h>

#include "deoglCubeMap.h"
#include "deoglRenderableColorCubeMap.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorCubeMap
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorCubeMap::deoglRenderableColorCubeMap( deoglRenderThread &renderThread,
int size, int componentCount, bool isFloat ) :
pSize( size ),
pComponentCount( componentCount ),
pIsFloat( isFloat ),
pInUse( false ),
pCubeMap( NULL ),
pMemoryUsageGPU( 0 )
{
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pCubeMap = new deoglCubeMap( renderThread );
		pCubeMap->SetSize( size );
		pCubeMap->SetFBOFormat( componentCount, isFloat );
		pCubeMap->CreateCubeMap();
		UpdateMemoryUsage();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderableColorCubeMap::~deoglRenderableColorCubeMap(){
	pCleanUp();
	
	//pRenderThread.GetMemoryManager().IncRenderableCubeColorGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableColorCubeMap::Matches( int size, int componentCount, bool isFloat ) const{
	return ( size == pSize ) && ( componentCount == pComponentCount ) && ( isFloat == pIsFloat );
}

void deoglRenderableColorCubeMap::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableColorCubeMap::UpdateMemoryUsage(){
	//const int oldMemoryGPU = pMemoryUsageGPU;
	
	if( pCubeMap ){
		pMemoryUsageGPU = pSize * pSize * 6 * pComponentCount;
		
		if( pIsFloat ){
			pMemoryUsageGPU *= 2;
		}
		
	}else{
		pMemoryUsageGPU = 0;
	}
	
	//pRenderThread.GetMemoryManager().IncRenderableCubeColorGPUTotal( pMemoryUsageGPU - oldMemoryGPU );
}



// private functions
//////////////////////

void deoglRenderableColorCubeMap::pCleanUp(){
	if( pCubeMap ){
		delete pCubeMap;
	}
}
