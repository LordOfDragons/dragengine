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

#ifndef _DEOGLEFFECTOVERLAYIMAGE_H_
#define _DEOGLEFFECTOVERLAYIMAGE_H_

#include "deoglEffect.h"

class deoglREffectOverlayImage;
class deEffectOverlayImage;



/**
 * Effect distort image.
 */
class deoglEffectOverlayImage : public deoglEffect{
private:
	const deEffectOverlayImage &pEffectOverlayImage;
	
	deoglREffectOverlayImage *pREffectOverlayImage;
	
	bool pDirtyTransparency;
	bool pDirtyImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create effect peer. */
	deoglEffectOverlayImage( deGraphicOpenGl &ogl, const deEffectOverlayImage &effect );
	
	/** Clean up effect. */
	virtual ~deoglEffectOverlayImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Effect color matrix engine resource. */
	inline const deEffectOverlayImage &GetEffectOverlayImage() const{ return pEffectOverlayImage; }
	
	
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Image changed. */
	virtual void ImageChanged();
	
	/** Transparency changed. */
	virtual void TransparencyChanged();
	/*@}*/
};

#endif
