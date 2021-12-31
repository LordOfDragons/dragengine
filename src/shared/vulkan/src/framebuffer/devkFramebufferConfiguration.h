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

#ifndef _DEVKFRAMEBUFFERCONFIGURATION_H_
#define _DEVKFRAMEBUFFERCONFIGURATION_H_

#include "../devkBasics.h"
#include "../image/devkImageView.h"

#include <dragengine/common/math/decMath.h>


/**
 * Vulkan framebuffer configuration.
 */
class devkFramebufferConfiguration{
private:
	int pAttachmentCount;
	devkImageView::Ref *pAttachments;
	decPoint pSize;
	int pLayerCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create framebuffer configuration. */
	devkFramebufferConfiguration();
	
	/** Create copy of framebuffer configuration. */
	devkFramebufferConfiguration( const devkFramebufferConfiguration &configuration );
	
	/** Clean up framebuffere configuration. */
	~devkFramebufferConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Attachment count. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	
	/** Set attachment count. */
	void SetAttachmentCount( int count );
	
	/** Attachment at index. */
	devkImageView *GetAttachmentAt( int index ) const;
	
	/** Set attachment at index. */
	void SetAttachmentAt( int index, devkImageView *attachment );
	
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize( const decPoint &size );
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Set layer count. */
	void SetLayerCount( int count );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkFramebufferConfiguration &configuration ) const;
	
	/** Copy configuration. */
	devkFramebufferConfiguration &operator=( const devkFramebufferConfiguration &configuration );
	/*@}*/
};

#endif

