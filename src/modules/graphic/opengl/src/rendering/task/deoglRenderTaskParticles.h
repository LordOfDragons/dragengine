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

#ifndef _DEOGLRENDERTASKPARTICLES_H_
#define _DEOGLRENDERTASKPARTICLES_H_

class deoglRenderTaskParticlesStep;
class deoglSPBlockUBO;
class deoglRTLogger;


/**
 * Render Task Particles.
 */
class deoglRenderTaskParticles{
private:
	deoglSPBlockUBO *pRenderParamBlock;
	
	deoglRenderTaskParticlesStep **pSteps;
	int pStepCount;
	int pStepSize;
	bool pRenderVSStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task. */
	deoglRenderTaskParticles();
	/** Cleans up the render task. */
	~deoglRenderTaskParticles();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Clear the render task preparing it for a new task. */
	void Clear();
	
	/** Retrieves the render parameter shader parameter block or NULL to use none. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{return pRenderParamBlock;}
	/** Sets the render parameter shader parameter block or NULL to use none. */
	void SetRenderParamBlock(deoglSPBlockUBO *paramBlock);
	
	/** Use vertex shader stereo rendering. */
	inline bool GetRenderVSStereo() const{return pRenderVSStereo;}
	
	/** Set use vertex shader stereo rendering. */
	void SetRenderVSStereo(bool renderVSStereo);
	
	/** Retrieves the number of steps. */
	inline int GetStepCount() const{return pStepCount;}
	/** Retrieves the step at the given position. */
	deoglRenderTaskParticlesStep *GetStepAt(int index) const;
	/** Adds an empty step. */
	deoglRenderTaskParticlesStep *AddStep();
	/** Removes all steps. */
	void RemoveAllSteps();
	
	/** Debug print. */
	void DebugPrint(deoglRTLogger &rtlogger);
	/*@}*/
};

#endif
