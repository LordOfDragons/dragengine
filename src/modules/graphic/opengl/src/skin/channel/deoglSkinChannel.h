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

#ifndef _DEOGLSKINCHANNEL_H_
#define _DEOGLSKINCHANNEL_H_

#include "../deoglSkinPropertyMap.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglArrayTexture;
class deoglCombinedTexture;
class deoglCubeMap;
class deoglImage;
class deoglPixelBuffer;
class deoglPixelBufferMipMap;
class deoglRenderThread;
class deoglRImage;
class deoglRSkin;
class deoglSkinTexture;
class deoglTexture;
class deoglVSDetermineChannelFormat;

class deFileResource;
class deImage;
class deSkinProperty;
class deSkinPropertyValue;
class deSkinPropertyColor;
class deSkinPropertyImage;
class deSkinPropertyVideo;
class deSkinPropertyConstructed;
class deSkinPropertyMapped;
class deSkinTexture;
class deVideo;
class decMemoryFile;



/**
 * Skin channel.
 * 
 * Stores the texture for a skin channel. The channel texture is typically a combined
 * texture. The source definition contains the image objects or color values required
 * to build the final texture. The channel texture is typically first a down scaled
 * version for faster loading to the GPU. Later on the texture is rebuild using the
 * full quality and used once finished. This allows a fast usage of the channel texture
 * without causing shuttering with cards having a slower upload speed. The width and
 * height parameter defines the full quality size of the texture which is not necessarily
 * the same as the dimension of the current texture object. The u and v factor parameters
 * are relative to the current texture object not the full quality size.
 */
class deoglSkinChannel{
public:
	/** Channel types. */
	enum eChannelTypes{
		/** Color (rgb). */
		ectColor,
		
		/** Color tint mask (r). */
		ectColorTintMask,
		
		/** Transparency (r). */
		ectTransparency,
		
		/** Omni-directional color (rgb cube). */
		ectColorOmnidirCube,
		
		/** Omni-directional color equirect map (rgb). */
		ectColorOmnidirEquirect,
		
		/** Solidity (r). */
		ectSolidity,
		
		/** Normal (rgb), optional normal deviation angle (a). */
		ectNormal,
		
		/** Height map (r), optional tech specific (g). */
		ectHeight,
		
		/** Emission color (rgb), emission strength (a). */
		ectEmissivity,
		
		/** Refraction distortion (rg). */
		ectRefractDistort,
		
		/** Ambient occlusion (r). */
		ectAO,
		
		/** Reflectivity (rgb). */
		ectReflectivity,
		
		/** Roughness (r). */
		ectRoughness,
		
		/** Environment map (rgb cube). */
		ectEnvironmentMap,
		
		/** Environment room (rgb cube). */
		ectEnvironmentRoom,
		
		/** Environment room mask (r). */
		ectEnvironmentRoomMask,
		
		/** Environment room emissivity (rgb cube). */
		ectEnvironmentRoomEmissivity,
		
		/** Absorption (rgb). */
		ectAbsorption,
		
		/** Rim emission (r). */
		ectRimEmissivity,
		
		/** Non-PBR albedo (rgb). */
		ectNonPbrAlbedo,
		
		/** Non-PBR metalness (r). */
		ectNonPbrMetalness
	};
	
	/** Number of channels. */
	static const int CHANNEL_COUNT = ectNonPbrMetalness + 1;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	eChannelTypes pType;
	decPoint3 pSize;
	int pComponentCount;
	bool pFloatFormat;
	bool pCompressed;
	float pFactorU;
	float pFactorV;
	deoglTexture *pTexture;
	deoglCubeMap *pCubeMap;
	deoglArrayTexture *pArrayTexture;
	deoglCombinedTexture *pCombinedTexture;
	deoglPixelBufferMipMap *pPixelBufferMipMap;
	
	decString pCacheID;
	bool pIsCached;
	bool pCanBeCached;
	decMemoryFile *pCacheVerify;
	decString pCacheIDSource1;
	decString pCacheIDSource2;
	decMemoryFile *pCacheConstrDefSource1;
	decMemoryFile *pCacheConstrDefSource2;
	decMemoryFile *pCacheConstrVerifySource1;
	decMemoryFile *pCacheConstrVerifySource2;
	
	decColor pUniformColor;
	bool pUniformColorMask[ 4 ];
	deoglRImage *pCombinedImage1;
	deoglRImage *pCombinedImage2;
	deoglImage *pDelayedCombineImage1;
	deoglImage *pDelayedCombineImage2;
	
	bool pUniform;
	bool pDynamic;
	
	deoglRImage *pImage;
	bool pImageHeld;
	
