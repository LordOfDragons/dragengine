/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglTimerQuerySet.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTimerQuerySet
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTimerQuerySet::deoglTimerQuerySet( deoglRenderThread &renderthread ) :
pRenderThread( renderthread ),
pQueries( NULL ),
pQueryCount( 0 ){
}

deoglTimerQuerySet::~deoglTimerQuerySet(){
	if( pQueries ){
		deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
		int i;
		for( i=0; i<pQueryCount; i++ ){
			dops.DeleteOpenGLQuery( pQueries[ i ] );
		}
		
		delete [] pQueries;
		pQueries = NULL;
	}
}



// Management
///////////////

void deoglTimerQuerySet::SetQueryCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pQueryCount ){
		if( pQueryCount > 0 ){
			pglDeleteQueries( pQueryCount, pQueries );
			pQueryCount = 0;
		}
		if( pQueries ){
			delete [] pQueries;
			pQueries = NULL;
		}
		
		if( count > 0 ){
			pQueries = new GLuint[ count ];
			
			OGL_CHECK( pRenderThread, pglGenQueries( count, pQueries ) );
			
			for( pQueryCount=0; pQueryCount<count; pQueryCount++ ){
				if( ! pQueries[ pQueryCount ] ){
					DETHROW( deeOutOfMemory );
				}
			}
		}
	}
}

void deoglTimerQuerySet::BeginQuery( int index ){
	if( index < 0 || index >= pQueryCount ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_CHECK( pRenderThread, pglBeginQuery( GL_TIME_ELAPSED, pQueries[ index ] ) );
}

void deoglTimerQuerySet::EndQuery(){
	OGL_CHECK( pRenderThread, pglEndQuery( GL_TIME_ELAPSED ) );
}

bool deoglTimerQuerySet::HasResult( int index ) const{
	if( index < 0 || index >= pQueryCount ){
		DETHROW( deeInvalidParam );
	}
	
	GLuint result;
	
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQueries[ index ], GL_QUERY_RESULT_AVAILABLE, &result ) );
	
	return result == GL_TRUE;
}

unsigned int deoglTimerQuerySet::GetResult( int index ) const{
	if( index < 0 || index >= pQueryCount ){
		DETHROW( deeInvalidParam );
	}
	
	GLuint result;
	
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQueries[ index ], GL_QUERY_RESULT, &result ) );
	
	return ( unsigned int )result;
}
