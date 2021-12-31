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

#ifndef _DEVKIMAGECONFIGURATION_H_
#define _DEVKIMAGECONFIGURATION_H_

#include "../devkBasics.h"
#include <dragengine/common/math/decMath.h>


/**
 * Vulkan image configuration.
 */
class devkImageConfiguration{
private:
	VkImageType pType;
	decPoint3 pSize;
	int pLayerCount;
	int pMipMapCount;
	VkFormat pFormat;
	VkImageCreateFlags pFlags;
	VkSampleCountFlagBits pSamples;
	VkImageUsageFlags pUsage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image configuration. */
	devkImageConfiguration();
	
	/** Create copy of image configuration. */
	devkImageConfiguration( const devkImageConfiguration &configuration );
	
	/** Clean up image configuration. */
	~devkImageConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Type. */
	inline VkImageType GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( VkImageType type );
	
	/** Size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize( const decPoint3 &size );
	
	/** Format. */
	inline VkFormat GetFormat() const{ return pFormat; }
	
	/** Set format. */
	void SetFormat( VkFormat format );
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Set layer count. */
	void SetLayerCount( int count );
	
	/** Mip map count. */
	inline int GetMipMapCount() const{ return pMipMapCount; }
	
	/** set mip map count. */
	void SetMipMapCount( int count );
	
	/** Flags. */
	inline VkImageCreateFlags GetFlags() const{ return pFlags; }
	
	/** Set flags. */
	void SetFlags( VkImageCreateFlags flags );
	
	/** Samples. */
	inline VkSampleCountFlagBits GetSamples() const{ return pSamples; }
	
	/** Set samples. */
	void SetSamples( VkSampleCountFlagBits samples );
	
	/** Usage. */
	inline VkImageUsageFlags GetUsage() const{ return pUsage; }
	
	/** Set usage. */
	void SetUsage( VkImageUsageFlags usage );
	
	
	
	/** Set 2D image. */
	void Set2D( const decPoint &size, VkFormat format );
	
	/** Set 3D image. */
	void Set3D( const decPoint3 &size, VkFormat format );
	
	/** Set Cube image. */
	void SetCube( int size, VkFormat format );
	
	/** Set 2D Array image. */
	void Set2DArray( const decPoint &size, int layerCount, VkFormat format );
	
	/** Set Cube Array image. */
	void SetCubeArray( int size, int layerCount, VkFormat format );
	
	
	
	/** Disable all usage. */
	void DisableAll();
	
	/** Enable/disable usage source/destination transfer. */
	void EnableTransfer( bool enable );
	
	/** Enable/disable usage source transfer. */
	void EnableTransferSource( bool enable );
	
	/** Enable/disable usage destination transfer. */
	void EnableTransferDestination( bool enable );
	
	/** Enable/disable usage sampled. */
	void EnableSampled( bool enable );
	
	/** Enable/disable usage storage. */
	void EnableStorage( bool enable );
	
	/** Enable/disable usage color attachment. */
	void EnableColorAttachment( bool enable );
	
	/** Enable/disable usage depth/stencil attachment. */
	void EnableDepthStencilAttachment( bool enable );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkImageConfiguration &configuration ) const;
	
	/** Copy configuration. */
	devkImageConfiguration &operator=( const devkImageConfiguration &configuration );
	/*@}*/
};

#endif

