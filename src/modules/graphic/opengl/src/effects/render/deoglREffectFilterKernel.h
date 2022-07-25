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

#ifndef _DEOGLREFFECTFILTERKERNEL_H_
#define _DEOGLREFFECTFILTERKERNEL_H_

#include "deoglREffect.h"
#include "../../shaders/deoglShaderProgramUsage.h"


/**
 * \brief Render effect filter kernel.
 */
class deoglREffectFilterKernel : public deoglREffect{
private:
	float *pKernel;
	int pKernelRows;
	int pKernelCols;
	float pScale;
	
	deoglShaderProgramUsage pShader;
	deoglShaderProgramUsage pShaderStereo;
	deoglShaderProgramUsage pShaderDownsample;
	deoglShaderProgramUsage pShaderDownsampleStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render effect. */
	deoglREffectFilterKernel( deoglRenderThread &renderThread );
	
	/** \brief Clean up render effect. */
	virtual ~deoglREffectFilterKernel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of kernel rows. */
	inline int GetKernelRows() const{ return pKernelRows; }
	
	/** \brief Number of kernel cols. */
	inline int GetKernelCols() const{ return pKernelCols; }
	
	/** \brief Set kernel size. */
	void SetKernelSize( int rows, int cols );
	
	/** \brief Kernel value. */
	float GetKernelValueAt( int row, int col ) const;
	
	/** \brief Set kernel value. */
	void SetKernelValueAt( int row, int col, float value );
	
	/** \brief Kernel scale. */
	inline float GetScale() const{ return pScale; }
	
	/** \brief Set kernel scale. */
	void SetScale( float scale );
	
	
	
	/** \brief Get shader creating it if required. */
	deoglShaderProgram *GetShader();
	deoglShaderProgram *GetShaderStereo();
	
	/** \brief Get downsample shader creating it if required. */
	deoglShaderProgram *GetShaderDownsample();
	deoglShaderProgram *GetShaderDownsampleStereo();
	
	/** \brief Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif
