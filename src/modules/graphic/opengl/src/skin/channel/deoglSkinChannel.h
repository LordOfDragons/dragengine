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
 * \brief Skin channel.
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
	/** \brief Channel types. */
	enum eChannelTypes{
		/** \brief Color channel with combined texture with color as rgb transparency as alpha. */
		ectColor,
		
		/** \brief Color tint mask channel with direct r texture. */
		ectColorTintMask,
		
		/** \brief Transparency channel with combined with transparency. */
		ectTransparency,
		
		/** \brief Omni-directional color channel with cube map. */
		ectColorOmnidirCube,
		
		/** \brief Omni-directional color channel with equirect map. */
		ectColorOmnidirEquirect,
		
		/** \brief Solidity channel with direct r texture. */
		ectSolidity,
		
		/** \brief Normal channel with normal as rgb and optional normal deviation angle as alpha. */
		ectNormal,
		
		/** \brief Height map with optional tech specific map as rg. */
		ectHeight,
		
		/** \brief Emission channel with combined texture with color as rgb and strength as alpha. */
		ectEmissivity,
		
		/** \brief Refraction distortion channel with direct rg(b) texture. */
		ectRefractDistort,
		
		/** \brief Ambient occlusion channel with direct r texture. */
		ectAO,
		
		/** \brief Reflectivity channel with reflectivity as rgb. */
		ectReflectivity,
		
		/** \brief Roughness channel with roughness as red. */
		ectRoughness,
		
		/** \brief Environment map channel with cube map texture. */
		ectEnvironmentMap,
		
		/** \brief Environment room channel with cube map texture. */
		ectEnvironmentRoom,
		
		/** \brief Environment room mask channel with r texture. */
		ectEnvironmentRoomMask,
		
		/** \brief Environment room emissivity channel with cube map texture. */
		ectEnvironmentRoomEmissivity,
		
		/** \brief Absorption. */
		ectAbsorption,
		
		/** \brief Rim emission channel. */
		ectRimEmissivity,
	};
	
	/** \brief Number of channels. */
	static const int CHANNEL_COUNT = ectRimEmissivity + 1;
	
	
	
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
	/** \brief Create skin channel. */
	deoglSkinChannel( deoglRenderThread &renderThread, eChannelTypes type );
	
	/** \brief Clean up skin channel. */
	~deoglSkinChannel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	
	/** \brief Full quality size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** \brief Set full quality size. */
	void SetSize( const decPoint3 &size );
	
	/** \brief Retrieves the component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Sets the component count. */
	void SetComponentCount( int componentCount );
	
	
	
	/** \brief Texture scaling factor in u direction. */
	inline float GetFactorU() const{ return pFactorU; }
	
	/** \brief Set texture scaling factor in u direction. */
	void SetFactorU( float factor );
	
	/** \brief Texture scaling factor in v direction. */
	inline float GetFactorV() const{ return pFactorV; }
	
	/** \brief Set texture scaling factor in v direction. */
	void SetFactorV( float factor );
	
	/** \brief Texture or \em NULL if this is not used. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Cube map or \em NULL if this is not used. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** \brief Array texture or \em NULL if this is not used. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** \brief Combined texture or \em NULL if not used. */
	inline deoglCombinedTexture *GetCombinedTexture() const{ return pCombinedTexture; }
	
	/** \brief Set combined texture or \em NULL if not used. */
	void SetCombinedTexture( deoglCombinedTexture *combinedTexture );
	
	
	
	/** \brief Pixel buffer mip map or \em NULL if not existing. */
	inline deoglPixelBufferMipMap *GetPixelBufferMipMap() const{ return pPixelBufferMipMap; }
	
	/** \brief Set pixel buffer mip map or \em NULL if not existing. */
	void SetPixelBufferMipMap( deoglPixelBufferMipMap *pbmipmap );
	
	/** \brief Texture data is cached. */
	inline bool GetIsCached() const{ return pIsCached; }
	
	/** \brief Set if texture data is cached. */
	void SetIsCached( bool isCached );
	
	/** \brief Texture can be cached. */
	inline bool GetCanBeCached() const{ return pCanBeCached; }
	
	/** \brief Set if texture can be cached. */
	void SetCanBeCached( bool canBeCached );
	
	/** \brief Cache identifier. */
	inline const decString &GetCacheID() const{ return pCacheID; }
	
	/** \brief Set cache identifier. */
	void SetCacheID( const char *cacheID );
	
	/** \brief Cache verify data or \em NULL if absent. */
	inline const decMemoryFile *GetCacheVerify() const{ return pCacheVerify; }
	
	
	
	/** \brief Uniform color. */
	inline const decColor &GetUniformColor() const{ return pUniformColor; }
	
	/** \brief Set uniform color. */
	void SetUniformColor( const decColor &color );
	
	/** \brief Color component is static. */
	bool IsComponentStatic( int component ) const;
	
	/** \brief At least one color component is static. */
	bool HasStaticComponent() const;
	
	/** \brief All color components are static. */
	bool AllComponentsStatic() const;
	
	/** \brief All color components are not static. */
	bool AllComponentsNotStatic() const;
	
	/** \brief First combined image or \em NULL if not existing. */
	inline deoglRImage *GetCombinedImage1() const{ return pCombinedImage1; }
	
	/** \brief Second combined image or \em NULL if not existing. */
	inline deoglRImage *GetCombinedImage2() const{ return pCombinedImage2; }
	
	/** \brief First delayed combined image or \em NULL if not existing. */
	inline deoglImage *GetDelayedCombinedImage1() const{ return pDelayedCombineImage1; }
	
	/** \brief Second delayed combined image or \em NULL if not existing. */
	inline deoglImage *GetDelayedCombinedImage2() const{ return pDelayedCombineImage2; }
	
	
	
	/** \brief Channel is purely a uniform color. */
	inline bool GetUniform() const{ return pUniform; }
	
	/** \brief Set if channel is purely a uniform color. */
	void SetUniform( bool uniform );
	
	/** \brief Channel has dynamic content. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** \brief Set channel has dynamic content. */
	void SetDynamic( bool dynamic );
	
	
	
	/** \brief Image or \em NULL to use the texture stored locally. */
	inline deoglRImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or \em NULL to use the texture stored locally. */
