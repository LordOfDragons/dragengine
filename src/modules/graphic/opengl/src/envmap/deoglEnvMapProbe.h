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

#ifndef _DEOGLENVMAPPROBE_H_
#define _DEOGLENVMAPPROBE_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicEnvMapProbe.h>

class deoglREnvMapProbe;

class deGraphicOpenGl;
class deEnvMapProbe;


/**
 * Environment map probe peer.
 */
class deoglEnvMapProbe : public deBaseGraphicEnvMapProbe{
private:
	deGraphicOpenGl &pOgl;
	const deEnvMapProbe &pEnvMapProbe;
	
	deoglREnvMapProbe *pREnvMapProbe;
	
	bool pDirtyOctreeNode;
	bool pDirtyEnvMapProbe;
	bool pDirtyMatrix;
	bool pDirtyInfluenceShape;
	bool pDirtyReflectionShape;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglEnvMapProbe( deGraphicOpenGl &ogl, const deEnvMapProbe &envMapProbe );
	
	/** Clean up peer. */
	virtual ~deoglEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Environment map probe. */
	inline const deEnvMapProbe &GetEnvMapProbe() const{ return pEnvMapProbe; }
	
	
	
	/** Render environment map probe. */
	inline deoglREnvMapProbe *GetREnvMapProbe() const{ return pREnvMapProbe; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Scaling changed. */
	virtual void ScalingChanged();
	
	/** Shape list influence changed. */
	virtual void ShapeListInfluenceChanged();
	
	/** Shape reflection changed. */
	virtual void ShapeReflectionChanged();
	
	/** Influence border size changed. */
	virtual void InfluenceBorderSizeChanged();
	
	/** Influence priority changed. */
	virtual void InfluencePriorityChanged();
	
	/** Image changed. */
	virtual void ImageChanged();
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