	deVideo *pVideo;
	int pVideoPlayer;
	bool pSharedVideoPlayer;
	int pRenderable;
	
	float pSolidityFilterPriority;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin channel. */
	deoglSkinChannel( deoglRenderThread &renderThread, eChannelTypes type );
	
	/** Clean up skin channel. */
	~deoglSkinChannel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	
	/** Full quality size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** Set full quality size. */
	void SetSize( const decPoint3 &size );
	
	/** Retrieves the component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Sets the component count. */
	void SetComponentCount( int componentCount );
	
	
	
	/** Texture scaling factor in u direction. */
	inline float GetFactorU() const{ return pFactorU; }
	
	/** Set texture scaling factor in u direction. */
	void SetFactorU( float factor );
	
	/** Texture scaling factor in v direction. */
	inline float GetFactorV() const{ return pFactorV; }
	
	/** Set texture scaling factor in v direction. */
	void SetFactorV( float factor );
	
	/** Texture or \em NULL if this is not used. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Cube map or \em NULL if this is not used. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** Array texture or \em NULL if this is not used. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** Combined texture or \em NULL if not used. */
	inline deoglCombinedTexture *GetCombinedTexture() const{ return pCombinedTexture; }
	
	/** Set combined texture or \em NULL if not used. */
	void SetCombinedTexture( deoglCombinedTexture *combinedTexture );
	
	
	
	/** Pixel buffer mip map or \em NULL if not existing. */
	inline deoglPixelBufferMipMap *GetPixelBufferMipMap() const{ return pPixelBufferMipMap; }
	
	/** Set pixel buffer mip map or \em NULL if not existing. */
	void SetPixelBufferMipMap( deoglPixelBufferMipMap *pbmipmap );
	
	/** Texture data is cached. */
	inline bool GetIsCached() const{ return pIsCached; }
	
	/** Set if texture data is cached. */
	void SetIsCached( bool isCached );
	
	/** Texture can be cached. */
	inline bool GetCanBeCached() const{ return pCanBeCached; }
	
	/** Set if texture can be cached. */
	void SetCanBeCached( bool canBeCached );
	
	/** Cache identifier. */
	inline const decString &GetCacheID() const{ return pCacheID; }
	
	/** Set cache identifier. */
	void SetCacheID( const char *cacheID );
	
	/** Cache verify data or \em NULL if absent. */
	inline const decMemoryFile *GetCacheVerify() const{ return pCacheVerify; }
	
	
	
	/** Uniform color. */
	inline const decColor &GetUniformColor() const{ return pUniformColor; }
	
	/** Set uniform color. */
	void SetUniformColor( const decColor &color );
	
	/** Color component is static. */
	bool IsComponentStatic( int component ) const;
	
	/** At least one color component is static. */
	bool HasStaticComponent() const;
	
	/** All color components are static. */
	bool AllComponentsStatic() const;
	
	/** All color components are not static. */
	bool AllComponentsNotStatic() const;
	
	/** First combined image or \em NULL if not existing. */
	inline deoglRImage *GetCombinedImage1() const{ return pCombinedImage1; }
	
	/** Second combined image or \em NULL if not existing. */
	inline deoglRImage *GetCombinedImage2() const{ return pCombinedImage2; }
	
	/** First delayed combined image or \em NULL if not existing. */
	inline deoglImage *GetDelayedCombinedImage1() const{ return pDelayedCombineImage1; }
	
	/** Second delayed combined image or \em NULL if not existing. */
	inline deoglImage *GetDelayedCombinedImage2() const{ return pDelayedCombineImage2; }
	
	
	
	/** Channel is purely a uniform color. */
	inline bool GetUniform() const{ return pUniform; }
	
	/** Set if channel is purely a uniform color. */
	void SetUniform( bool uniform );
	
	/** Channel has dynamic content. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** Set channel has dynamic content. */
	void SetDynamic( bool dynamic );
	
	
	
	/** Image or \em NULL to use the texture stored locally. */
	inline deoglRImage *GetImage() const{ return pImage; }
	
	/** Set image or \em NULL to use the texture stored locally. */
// 	void SetImage( deoglRImage *image );
	
	/**
	 * Video or \em NULL to use the texture stored locally.
	 * \details This has to be called only by synchronization in main thread. The video pointer
	 *          is only valid as long as deoglSkin is existing.
	 */
	inline deVideo *GetVideo() const{ return pVideo; }
	
	/**
	 * Set video or \em NULL to use the texture stored locally.
	 * \details This has to be called only by synchronization in main thread. The video pointer
	 *          is only valid as long as deoglSkin is existing.
	 */
	void SetVideo( deVideo *video );
	
