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
