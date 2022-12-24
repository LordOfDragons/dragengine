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
#include "../../pipeline/deoglPipeline.h"


/**
 * Render effect filter kernel.
 */
class deoglREffectFilterKernel : public deoglREffect{
private:
	float *pKernel;
	int pKernelRows;
	int pKernelCols;
	float pScale;
	
	deoglPipeline *pPipeline;
	deoglPipeline *pPipelineStereo;
	deoglPipeline *pPipelineDownsample;
	deoglPipeline *pPipelineDownsampleStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render effect. */
	deoglREffectFilterKernel( deoglRenderThread &renderThread );
	
	/** Clean up render effect. */
	virtual ~deoglREffectFilterKernel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of kernel rows. */
	inline int GetKernelRows() const{ return pKernelRows; }
	
	/** Number of kernel cols. */
	inline int GetKernelCols() const{ return pKernelCols; }
	
	/** Set kernel size. */
	void SetKernelSize( int rows, int cols );
	
	/** Kernel value. */
	float GetKernelValueAt( int row, int col ) const;
	
	/** Set kernel value. */
	void SetKernelValueAt( int row, int col, float value );
	
	/** Kernel scale. */
	inline float GetScale() const{ return pScale; }
	
	/** Set kernel scale. */
	void SetScale( float scale );
	
	
	
	/** Get pipeline creating it if required. */
	deoglPipeline *GetPipeline();
	deoglPipeline *GetPipelineStereo();
	
	/** Get downsample pipeline creating it if required. */
	deoglPipeline *GetPipelineDownsample();
	deoglPipeline *GetPipelineDownsampleStereo();
	
	/** Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif
