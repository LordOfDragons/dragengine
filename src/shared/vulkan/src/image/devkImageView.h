/* 
 * Drag[en]gine Shared Vulkan
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

#ifndef _DEVKIMAGEVIEW_H_
#define _DEVKIMAGEVIEW_H_

#include "devkImage.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>


/**
 * Vulkan render pass.
 */
class devkImageView : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkImageView> Ref;
	
	
	
protected:
	const devkImage::Ref pImage;
	
	VkImageView pImageView;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image view. */
	devkImageView( devkImage *image );
	
	/** Create image view. */
	devkImageView( devkImage *image, int layer );
	
	/** Create image view. */
	devkImageView( devkImage *image, int layer, int level );
	
protected:
	/** Clean up pipeline. */
	virtual ~devkImageView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Image. */
	inline devkImage *GetImage() const{ return pImage; }
	
	/** Image view. */
	inline VkImageView GetImageView() const{ return pImageView; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

