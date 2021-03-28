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

#ifndef _DEOGLSHADERCOMPILED_H_
#define _DEOGLSHADERCOMPILED_H_

#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglShaderDefines;
class deoglRenderThread;



/**
 * @brief Compiled Shader Program.
 * A compiled shader program. Shader programs are compiled from a base code loaded
 * from disk using defines to produce a specialized program.
 */
class deoglShaderCompiled{
private:
	deoglRenderThread &pRenderThread;
	
	GLuint pHandleShader;
	GLuint pHandleC;
	GLuint pHandleTCP;
	GLuint pHandleTEP;
	GLuint pHandleGP;
	GLuint pHandleVP;
	GLuint pHandleFP;
	
	int *pParameters;
	int pParameterCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader compiled object. */
	deoglShaderCompiled( deoglRenderThread &renderThread );
	/** Cleans up the shader compiled object. */
	~deoglShaderCompiled();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Activates the program. */
	void Activate();
	
	/** Create compute program if not existing. */
	void CreateComputeProgram();
	
	/** Create tessellation control program if not existing. */
	void CreateTessellationControlProgram();
	
	/** Create tessellation evaluation program if not existing. */
	void CreateTessellationEvaluationProgram();
	/** Create geometry program if not existing. */
	void CreateGeometryProgram();
	/** Create vertex program if not existing. */
	void CreateVertexProgram();
	/** Create fragment program if not existing. */
	void CreateFragmentProgram();
	
	/** Retrieves the shader handle. */
	inline GLuint GetHandleShader() const{ return pHandleShader; }
	
	/** Compute program object handle. */
	inline GLuint GetHandleC() const{ return pHandleC; }
	
	/** Retrieves the tessellation control program object handle. */
	inline GLuint GetHandleTCP() const{ return pHandleTCP; }
	
	/** Retrieves the tessellation evaluation program object handle. */
	inline GLuint GetHandleTEP() const{ return pHandleTEP; }
	/** Retrieves the geometry program object handle. */
	inline GLuint GetHandleGP() const{ return pHandleGP; }
	/** Retrieves the vertex program object handle. */
	inline GLuint GetHandleVP() const{ return pHandleVP; }
	/** Retrieves the fragment program object handle. */
	inline GLuint GetHandleFP() const{ return pHandleFP; }
	/** Determines if the shader has tessellation. */
	bool GetHasTessellation() const;
	/*@}*/
	
	/** @name Parameters */
	/*@{*/
	/** Sets the number of parameters. */
	void SetParameterCount( int count );
	/** Retrieves the parameter location or -1 if not used. */
	int GetParameterAt( int parameter ) const;
	/** Sets the parameter location. */
	void SetParameterAt( int parameter, int location );
	
	/** Sets the values of the parameter at the given position. */
	void SetParameterFloat( int index, float p1 );
	void SetParameterFloat( int index, float p1, float p2 );
	void SetParameterFloat( int index, float p1, float p2, float p3 );
	void SetParameterFloat( int index, float p1, float p2, float p3, float p4 );
	void SetParameterInt( int index, int p1 );
	void SetParameterInt( int index, int p1, int p2 );
	void SetParameterInt( int index, int p1, int p2, int p3 );
	void SetParameterInt( int index, int p1, int p2, int p3, int p4 );
	
	void SetParameterPoint2( int index, const decPoint &point );
	void SetParameterPoint3( int index, const decPoint3 &point );
	
	/** Sets a 3-color parameter. */
	void SetParameterColor3( int index, const decColor &color );
	/** Sets a 4-color parameter. */
	void SetParameterColor4( int index, const decColor &color );
	/** Sets a 4-color parameter with explicit alpha. */
	void SetParameterColor4( int index, const decColor &color, float alpha );
	
	/** Sets a 3-component vector parameter. */
	void SetParameterVector3( int index, const decVector &vector );
	/** Sets a 3-component double vector parameter. */
	void SetParameterDVector3( int index, const decDVector &vector );
	
	/** Sets a 4-component vector parameter. */
	void SetParameterVector4( int index, const decVector4 &vector );
	/** Sets a 4-component double vector parameter. */
	void SetParameterDVector4( int index, const decDVector4 &vector );
	
	/** Sets a 4x4 matrix parameter. */
	void SetParameterMatrix4x4( int index, const decMatrix &matrix );
	/** Sets a 4x3 matrix parameter. */
	void SetParameterMatrix4x3( int index, const decMatrix &matrix );
	/** Sets a 3x3 matrix parameter. */
	void SetParameterMatrix3x3( int index, const decMatrix &matrix );
	/** Sets a 3x2 matrix parameter. */
	void SetParameterMatrix3x2( int index, const decMatrix &matrix );
	/** Sets a 4x4 double matrix parameter. */
	void SetParameterDMatrix4x4( int index, const decDMatrix &matrix );
	/** Sets a 4x3 double matrix parameter. */
	void SetParameterDMatrix4x3( int index, const decDMatrix &matrix );
	/** Sets a 3x3 double matrix parameter. */
	void SetParameterDMatrix3x3( int index, const decDMatrix &matrix );
	/** Sets a 3x2 double matrix parameter. */
	void SetParameterDMatrix3x2( int index, const decDMatrix &matrix );
	/** Sets a 4x4 matrix parameter using an array of float values. */
	void SetParameterMatrix4x4( int index, const float *values );
	/** Sets a 4x3 matrix parameter using an array of float values. */
	void SetParameterMatrix4x3( int index, const float *values );
	/** Sets a 3x3 matrix parameter using an array of float values. */
	void SetParameterMatrix3x3( int index, const float *values );
	/** Sets a 3x2 matrix parameter using an array of float values. */
	void SetParameterMatrix3x2( int index, const float *values );
	
	/** Sets a 3x3 texture matrix parameter. */
	void SetParameterTexMatrix3x3( int index, const decTexMatrix &matrix );
	/** Sets a 3x2 texture matrix parameter. */
	void SetParameterTexMatrix3x2( int index, const decTexMatrix &matrix );
	/** Sets a 3x2 texture matrix parameter. */
	void SetParameterTexMatrix3x2( int index, const decTexMatrix2 &matrix );
	
	/**
	 * Set color matrix parameters.
	 * \details The left most 4x4 part is set to the first parameter and the
	 *          right most colon to the second parameter.
	 */
	void SetParameterColorMatrix5x4( int index1, int index2, const decColorMatrix &matrix );
	/*@}*/
};

#endif
