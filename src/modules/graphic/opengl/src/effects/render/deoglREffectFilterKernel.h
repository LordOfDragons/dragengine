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
	
	const deoglPipeline *pPipeline;
	const deoglPipeline *pPipelineStereo;
	const deoglPipeline *pPipelineDownsample;
	const deoglPipeline *pPipelineDownsampleStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render effect. */
	deoglREffectFilterKernel(deoglRenderThread &renderThread);
	
	/** Clean up render effect. */
	~deoglREffectFilterKernel() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of kernel rows. */
	inline int GetKernelRows() const{ return pKernelRows; }
	
	/** Number of kernel cols. */
	inline int GetKernelCols() const{ return pKernelCols; }
	
	/** Set kernel size. */
	void SetKernelSize(int rows, int cols);
	
	/** Kernel value. */
	float GetKernelValueAt(int row, int col) const;
	
	/** Set kernel value. */
	void SetKernelValueAt(int row, int col, float value);
	
	/** Kernel scale. */
	inline float GetScale() const{ return pScale; }
	
	/** Set kernel scale. */
	void SetScale(float scale);
	
	
	
	/** Get pipeline creating it if required. */
	const deoglPipeline *GetPipeline();
	const deoglPipeline *GetPipelineStereo();
	
	/** Get downsample pipeline creating it if required. */
	const deoglPipeline *GetPipelineDownsample();
	const deoglPipeline *GetPipelineDownsampleStereo();
	
	/** Render effect. */
	void Render(deoglRenderPlan &plan) override;
};

#endif
