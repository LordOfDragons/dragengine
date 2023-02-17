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

#include "deoglShaderCompiled.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderCompiled
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderCompiled::deoglShaderCompiled( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	pHandleC = 0;
	pHandleTCP = 0;
	pHandleTEP = 0;
	pHandleGP = 0;
	pHandleVP = 0;
	pHandleFP = 0;
	
	pHandleShader = pglCreateProgram();
	if( ! pHandleShader ){
		DETHROW( deeOutOfMemory );
	}
	
	pParameters = NULL;
	pParameterCount = 0;
}

deoglShaderCompiled::~deoglShaderCompiled(){
	if( pParameters ){
		delete [] pParameters;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLProgram( pHandleShader );
	dops.DeleteOpenGLShader( pHandleFP );
	dops.DeleteOpenGLShader( pHandleVP );
	dops.DeleteOpenGLShader( pHandleGP );
	dops.DeleteOpenGLShader( pHandleTEP );
	dops.DeleteOpenGLShader( pHandleTCP );
	dops.DeleteOpenGLShader( pHandleC );
}



// Management
///////////////

void deoglShaderCompiled::Activate(){
	OGL_CHECK( pRenderThread, pglUseProgram( pHandleShader ) );
}

void deoglShaderCompiled::CreateComputeProgram(){
	if( ! pHandleC ){
		pHandleC = pglCreateShader( GL_COMPUTE_SHADER );
		if( ! pHandleC ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deoglShaderCompiled::CreateTessellationControlProgram(){
	if( ! pHandleTCP ){
		pHandleTCP = pglCreateShader( GL_TESS_CONTROL_SHADER );
		if( ! pHandleTCP ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deoglShaderCompiled::CreateTessellationEvaluationProgram(){
	if( ! pHandleTEP ){
		pHandleTEP = pglCreateShader( GL_TESS_EVALUATION_SHADER );
		if( ! pHandleTEP ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deoglShaderCompiled::CreateGeometryProgram(){
	if( ! pHandleGP ){
		pHandleGP = pglCreateShader( GL_GEOMETRY_SHADER );
		if( ! pHandleGP ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deoglShaderCompiled::CreateVertexProgram(){
	if( ! pHandleVP ){
		pHandleVP = pglCreateShader( GL_VERTEX_SHADER );
		if( ! pHandleVP ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deoglShaderCompiled::CreateFragmentProgram(){
	if( ! pHandleFP ){
		pHandleFP = pglCreateShader( GL_FRAGMENT_SHADER );
		if( ! pHandleFP ){
			DETHROW( deeOutOfMemory );
		}
	}
}



bool deoglShaderCompiled::GetHasTessellation() const{
	return pHandleTCP || pHandleTEP;
}



// Parameters
///////////////

void deoglShaderCompiled::SetParameterCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters ){
		delete [] pParameters;
		pParameters = NULL;
		pParameterCount = 0;
	}
	
	if( count > 0 ){
		pParameters = new int[ count ];
		
		for( pParameterCount=0; pParameterCount<count; pParameterCount++ ){
			pParameters[ pParameterCount ] = -1;
		}
	}
}

int deoglShaderCompiled::GetParameterAt( int parameter ) const{
	if( parameter < 0 || parameter >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pParameters[ parameter ];
}

void deoglShaderCompiled::SetParameterAt( int parameter, int location ) const{
	if( parameter < 0 || parameter >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters[ parameter ] = location;
}



void deoglShaderCompiled::SetParameterFloat( int index, float p1 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform1f( pParameters[ index ], p1 ) );
	}
}

void deoglShaderCompiled::SetParameterFloat( int index, float p1, float p2 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform2f( pParameters[ index ], p1, p2 ) );
	}
}

void deoglShaderCompiled::SetParameterFloat( int index, float p1, float p2, float p3 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3f( pParameters[ index ], p1, p2, p3 ) );
	}
}

void deoglShaderCompiled::SetParameterFloat( int index, float p1, float p2, float p3, float p4 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4f( pParameters[ index ], p1, p2, p3, p4 ) );
	}
}



void deoglShaderCompiled::SetParameterInt( int index, int p1 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform1i( pParameters[ index ], p1 ) );
	}
}

void deoglShaderCompiled::SetParameterInt( int index, int p1, int p2 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform2i( pParameters[ index ], p1, p2 ) );
	}
}

void deoglShaderCompiled::SetParameterInt( int index, int p1, int p2, int p3 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3i( pParameters[ index ], p1, p2, p3 ) );
	}
}

void deoglShaderCompiled::SetParameterInt( int index, int p1, int p2, int p3, int p4 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4i( pParameters[ index ], p1, p2, p3, p4 ) );
	}
}



void deoglShaderCompiled::SetParameterUInt( int index, unsigned int p1 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform1ui( pParameters[ index ], p1 ) );
	}
}

void deoglShaderCompiled::SetParameterUInt( int index, unsigned int p1, unsigned int p2 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform2ui( pParameters[ index ], p1, p2 ) );
	}
}

void deoglShaderCompiled::SetParameterUInt( int index, unsigned int p1, unsigned int p2, unsigned int p3 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3ui( pParameters[ index ], p1, p2, p3 ) );
	}
}

