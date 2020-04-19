/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALSHADER_H_
#define _DEALSHADER_H_

#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "../deObject.h"
#include "../common/math/decMath.h"
#include "../common/collection/decIntList.h"


class dealDisplay;


/**
 * \brief Shader.
 * 
 * Usage:
 * \code{.cpp}
 * shader = new dealShader( display );
 * shader->CompileVertexProgram( "code..." );
 * shader->CompileFragmentProgram( "code..." );
 * shader->BindAttributeLocation( "inPosition", 0 );
 * shader->Link();
 * shader->Activate();
 * shader->BindTextureUnitLocation( "texColor", 0 );
 * shader->ResolveParameter( "vParam1" ); // available at index 0 now
 * shader->ResolveParameter( "vParam2" ); // available at index 1 now
 * \endcode
 * 
 * After linking the shader should not be modified anymore.
 * 
 */
class dealShader : public deObject{
private:
	dealDisplay &pDisplay;
	
	GLuint pHandleShader;
	GLuint pHandleVP;
	GLuint pHandleFP;
	
	decIntList pParameters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shader. */
	dealShader( dealDisplay &display );
	
	/** \brief Clean up shader. */
	virtual ~dealShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	
	
	/** \brief Compile vertex program. */
	void CompileVertexProgram( const char *source );
	
	/** \brief Compile vertex program from asset source file. */
	void CompileVertexProgramAsset( const char *filename );
	
	/** \brief Compile fragment program. */
	void CompileFragmentProgram( const char *source );
	
	/** \brief Compile fragment program from asset source file. */
	void CompileFragmentProgramAsset( const char *filename );
	
	/** \brief Bind attribute location. */
	void BindAttributeLocation( const char *attribute, int location );
	
	/** \brief Link shader. */
	void Link();
	
	/** \brief Activate shader. */
	void Activate();
	
	/** \brief Bind texture unit location. */
	void BindTextureUnitLocation( const char *textureUnit, int location );
	
	/** \brief Resolve parameter adding it to the list of known parameters. */
	void ResolveParameter( const char *parameter );
	
	
	
	/** \brief Set float parameter. */
	void SetParameterFloat( int index, float p1 );
	
	/** \brief Set float parameter. */
	void SetParameterFloat( int index, float p1, float p2 );
	
	/** \brief Set float parameter. */
	void SetParameterFloat( int index, float p1, float p2, float p3 );
	
	/** \brief Set float parameter. */
	void SetParameterFloat( int index, float p1, float p2, float p3, float p4 );
	
	/** \brief Set 3-color parameter. */
	void SetParameterColor3( int index, const decColor &color );
	
	/** \brief Set 4-color parameter. */
	void SetParameterColor4( int index, const decColor &color );
	
	/** \brief Set 4-color parameter with explicit alpha. */
	void SetParameterColor4( int index, const decColor &color, float alpha );
	
	/** \brief Set 2-component vector parameter. */
	void SetParameterVector2( int index, const decVector2 &vector );
	
	/** \brief Set 3x3 texture matrix parameter. */
	void SetParameterTexMatrix3x3( int index, const decTexMatrix &matrix );
	
	/** \brief Set 3x3 texture matrix parameter. */
	void SetParameterTexMatrix3x3( int index, const decTexMatrix2 &matrix );
	
	/**
	 * \brief Set color matrix parameters.
	 * \details The left most 4x4 part is set to the first parameter and the
	 *          right most colon to the second parameter.
	 */
	void SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix &matrix );
	
	/**
	 * \brief Set color matrix parameters.
	 * \details The left most 4x4 part is set to the first parameter and the
	 *          right most colon to the second parameter.
	 */
	void SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix3 &matrix );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
 
 
