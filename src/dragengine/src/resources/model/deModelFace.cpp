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

#include <stdlib.h>

#include "deModelFace.h"
#include "../../common/exceptions.h"



// Class deModelFace
//////////////////////

// Constructor, destructor
////////////////////////////

deModelFace::deModelFace(){
	pTexture = 0;
	pVertex[ 0 ] = 0;
	pVertex[ 1 ] = 0;
	pVertex[ 2 ] = 0;
	pNormal[ 0 ] = 0;
	pNormal[ 1 ] = 0;
	pNormal[ 2 ] = 0;
	pTangent[ 0 ] = 0;
	pTangent[ 1 ] = 0;
	pTangent[ 2 ] = 0;
	pTextureCoordinates[ 0 ] = 0;
	pTextureCoordinates[ 1 ] = 0;
	pTextureCoordinates[ 2 ] = 0;
	pFaceNormal.Set( 0.0f, 1.0f, 0.0f );
	pFaceTangent.Set( 1.0f, 0.0f, 0.0f );
}

deModelFace::~deModelFace(){
}



// Management
///////////////

void deModelFace::SetTexture( int texture ){
	if( texture < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = texture;
}



void deModelFace::SetVertex1( int vertex ){
	if( vertex < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pVertex[ 0 ] = vertex;
}

void deModelFace::SetVertex2( int vertex ){
	if( vertex < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pVertex[ 1 ] = vertex;
}

void deModelFace::SetVertex3( int vertex ){
	if( vertex < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pVertex[ 2 ] = vertex;
}

int deModelFace::GetVertexAt( int corner ) const{
	if( corner < 0 || corner > 2 ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertex[ corner ];
}

void deModelFace::SetVertexAt( int corner, int vertex ){
	if( corner < 0 || corner > 2 || vertex < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pVertex[ corner ] = vertex;
}



void deModelFace::SetNormal1( int normal ){
	if( normal < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pNormal[ 0 ] = normal;
}

void deModelFace::SetNormal2( int normal ){
	if( normal < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pNormal[ 1 ] = normal;
}

void deModelFace::SetNormal3( int normal ){
	if( normal < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pNormal[ 2 ] = normal;
}

int deModelFace::GetNormalAt( int corner ) const{
	if( corner < 0 || corner > 2 ){
		DETHROW( deeInvalidParam );
	}
	
	return pNormal[ corner ];
}

void deModelFace::SetNormalAt( int corner, int normal ){
	if( corner < 0 || corner > 2 || normal < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pNormal[ corner ] = normal;
}



void deModelFace::SetTangent1( int tangent ){
	if( tangent < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTangent[ 0 ] = tangent;
}

void deModelFace::SetTangent2( int tangent ){
	if( tangent < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTangent[ 1 ] = tangent;
}

void deModelFace::SetTangent3( int tangent ){
	if( tangent < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTangent[ 2 ] = tangent;
}

int deModelFace::GetTangentAt( int corner ) const{
	if( corner < 0 || corner > 2 ){
		DETHROW( deeInvalidParam );
	}
	
	return pTangent[ corner ];
}

void deModelFace::SetTangentAt( int corner, int tangent ){
	if( corner < 0 || corner > 2 || tangent < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTangent[ corner ] = tangent;
}



void deModelFace::SetTextureCoordinates1( int textureCoordinates ){
	if( textureCoordinates < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinates[ 0 ] = textureCoordinates;
}

void deModelFace::SetTextureCoordinates2( int textureCoordinates ){
	if( textureCoordinates < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinates[ 1 ] = textureCoordinates;
}

void deModelFace::SetTextureCoordinates3( int textureCoordinates ){
	if( textureCoordinates < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinates[ 2 ] = textureCoordinates;
}

int deModelFace::GetTextureCoordinatesAt( int corner ) const{
	if( corner < 0 || corner > 2 ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureCoordinates[ corner ];
}

void deModelFace::SetTextureCoordinatesAt( int corner, int textureCoordinates ){
	if( corner < 0 || corner > 2 || textureCoordinates < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinates[ corner ] = textureCoordinates;
}



void deModelFace::SetFaceNormal( const decVector &normal ){
	pFaceNormal = normal;
}

void deModelFace::SetFaceTangent( const decVector &tangent ){
	pFaceTangent = tangent;
}
