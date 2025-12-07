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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealShader> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shader. */
	dealShader(dealDisplay &display);
	
	/** \brief Clean up shader. */
	~dealShader() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	
	
	/** \brief Compile vertex program. */
	void CompileVertexProgram(const char *source);
	
	/** \brief Compile vertex program from asset source file. */
	void CompileVertexProgramAsset(const char *filename);
	
	/** \brief Compile fragment program. */
	void CompileFragmentProgram(const char *source);
	
	/** \brief Compile fragment program from asset source file. */
	void CompileFragmentProgramAsset(const char *filename);
	
	/** \brief Bind attribute location. */
	void BindAttributeLocation(const char *attribute, int location);
	
	/** \brief Link shader. */
	void Link();
	
	/** \brief Activate shader. */
	void Activate();
	
	/** \brief Bind texture unit location. */
	void BindTextureUnitLocation(const char *textureUnit, int location);
	
	/** \brief Resolve parameter adding it to the list of known parameters. */
	void ResolveParameter(const char *parameter);
	
	
	
	/** \brief Set float parameter. */
	void SetParameterFloat(int index, float p1);
	
	/** \brief Set float parameter. */
	void SetParameterFloat(int index, float p1, float p2);
	
	/** \brief Set float parameter. */
	void SetParameterFloat(int index, float p1, float p2, float p3);
	
	/** \brief Set float parameter. */
	void SetParameterFloat(int index, float p1, float p2, float p3, float p4);
	
	/** \brief Set 3-color parameter. */
	void SetParameterColor3(int index, const decColor &color);
	
	/** \brief Set 4-color parameter. */
	void SetParameterColor4(int index, const decColor &color);
	
	/** \brief Set 4-color parameter with explicit alpha. */
	void SetParameterColor4(int index, const decColor &color, float alpha);
	
	/** \brief Set 2-component vector parameter. */
	void SetParameterVector2(int index, const decVector2 &vector);
	
	/** \brief Set 3x3 texture matrix parameter. */
	void SetParameterTexMatrix3x3(int index, const decTexMatrix &matrix);
	
	/** \brief Set 3x3 texture matrix parameter. */
	void SetParameterTexMatrix3x3(int index, const decTexMatrix2 &matrix);
	
	/**
	 * \brief Set color matrix parameters.
	 * \details The left most 4x4 part is set to the first parameter and the
	 *          right most colon to the second parameter.
	 */
	void SetParameterColorMatrix5x4(int index1, int index2, const decColorMatrix &matrix);
	
	/**
	 * \brief Set color matrix parameters.
	 * \details The left most 4x4 part is set to the first parameter and the
	 *          right most colon to the second parameter.
	 */
	void SetParameterColorMatrix5x4(int index1, int index2, const decColorMatrix3 &matrix);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
 
 