// 	void SetImage( deoglRImage *image );
	
	/**
	 * \brief Video or \em NULL to use the texture stored locally.
	 * \details This has to be called only by synchronization in main thread. The video pointer
	 *          is only valid as long as deoglSkin is existing.
	 */
	inline deVideo *GetVideo() const{ return pVideo; }
	
	/**
	 * \brief Set video or \em NULL to use the texture stored locally.
	 * \details This has to be called only by synchronization in main thread. The video pointer
	 *          is only valid as long as deoglSkin is existing.
	 */
	void SetVideo( deVideo *video );
	
	/** \brief Video player index or -1 if not used. */
	inline int GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Set video player index or -1 if not used. */
	void SetVideoPlayer( int index );
	
	/** \brief Video player is shared. */
	inline bool GetSharedVideoPlayer() const{ return pSharedVideoPlayer; }
	
	/** \brief Set if video player is shared. */
	void SetSharedVideoPlayer( bool shared );
	
	/** \brief Index of the renderable or -1 if not dynamic. */
	inline int GetRenderable() const{ return pRenderable; }
	
	/** \brief Set index of the renderable or -1 if not dynamic. */
	void SetRenderable( int index );
	
	
	
	/**
	 * \brief Prepare for building channel.
	 * \details Creates the required texture object and cache parameters required for loading caches.
	 */
	void PrepareChannel( deoglRSkin &skin, deoglSkinTexture &texture, const deSkinTexture &engTexture,
		const deoglVSDetermineChannelFormat &channelFormat );
	
	/**
	 * \brief Build channel but does not create anything requiring opengl calls.
	 * \details Call after loading caches if cache ID is empty or cache is outdated or missing.
	 */
	void BuildChannel( const deSkinTexture &engTexture );
	
	/** \brief Generate cone map. */
	void GenerateConeMap();
	
	/** \brief Clear cache data build during a call to PrepareChannel not required anymore. */
	void ClearCacheData();
	
	/**
	 * \brief Use shared image.
	 * 
	 * If shared image is not in use by a skin moves textures to shared image. Otherwise deletes
	 * textures without using delayed deletion to avoid dead-locking.
	 * 
	 * \warning Called from deoglDelayedOperations only. Runs in render thread.
	 */
	void UseSharedImage();
	
	
	
	/** \brief Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	
	
	
	/** \brief Channel name for logging and debugging. */
	static const char *ChannelNameFor( eChannelTypes type );
	
	/**
	 * \brief Channel type matching property type.
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
