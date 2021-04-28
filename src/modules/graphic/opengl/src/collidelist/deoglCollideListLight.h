/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLCOLLIDELISTLIGHT_H_
#define _DEOGLCOLLIDELISTLIGHT_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglRLight;
class deoglOcclusionTest;



/**
 * Light in collide list.
 */
class deoglCollideListLight : public deoglOcclusionTestListener{
private:
	deoglRLight *pLight;
	bool pCulled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list light. */
	deoglCollideListLight();
	
	/** Clean up collide list light. */
	~deoglCollideListLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list. */
	void Clear();
	
	/** Light. */
	inline deoglRLight *GetLight() const{ return pLight; }
	
	/** Set light. */
	void SetLight( deoglRLight *light );
	
	
	
	/** Light is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set light is culled. */
	void SetCulled( bool visible );
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
};

#endif
