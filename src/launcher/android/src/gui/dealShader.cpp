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

#include "dealShader.h"
#include "dealDisplay.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../common/file/decMemoryFile.h"
#include "../common/math/decMath.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealShader
/////////////////////

// Constructors, destructors
//////////////////////////////

dealShader::dealShader(  dealDisplay &display ) :
pDisplay( display ),

pHandleShader( 0 ),
pHandleVP( 0 ),
pHandleFP( 0 )
{
	pHandleShader = glCreateProgram();
	if( ! pHandleShader ){
		DETHROW( deeOutOfMemory );
	}
}

dealShader::~dealShader(){
	pCleanUp();
}



// Management
///////////////

void dealShader::CompileVertexProgram( const char *source ){
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	if( pHandleVP ){
		DETHROW( deeInvalidParam );
	}
	
	int sourceLen = strlen( source );
	int result;
	
	// create handle
	pHandleVP = glCreateShader( GL_VERTEX_SHADER );
	if( ! pHandleVP ){
		DETHROW( deeOutOfMemory );
	}
	
	// load source
	glShaderSource( pHandleVP, 1, &source, &sourceLen );
	
	// compile source
	glCompileShader( pHandleVP );
	glGetShaderiv( pHandleVP, GL_COMPILE_STATUS, &result );
	
	if( ! result ){
		int blen = 0;
		
		glGetShaderiv( pHandleVP, GL_INFO_LOG_LENGTH , &blen );
		
		if( blen > 1 ){
			int slen = 0;
			decString error;
			error.Set( ' ', blen );
			
			glGetShaderInfoLog( pHandleVP, blen, &slen, ( char* )error.GetString() );
			pDisplay.GetLauncher().GetLogger().LogErrorFormat( LOGSOURCE, "Vertex-Program: %s", error.GetString() );
			pDisplay.GetLauncher().GetLogger().LogErrorFormat( LOGSOURCE, "Source: %s", source );
			DETHROW( deeInvalidParam );
		}
	}
	
	// attach to shader
	glAttachShader( pHandleShader, pHandleVP );
}

void dealShader::CompileVertexProgramAsset( const char *filename ){
	decMemoryFile *memoryFileSource = NULL;
	
	try{
		memoryFileSource = new decMemoryFile( "source" );
		pDisplay.GetLauncher().LoadAsset( filename, *memoryFileSource );
		
		decString source;
		source.Set( ' ', memoryFileSource->GetLength() );
		memcpy( ( char* )source.GetString(), memoryFileSource->GetPointer(), memoryFileSource->GetLength() );
		
		CompileVertexProgram( source );
		
		memoryFileSource->FreeReference();
		
	}catch( const deException & ){
		if( memoryFileSource ){
			memoryFileSource->FreeReference();
		}
		throw;
	}
}

void dealShader::CompileFragmentProgram( const char *source ){
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	if( pHandleFP ){
		DETHROW( deeInvalidParam );
	}
	
	int sourceLen = strlen( source );
	int result;
	
	// create handle
	pHandleFP = glCreateShader( GL_FRAGMENT_SHADER );
	if( ! pHandleFP ){
		DETHROW( deeOutOfMemory );
	}
	
	// load source
	glShaderSource( pHandleFP, 1, &source, &sourceLen );
	
	// compile source
	glCompileShader( pHandleFP );
	glGetShaderiv( pHandleFP, GL_COMPILE_STATUS, &result );
	
	if( ! result ){
		int blen = 0;
		
		glGetShaderiv( pHandleFP, GL_INFO_LOG_LENGTH , &blen );
		
		if( blen > 1 ){
			int slen = 0;
			decString error;
			error.Set( ' ', blen );
			
			glGetShaderInfoLog( pHandleFP, blen, &slen, ( char* )error.GetString() );
			pDisplay.GetLauncher().GetLogger().LogErrorFormat( LOGSOURCE, "Fragment-Program: %s", error.GetString() );
			pDisplay.GetLauncher().GetLogger().LogErrorFormat( LOGSOURCE, "Source: %s", source );
			DETHROW( deeInvalidParam );
		}
	}
	
	// attach to shader
	glAttachShader( pHandleShader, pHandleFP );
}

void dealShader::CompileFragmentProgramAsset( const char *filename ){
	decMemoryFile *memoryFileSource = NULL;
	
	try{
		memoryFileSource = new decMemoryFile( "source" );
		pDisplay.GetLauncher().LoadAsset( filename, *memoryFileSource );
		
		decString source;
		source.Set( ' ', memoryFileSource->GetLength() );
		memcpy( ( char* )source.GetString(), memoryFileSource->GetPointer(), memoryFileSource->GetLength() );
		
		CompileFragmentProgram( source );
		
		memoryFileSource->FreeReference();
		
	}catch( const deException & ){
		if( memoryFileSource ){
			memoryFileSource->FreeReference();
		}
		throw;
	}
}

void dealShader::BindAttributeLocation( const char *attribute, int location ){
	if( ! pHandleVP || ! pHandleFP ){
		DETHROW( deeInvalidAction );
	}
	if( ! attribute ){
		DETHROW( deeInvalidParam );
	}
	
	glBindAttribLocation( pHandleShader, location, attribute );
}

