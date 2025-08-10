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
 * Required channel format visitor.
 */
class deoglVSDetermineChannelFormat{
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
	deoglSkinChannel::eTextureType pTextureType;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglVSDetermineChannelFormat( deoglRenderThread &renderThread, const deoglRSkin &skin,
		const deoglSkinTexture &oglTex, const deSkinTexture &tex );
	
	/** Clean up visitor. */
	virtual ~deoglVSDetermineChannelFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Process channel to determine required channel format. */
	void ProcessChannel( deoglSkinChannel::eChannelTypes channel );
	
	
	
	/** Channel has defined format. */
	inline bool GetIsDefined() const{ return pIsDefined; }
	
	/** Required texture size. */
	inline const decPoint3 &GetRequiredSize() const{ return pRequiredSize; }
	
	/** Required component count. */
	inline int GetRequiredComponentCount() const{ return pRequiredComponentCount; }
	
	/** Float format is required. */
	inline bool GetRequiresFloat() const{ return pRequiresFloat; }
	
	/** Channel is uniform. */
	inline bool GetIsUniform() const{ return pIsUniform; }
	
	/** Channel is dynamic. */
	inline bool GetIsDynamic() const{ return pIsDynamic; }
	
	/** Mip mapping is allowed. */
	inline bool GetAllowMipMap() const{ return pAllowMipMap; }
	
	/** Shared image. */
	inline deoglRImage *GetSharedImage() const{ return pSharedImage; }
	
	/** Texture type. */
	inline deoglSkinChannel::eTextureType GetTextureType() const{ return pTextureType; }
	
	
	
	/** Visit property. */
	void VisitProperty( deSkinProperty &property );
	
	/** Set required size. */
	bool SetRequiredSize( const decPoint3 &size );
	
	/** Set use image if possible. */
	void SetSharedImage( deoglRImage *image );
	
	/** Log warning for an image not having square size. */
	void WarnImageNotSquareSize( const deSkinPropertyImage &property ) const;
	
	/** Log warning for an image not support for omnidirection mapping. */
	void WarnImageNotOmnidirectional( const deSkinPropertyImage &property ) const;
	
	/** Log warning for an image with a size not matching the other images in a combined channel. */
	void WarnImageIncompatibleSize( const deSkinPropertyImage &property ) const;
	
	/** Log warning for an image with an unsupported number of components. */
	void WarnImageIncompatibleComponentCount( const deSkinPropertyImage &property, int componentCount ) const;
	
	/** Log warning for an image with a size not matching the other images in a combined channel. */
	void WarnImageIncompatibleSize( const deSkinPropertyConstructed &property ) const;
	
	/** Log warning for an image not having square size. */
	void WarnImageNotSquareSize( const deSkinPropertyConstructed &property ) const;
	
	/** Log warning for an image not support for omnidirection mapping. */
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
	
	void pSetFromNoSize( int requiredComponentCount, int requiredDepth = 1 );
	void pSetFromImage( const deSkinPropertyImage &property, deoglRImage *image, int requiredComponentCount = 1 );
	void pSetFromImageCube( const deSkinPropertyImage &property, deoglRImage *image, int requiredComponentCount = 1 );
	void pSetFromConstructed( const deSkinPropertyConstructed &property, int requiredComponentCount );
	void pSetFromConstructedCube( const deSkinPropertyConstructed &property, int requiredComponentCount );
};

#endif
