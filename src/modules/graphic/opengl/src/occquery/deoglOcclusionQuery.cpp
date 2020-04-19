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

#include "deoglOcclusionQuery.h"
#include "deoglOcclusionQueryManager.h"
#include "../shapes/deoglShape.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionQuery
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionQuery::deoglOcclusionQuery( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pQuery( 0 )
{
	OGL_CHECK( renderThread, pglGenQueries( 1, &pQuery ) );
	if( ! pQuery ){
		DETHROW( deeOutOfMemory );
	}
}

deoglOcclusionQuery::~deoglOcclusionQuery(){
	//EndQuery();
	if( pQuery ){
		pglDeleteQueries( 1, &pQuery );
	}
}



// Management
///////////////

bool deoglOcclusionQuery::IsActive() const{
	return pRenderThread.GetOcclusionQueryManager().GetActiveQuery() == this;
}

void deoglOcclusionQuery::BeginQuery( eQueryTypes type ){
	deoglOcclusionQueryManager &ocmgr = pRenderThread.GetOcclusionQueryManager();
	
	switch( type ){
	case eqtAny:
		ocmgr.EndActiveQuery();
		OGL_CHECK( pRenderThread, pglBeginQuery( GL_ANY_SAMPLES_PASSED, pQuery ) );
		break;
		
	case eqtCount:
		#ifdef OS_ANDROID
		DETHROW( deeInvalidParam );
		#else
		ocmgr.EndActiveQuery();
		OGL_CHECK( pRenderThread, pglBeginQuery( GL_SAMPLES_PASSED, pQuery ) );
		#endif
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	pType = type;
	
	ocmgr.SetActiveQuery( this );
}

void deoglOcclusionQuery::EndQuery(){
	deoglOcclusionQueryManager &ocmgr = pRenderThread.GetOcclusionQueryManager();
	if( ocmgr.GetActiveQuery() != this ){
		return;
	}
	
	switch( pType ){
	case eqtAny:
		OGL_CHECK( pRenderThread, pglEndQuery( GL_ANY_SAMPLES_PASSED ) );
		break;
		
	case eqtCount:
		#ifdef OS_ANDROID
		DETHROW( deeInvalidParam );
		#else
		OGL_CHECK( pRenderThread, pglEndQuery( GL_SAMPLES_PASSED ) );
		#endif
		break;
	}
	
	ocmgr.SetActiveQuery( NULL );
}

bool deoglOcclusionQuery::HasResult() const{
	GLuint result;
	
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQuery, GL_QUERY_RESULT_AVAILABLE, &result ) );
	
	return result == GL_TRUE;
}

int deoglOcclusionQuery::GetResult() const{
	GLuint result;
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQuery, GL_QUERY_RESULT, &result ) );
	return ( int )result;
}

bool deoglOcclusionQuery::GetResultAny() const{
	GLuint result;
	OGL_CHECK( pRenderThread, pglGetQueryObjectuiv( pQuery, GL_QUERY_RESULT, &result ) );
	return result > 0;
}