void deoglShaderCompiled::SetParameterUInt( int index, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4 ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4ui( pParameters[ index ], p1, p2, p3, p4 ) );
	}
}



void deoglShaderCompiled::SetParameterPoint2( int index, const decPoint &point ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform2i( pParameters[ index ], point.x, point.y ) );
	}
}

void deoglShaderCompiled::SetParameterPoint3( int index, const decPoint3 &point ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3i( pParameters[ index ], point.x, point.y, point.z ) );
	}
}



void deoglShaderCompiled::SetParameterColor3( int index, const decColor &color ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3f( pParameters[ index ], color.r, color.g, color.b ) );
	}
}

void deoglShaderCompiled::SetParameterColor4( int index, const decColor &color ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4f( pParameters[ index ], color.r, color.g, color.b, color.a ) );
	}
}

void deoglShaderCompiled::SetParameterColor4( int index, const decColor &color, float alpha ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4f( pParameters[ index ], color.r, color.g, color.b, alpha ) );
	}
}



void deoglShaderCompiled::SetParameterVector2( int index, const decVector2 &vector ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform2f( pParameters[ index ], vector.x, vector.y ) );
	}
}



void deoglShaderCompiled::SetParameterVector3( int index, const decVector &vector ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3f( pParameters[ index ], vector.x, vector.y, vector.z ) );
	}
}

void deoglShaderCompiled::SetParameterDVector3( int index, const decDVector &vector ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform3f( pParameters[ index ], ( GLfloat )vector.x, ( GLfloat )vector.y, ( GLfloat )vector.z ) );
	}
}



void deoglShaderCompiled::SetParameterVector4( int index, const decVector4 &vector ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4f( pParameters[ index ], vector.x, vector.y, vector.z, vector.w ) );
	}
}

void deoglShaderCompiled::SetParameterDVector4( int index, const decDVector4 &vector ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniform4f( pParameters[ index ], ( GLfloat )vector.x,
			( GLfloat )vector.y, ( GLfloat )vector.z, ( GLfloat )vector.w ) );
	}
}



void deoglShaderCompiled::SetParameterMatrix4x4( int index, const decMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix4fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&matrix ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix4x3( int index, const decMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix4x3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&matrix ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix3x3( int index, const decMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 9 ] = {
			matrix.a11, matrix.a12, matrix.a13,
			matrix.a21, matrix.a22, matrix.a23,
			matrix.a31, matrix.a32, matrix.a33 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix3x2( int index, const decMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 6 ] = {
			matrix.a11, matrix.a12, matrix.a13,
			matrix.a21, matrix.a22, matrix.a23 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix3x2fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterDMatrix4x4( int index, const decDMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 16 ] = {
			( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13, ( GLfloat )matrix.a14,
			( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23, ( GLfloat )matrix.a24,
			( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33, ( GLfloat )matrix.a34,
			( GLfloat )matrix.a41, ( GLfloat )matrix.a42, ( GLfloat )matrix.a43, ( GLfloat )matrix.a44 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix4fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterDMatrix4x3( int index, const decDMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 12 ] = {
			( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13, ( GLfloat )matrix.a14,
			( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23, ( GLfloat )matrix.a24,
			( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33, ( GLfloat )matrix.a34 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix4x3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterDMatrix3x3( int index, const decDMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 9 ] = {
			( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13,
			( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23,
			( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterDMatrix3x2( int index, const decDMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		const GLfloat oglm[ 6 ] = {
			( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13,
			( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix3x2fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix4x4( int index, const float *values ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix4fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )values ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix4x3( int index, const float *values ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix4x3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )values ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix3x3( int index, const float *values ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )values ) );
	}
}

void deoglShaderCompiled::SetParameterMatrix3x2( int index, const float *values ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix3x2fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )values ) );
	}
}

void deoglShaderCompiled::SetParameterTexMatrix3x3( int index, const decTexMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix3fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&matrix ) );
	}
}

void deoglShaderCompiled::SetParameterTexMatrix3x2( int index, const decTexMatrix &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix3x2fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&matrix ) );
	}
}

void deoglShaderCompiled::SetParameterTexMatrix3x2( int index, const decTexMatrix2 &matrix ) const{
	if( index < 0 || index >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index ] != -1 ){
		OGL_CHECK( pRenderThread, pglUniformMatrix3x2fv( pParameters[ index ], 1, GL_TRUE, ( const GLfloat * )&matrix ) );
	}
}

void deoglShaderCompiled::SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix &matrix ) const{
	if( index1 < 0 || index1 >= pParameterCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParameters[ index1 ] != -1 ){
		const GLfloat oglm[ 16 ] = {
			( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13, ( GLfloat )matrix.a14,
			( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23, ( GLfloat )matrix.a24,
			( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33, ( GLfloat )matrix.a34,
			( GLfloat )matrix.a41, ( GLfloat )matrix.a42, ( GLfloat )matrix.a43, ( GLfloat )matrix.a44 };
		
		OGL_CHECK( pRenderThread, pglUniformMatrix4fv( pParameters[ index1 ], 1, GL_TRUE, ( const GLfloat * )&oglm ) );
	}
	
	SetParameterFloat( index2, matrix.a15, matrix.a25, matrix.a35, matrix.a45 );
}