void dealShader::Link(){
	if( ! pHandleVP || ! pHandleFP ){
		DETHROW( deeInvalidAction );
	}
	
	int result;
	
	glLinkProgram( pHandleShader );
	glGetProgramiv( pHandleShader, GL_LINK_STATUS, &result );
	
	if( ! result ){
		int blen = 0;
		
		glGetProgramiv( pHandleShader, GL_INFO_LOG_LENGTH , &blen );
		
		if( blen > 1 ){
			int slen = 0;
			decString error;
			error.Set( ' ', blen );
			
			glGetProgramInfoLog( pHandleShader, blen, &slen, ( char* )error.GetString() );
			pDisplay.GetLauncher().GetLogger().LogErrorFormat( LOGSOURCE, "Link Shader: %s", error.GetString() );
			DETHROW( deeInvalidParam );
		}
	}
}

void dealShader::Activate(){
	glUseProgram( pHandleShader );
}

void dealShader::BindTextureUnitLocation( const char *textureUnit, int location ){
	if( ! pHandleVP || ! pHandleFP ){
		DETHROW( deeInvalidAction );
	}
	if( ! textureUnit ){
		DETHROW( deeInvalidParam );
	}
	
	const int realLocation = glGetUniformLocation( pHandleShader, textureUnit );
	if( realLocation == -1 ){
		return;
	}
	
	glUniform1i( realLocation, location );
}



void dealShader::ResolveParameter( const char *parameter ){
	if( ! pHandleVP || ! pHandleFP ){
		DETHROW( deeInvalidAction );
	}
	if( ! parameter ){
		DETHROW( deeInvalidParam );
	}
	
	pParameters.Add( glGetUniformLocation( pHandleShader, parameter ) );
}



void dealShader::SetParameterFloat( int index, float p1 ){
	const int location = pParameters.GetAt( index );
	if( location == -1 ){
		return;
	}
	
	glUniform1f( location, p1 );
}

void dealShader::SetParameterFloat( int index, float p1, float p2 ){
	const int location = pParameters.GetAt( index );
	if( location == -1 ){
		return;
	}
	
	glUniform2f( location, p1, p2 );
}

void dealShader::SetParameterFloat( int index, float p1, float p2, float p3 ){
	const int location = pParameters.GetAt( index );
	if( location == -1 ){
		return;
	}
	
	glUniform3f( location, p1, p2, p3 );
}

void dealShader::SetParameterFloat( int index, float p1, float p2, float p3, float p4 ){
	const int location = pParameters.GetAt( index );
	if( location == -1 ){
		return;
	}
	
	glUniform4f( location, p1, p2, p3, p4 );
}

void dealShader::SetParameterColor3( int index, const decColor &color ){
	SetParameterFloat( index, color.r, color.g, color.b );
}

void dealShader::SetParameterColor4( int index, const decColor &color ){
	SetParameterFloat( index, color.r, color.g, color.b, color.a );
}

void dealShader::SetParameterColor4( int index, const decColor &color, float alpha ){
	SetParameterFloat( index, color.r, color.g, color.b, alpha );
}

void dealShader::SetParameterVector2( int index, const decVector2 &vector ){
	SetParameterFloat( index, vector.x, vector.y );
}

void dealShader::SetParameterTexMatrix3x3( int index, const decTexMatrix &matrix ){
	const int location = pParameters.GetAt( index );
	if( location == -1 ){
		return;
	}
	
	glUniformMatrix3fv( location, 1, GL_TRUE, ( const GLfloat * )&matrix );
}

void dealShader::SetParameterTexMatrix3x3( int index, const decTexMatrix2 &matrix ){
	SetParameterTexMatrix3x3( index, matrix.ToTexMatrix() );
}

void dealShader::SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix &matrix ){
	const int location = pParameters.GetAt( index1 );
	if( location == -1 ){
		return;
	}
	
	const GLfloat oglm[ 16 ] = {
		( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13, ( GLfloat )matrix.a14,
		( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23, ( GLfloat )matrix.a24,
		( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33, ( GLfloat )matrix.a34,
		( GLfloat )matrix.a41, ( GLfloat )matrix.a42, ( GLfloat )matrix.a43, ( GLfloat )matrix.a44 };
	
	glUniformMatrix4fv( location, 1, GL_TRUE, ( const GLfloat * )&oglm );
	
	SetParameterFloat( index2, matrix.a15, matrix.a25, matrix.a35, matrix.a45 );
}

void dealShader::SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix3 &matrix ){
	const int location = pParameters.GetAt( index1 );
	if( location == -1 ){
		return;
	}
	
	const GLfloat oglm[ 16 ] = {
		( GLfloat )matrix.a11, ( GLfloat )matrix.a12, ( GLfloat )matrix.a13, ( GLfloat )matrix.a14,
		( GLfloat )matrix.a21, ( GLfloat )matrix.a22, ( GLfloat )matrix.a23, ( GLfloat )matrix.a24,
		( GLfloat )matrix.a31, ( GLfloat )matrix.a32, ( GLfloat )matrix.a33, ( GLfloat )matrix.a34,
		( GLfloat )0.0,        ( GLfloat )0.0,        ( GLfloat )0.0,        ( GLfloat )1.0 };
	
	glUniformMatrix4fv( location, 1, GL_TRUE, ( const GLfloat * )&oglm );
	
	SetParameterFloat( index2, 0.0f, 0.0f, 0.0f, 0.0f );
}



// Private functions
//////////////////////

void dealShader::pCleanUp(){
	if( pHandleShader ){
		glDeleteProgram( pHandleShader );
	}
	
	if( pHandleFP ){
		glDeleteShader( pHandleFP );
	}
	if( pHandleVP ){
		glDeleteShader( pHandleVP );
	}
}
