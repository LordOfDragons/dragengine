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

#ifndef _DEOGLENVMAPPROBE_H_
#define _DEOGLENVMAPPROBE_H_

#include "deoglREnvMapProbe.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicEnvMapProbe.h>

class deGraphicOpenGl;
class deEnvMapProbe;


/**
 * Environment map probe peer.
 */
class deoglEnvMapProbe : public deBaseGraphicEnvMapProbe{
private:
	deGraphicOpenGl &pOgl;
	const deEnvMapProbe &pEnvMapProbe;
	
	deoglREnvMapProbe::Ref pREnvMapProbe;
	
	bool pDirtyOctreeNode;
	bool pDirtyEnvMapProbe;
	bool pDirtyMatrix;
	bool pDirtyInfluenceShape;
	bool pDirtyReflectionShape;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglEnvMapProbe(deGraphicOpenGl &ogl, const deEnvMapProbe &envMapProbe);
	
	/** Clean up peer. */
	~deoglEnvMapProbe() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Environment map probe. */
	inline const deEnvMapProbe &GetEnvMapProbe() const{ return pEnvMapProbe; }
	
	
	
	/** Render environment map probe. */
	inline const deoglREnvMapProbe::Ref &GetREnvMapProbe() const{ return pREnvMapProbe; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Scaling changed. */
	void ScalingChanged() override;
	
	/** Shape list influence changed. */
	void ShapeListInfluenceChanged() override;
	
	/** Shape reflection changed. */
	void ShapeReflectionChanged() override;
	
	/** Influence border size changed. */
	void InfluenceBorderSizeChanged() override;
	
	/** Influence priority changed. */
	void InfluencePriorityChanged() override;
	
	/** Image changed. */
	void ImageChanged() override;
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
