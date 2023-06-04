/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRSWAPCHAIN_H_
#define _DEOXRSWAPCHAIN_H_

#include "deoxrBasics.h"
#include "graphicapi/deoxrGraphicApiOpenGL.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoxrSession;


/**
 * Oxr swapchain.
 */
class deoxrSwapchain : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSwapchain> Ref;
	
	/** Image. */
	struct sImage{
		uint32_t openglImage;
		deoxrGraphicApiOpenGL::Framebuffer::Ref openglFbo;
	};
	
	/** Type. */
	enum eType{
		etColor,
		etDepth
	};
	
	
	
private:
	deoxrSession &pSession;
	
	const eType pType;
	decPoint pSize;
	
	XrSwapchain pSwapchain;
	int64_t pImageFormat;
	
	sImage *pImages;
	int pImageCount;
	
	uint32_t pAcquiredImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create space. */
	deoxrSwapchain( deoxrSession &session, const decPoint &size, eType type );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSwapchain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Swapchain. */
	inline XrSwapchain GetSwapchain() const{ return pSwapchain; }
	
	/** Image format. */
	inline int64_t GetImageFormat() const{ return pImageFormat; }
	
	/** Count of images. */
	inline int GetImageCount() const{ return pImageCount; }
	
	/** Image at index. */
	const sImage &GetImageAt( int index ) const;
	
	/** Acquire image. */
	void AcquireImage();
	
	/** Release image. */
	void ReleaseImage();
	
	/** Acquired image. */
	inline uint32_t GetAcquiredImage() const{ return pAcquiredImage; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetImages();
};

#endif

