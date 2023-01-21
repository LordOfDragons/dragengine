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

#ifndef _DEOGLEFFECT_H_
#define _DEOGLEFFECT_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicEffect.h>

class deoglREffect;

class deEffect;
class deGraphicOpenGl;



/**
 * Effect.
 */
class deoglEffect : public deBaseGraphicEffect{
private:
	deGraphicOpenGl &pOgl;
	const deEffect &pEffect;
	
	deoglREffect *pREffect;
	
	bool pDirtyEnabled;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create effect peer. */
	deoglEffect( deGraphicOpenGl &ogl, const deEffect &effect );
	
	/** Clean up effect. */
	virtual ~deoglEffect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Effect engine resource. */
	inline const deEffect &GetEffect() const{ return pEffect; }
	
	
	
	/** Render effect. */
	inline deoglREffect *GetREffect() const{ return pREffect; }
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Enabled state changed. */
	virtual void EnabledChanged();
	/*@}*/
	
protected:
	void InitREffect( deoglREffect *reffect );
};

#endif
