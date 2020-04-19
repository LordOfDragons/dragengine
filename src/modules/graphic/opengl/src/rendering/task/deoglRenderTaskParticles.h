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

#ifndef _DEOGLRENDERTASKPARTICLES_H_
#define _DEOGLRENDERTASKPARTICLES_H_

class deoglRenderTaskParticlesStep;
class deoglSPBlockUBO;
class deoglRTLogger;


/**
 * @brief Render Task Particles.
 */
class deoglRenderTaskParticles{
private:
	deoglSPBlockUBO *pRenderParamBlock;
	
	deoglRenderTaskParticlesStep **pSteps;
	int pStepCount;
	int pStepSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task. */
	deoglRenderTaskParticles();
	/** Cleans up the render task. */
	~deoglRenderTaskParticles();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Clear the render task preparing it for a new task. */
	void Clear();
	
	/** Retrieves the render parameter shader parameter block or NULL to use none. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{ return pRenderParamBlock; }
	/** Sets the render parameter shader parameter block or NULL to use none. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	/** Retrieves the number of steps. */
	inline int GetStepCount() const{ return pStepCount; }
	/** Retrieves the step at the given position. */
	deoglRenderTaskParticlesStep *GetStepAt( int index ) const;
	/** Adds an empty step. */
	deoglRenderTaskParticlesStep *AddStep();
	/** Removes all steps. */
	void RemoveAllSteps();
	
	/** \brief Debug print. */
	void DebugPrint( deoglRTLogger &rtlogger );
	/*@}*/
};

#endif
