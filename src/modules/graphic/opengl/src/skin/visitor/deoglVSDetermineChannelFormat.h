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

#ifndef _DEOGLVSDETERMINECHANNELFORMAT_H_
#define _DEOGLVSDETERMINECHANNELFORMAT_H_

#include "../channel/deoglSkinChannel.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRSkin;
class deoglRImage;
class deoglSkinTexture;

class deSkinProperty;
class deSkinPropertyConstructed;
class deSkinPropertyImage;
class deSkinTexture;



/**
 * \brief Required channel format visitor.
 */
class deoglVSDetermineChannelFormat{
public:
	deoglRenderThread &pRenderThread;
	const deoglRSkin &pSkin;
	const deoglSkinTexture &pOglTex;
	const deSkinTexture &pSkinTex;
	deoglSkinChannel::eChannelTypes pOglChanType;
	
	decPoint3 pRequiredSize;
	bool pIsUniform;
	bool pIsDynamic;
	bool pIsDefined;
	int pRequiredComponentCount;
	bool pRequiresFloat;
	bool pAllowMipMap;
	deoglRImage *pSharedImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoglVSDetermineChannelFormat( deoglRenderThread &renderThread, const deoglRSkin &skin,
		const deoglSkinTexture &oglTex, const deSkinTexture &tex );
	
	/** \brief Clean up visitor. */
	virtual ~deoglVSDetermineChannelFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Process channel to determine required channel format. */
	void ProcessChannel( deoglSkinChannel::eChannelTypes channel );
	
	
	
	/** \brief Channel has defined format. */
	inline bool GetIsDefined() const{ return pIsDefined; }
	
	/** \brief Required texture size. */
	inline const decPoint3 &GetRequiredSize() const{ return pRequiredSize; }
	
	/** \brief Required component count. */
	inline int GetRequiredComponentCount() const{ return pRequiredComponentCount; }
	
	/** \brief Float format is required. */
	inline bool GetRequiresFloat() const{ return pRequiresFloat; }
	
	/** \brief Channel is uniform. */
	inline bool GetIsUniform() const{ return pIsUniform; }
	
	/** \brief Channel is dynamic. */
	inline bool GetIsDynamic() const{ return pIsDynamic; }
	
	/** \brief Mip mapping is allowed. */
	inline bool GetAllowMipMap() const{ return pAllowMipMap; }
	
	/** \brief Shared image. */
	inline deoglRImage *GetSharedImage() const{ return pSharedImage; }
	
	
	
	/** \brief Visit property. */
	void VisitProperty( deSkinProperty &property );
	
	/** \brief Set required size. */
	bool SetRequiredSize( const decPoint3 &size );
	
	/** \brief Set use image if possible. */
	void SetSharedImage( deoglRImage *image );
	
	/** \brief Log warning for an image not having square size. */
	void WarnImageNotSquareSize( const deSkinPropertyImage &property ) const;
	
	/** \brief Log warning for an image not support for omnidirection mapping. */
	void WarnImageNotOmnidirectional( const deSkinPropertyImage &property ) const;
	
	/** \brief Log warning for an image with a size not matching the other images in a combined channel. */
	void WarnImageIncompatibleSize( const deSkinPropertyImage &property ) const;
	
	/** \brief Log warning for an image with an unsupported number of components. */
	void WarnImageIncompatibleComponentCount( const deSkinPropertyImage &property, int componentCount ) const;
	
	/** \brief Log warning for an image with a size not matching the other images in a combined channel. */
	void WarnImageIncompatibleSize( const deSkinPropertyConstructed &property ) const;
	
	/** \brief Log warning for an image not having square size. */
	void WarnImageNotSquareSize( const deSkinPropertyConstructed &property ) const;
	
	/** \brief Log warning for an image not support for omnidirection mapping. */
	void WarnImageNotOmnidirectional( const deSkinPropertyConstructed &property ) const;
	/*@}*/
	
	
	
private:
	bool pChannelMatches( const deoglSkinPropertyMap::ePropertyTypes channelType ) const;
	void pProcessPropertyImage( const deSkinPropertyImage &property,
		const deoglSkinPropertyMap::ePropertyTypes channelType );
	void pProcessPropertyColorVideo( const deoglSkinPropertyMap::ePropertyTypes channelType );
	void pProcessPropertyValue( const deoglSkinPropertyMap::ePropertyTypes channelType );
	void pProcessPropertyConstructed( const deSkinPropertyConstructed &property,
		const deoglSkinPropertyMap::ePropertyTypes channelType );
};

#endif
