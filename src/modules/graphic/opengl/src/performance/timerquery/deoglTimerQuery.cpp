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

#include "deoglTimerQuery.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTimerQuery
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglTimerQuery::deoglTimerQuery( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pQuery( 0 )
{
	OGL_CHECK( renderThread, pglGenQueries( 1, &pQuery ) );
	if( ! pQuery ){
		DETHROW( deeOutOfMemory );
	}
}

deoglTimerQuery::~deoglTimerQuery(){
	if( pQuery ){
		pglDeleteQueries( 1, &pQuery );
	}
}



// Management
///////////////

void deoglTimerQuery::BeginQuery(){
	OGL_CHECK( pRenderThread, pglBeginQuery( GL_TIME_ELAPSED, pQuery ) );
}

void deoglTimerQuery::EndQuery(){
	OGL_CHECK( pRenderThread, pglEndQuery( GL_TIME_ELAPSED ) );
}

bool deoglTimerQuery::HasResult() const{
	GLuint result;
	
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQuery, GL_QUERY_RESULT_AVAILABLE, &result ) );
	
	return result == GL_TRUE;
}

unsigned int deoglTimerQuery::GetResult() const{
	GLuint result;
	
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQuery, GL_QUERY_RESULT, &result ) );
	
	return ( unsigned int )result;
}
