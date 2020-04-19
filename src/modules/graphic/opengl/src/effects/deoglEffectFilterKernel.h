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

#ifndef _DEOGLEFFECTFILTERKERNEL_H_
#define _DEOGLEFFECTFILTERKERNEL_H_

#include "deoglEffect.h"

class deoglREffectFilterKernel;
class deEffectFilterKernel;



/**
 * \brief Effect filter kernel.
 */
class deoglEffectFilterKernel : public deoglEffect{
private:
	const deEffectFilterKernel &pEffectFilterKernel;
	
	deoglREffectFilterKernel *pREffectFilterKernel;
	
	bool pDirtyKernel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect peer. */
	deoglEffectFilterKernel( deGraphicOpenGl &ogl, const deEffectFilterKernel &effect );
	
	/** \brief Clean up effect. */
	virtual ~deoglEffectFilterKernel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Effect color matrix engine resource. */
	inline const deEffectFilterKernel &GetEffectFilterKernel() const{ return pEffectFilterKernel; }
	
	
	
	/** \brief Update render thread counterpart if required. */
	virtual void SyncToRender();
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** \brief Filter kernel changed. */
	virtual void FilterKernelChanged();
	/*@}*/
};

#endif
