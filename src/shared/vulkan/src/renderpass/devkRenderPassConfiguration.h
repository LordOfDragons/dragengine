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

#ifndef _DEVKRENDERPASSCONFIGURATION_H_
#define _DEVKRENDERPASSCONFIGURATION_H_

#include "../devkBasics.h"

#include <dragengine/common/math/decMath.h>


/**
 * Vulkan render pass configuration.
 */
class devkRenderPassConfiguration{
public:
	/** Enter type. */
	enum pInType{
		eitClear,
		eitLoad,
		eitDiscard
	};
	
	/** End type. */
	enum eOutType{
		eotRender,
		eotShader,
		eotReadBack,
		eotDisplay,
		eotDiscard
	};
	
	/** Sub pass configuration. */
	struct sSubPass{
		int colorAttachments[ 8 ];
		int depthStencilAttachment;
	};
	
	
	
private:
	int pAttachmentCount;
	VkAttachmentDescription *pAttachments;
	VkClearValue *pClearValues;
	int pSubPassCount;
	sSubPass *pSubPasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render pass configuration. */
	devkRenderPassConfiguration();
	
	/** Create copy of render pass configuration. */
	devkRenderPassConfiguration( const devkRenderPassConfiguration &configuration );
	
	/** Clean up render pass configuration. */
	~devkRenderPassConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Attachment count. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	
	/** Set attachment count. */
	void SetAttachmentCount( int count );
	
	/** Attachment at index. */
	const VkAttachmentDescription &GetAttachmentAt( int index ) const;
	
	/** Set attachment at index. */
	void SetAttachmentAt( int index, const VkAttachmentDescription &attachment );
	
	/** Set color attachment at index. */
	void SetColorAttachmentAt( int index, VkFormat format, pInType in, eOutType out );
	
	/** Clear value at index. */
	const VkClearValue &GetClearValueAt( int index ) const;
	
	/** Set clear value at index. */
	void SetClearValueAt( int index, const VkClearValue &clearValue );
	
	/** Set color clear value at index. */
	void SetClearValueColorFloatAt( int index, float red, float green, float blue, float alpha );
	void SetClearValueColorIntAt( int index, int red, int green, int blue, int alpha );
	void SetClearValueColorUIntAt( int index, int red, int green, int blue, int alpha );
	
	/** Set depth/stencil clear value at index. */
	void SetClearValueDepthAt( int index, float depth, int stencil );
	
	/** Internal use only. */
	inline const VkAttachmentDescription *GetAttachments() const{ return pAttachments; }
	inline const VkClearValue *GetClearValues() const{ return pClearValues; }
	
	
	
	/** Sub pass count. */
	inline int GetSubPassCount() const{ return pSubPassCount; }
	
	/** Set sub pass count. */
	void SetSubPassCount( int count );
	
	/** Sub pass at index. */
	const sSubPass &GetSubPassAt( int index ) const;
	
	/** Set sub pass at index. */
	void SetSubPassAt( int index, const sSubPass &subpass );
	
	/** Set sub pass at index. */
	void SetSubPassAt( int index, int depthStencil, int color1 = -1, int color2 = -1,
		int color3 = -1, int color4 = -1, int color5 = -1, int color6 = -1,
		int color7 = -1, int color8 = -1 );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkRenderPassConfiguration &configuration ) const;
	
	/** Copy configuration. */
	devkRenderPassConfiguration &operator=( const devkRenderPassConfiguration &configuration );
	/*@}*/
};

#endif