	/** Video player index or -1 if not used. */
	inline int GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** Set video player index or -1 if not used. */
	void SetVideoPlayer( int index );
	
	/** Video player is shared. */
	inline bool GetSharedVideoPlayer() const{ return pSharedVideoPlayer; }
	
	/** Set if video player is shared. */
	void SetSharedVideoPlayer( bool shared );
	
	/** Index of the renderable or -1 if not dynamic. */
	inline int GetRenderable() const{ return pRenderable; }
	
	/** Set index of the renderable or -1 if not dynamic. */
	void SetRenderable( int index );
	
	
	
	/**
	 * Prepare for building channel.
	 * \details Creates the required texture object and cache parameters required for loading caches.
	 */
	void PrepareChannel( deoglRSkin &skin, deoglSkinTexture &texture, const deSkinTexture &engTexture,
		const deoglVSDetermineChannelFormat &channelFormat );
	
	/**
	 * Build channel but does not create anything requiring opengl calls.
	 * \details Call after loading caches if cache ID is empty or cache is outdated or missing.
	 */
	void BuildChannel( const deSkinTexture &engTexture );
	
	/** Generate cone map. */
	void GenerateConeMap();
	
	/** Clear cache data build during a call to PrepareChannel not required anymore. */
	void ClearCacheData();
	
	/**
	 * Use shared image.
	 * 
	 * If shared image is not in use by a skin moves textures to shared image. Otherwise deletes
	 * textures without using delayed deletion to avoid dead-locking.
	 * 
	 * \warning Called from deoglDelayedOperations only. Runs in render thread.
	 */
	void UseSharedImage();
	
	
	
	/** Channel name for logging and debugging. */
	static const char *ChannelNameFor( eChannelTypes type );
	
	/**
	 * Channel type matching property type.
	 * \param[in] propertyType Property type to match.
	 * \param[out] channelType Matching channel type. Updated only if \em true is returned.
	 * \returns \em true if matching.
	 */
	static bool ChannelTypeMatchingPropertyType( deoglSkinPropertyMap::ePropertyTypes propertyType,
		eChannelTypes &channelType );
	/*@}*/
	
	
	
private:
	void pInitUniformColor();
	bool pIsPropertyValidForType( deoglSkinPropertyMap::ePropertyTypes propertyType ) const;
	
	
	
	void pCreatePixelBufferMipMap( deoglSkinTexture &texture,
		const deoglVSDetermineChannelFormat &channelFormat );
	
	void pCreateTextureObject( const deoglSkinTexture &texture );
	
	void pPrepareProperty( deoglRSkin &skin, deoglSkinTexture &texture,
		deSkinProperty &property );
	
	void pPreparePropertyValue( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglSkinTexture &texture, const deSkinPropertyValue &property );
	
	void pPreparePropertyColor( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglSkinTexture &texture, const deSkinPropertyColor &property );
	
	void pPreparePropertyImage( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglSkinTexture &texture, const deSkinPropertyImage &property );
	
	void pPreparePropertyVideo( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglRSkin &skin, deoglSkinTexture &texture, const deSkinPropertyVideo &property );
	
	void pPreparePropertyConstructed( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglSkinTexture &texture, const deSkinPropertyConstructed &property );
	
	void pPreparePropertyMapped( deoglSkinPropertyMap::ePropertyTypes propertyType,
		deoglRSkin &skin, deoglSkinTexture &texture, const deSkinPropertyMapped &property );
	
	void pClearUniformMasks( int targetRed, int targetGreen, int targetBlue, int targetAlpha );
	
	decString pCRC32String( const decString &string ) const;
	decString pCRC32Data( const decMemoryFile &data ) const;
	
	decString pColor2CacheBinary( const decColor &color ) const;
	
	void pBuildCacheID();
	
	void pBuildCacheVerify();
	
	
	
	void pBuildProperty( deSkinProperty &property );
	
	void pBuildPropertyImage( deoglSkinPropertyMap::ePropertyTypes propertyType,
		const deSkinPropertyImage &property );
	
	void pBuildPropertyConstructed( deoglSkinPropertyMap::ePropertyTypes propertyType,
		const deSkinPropertyConstructed &property );
	
	void pWriteImageToPixelBuffer( const deImage &image, int targetRed,
		int targetGreen, int targetBlue, int targetAlpha );
	
	void pWriteImageToPixelBuffer( const deImage &image, int srcLayer, int destLayer,
		int targetRed, int targetGreen, int targetBlue, int targetAlpha );
	
	void pFillWithUniformColor();
};

#endif
