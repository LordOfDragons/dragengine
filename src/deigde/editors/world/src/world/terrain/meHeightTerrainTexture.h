/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHEIGHTTERRAINTEXTURE_H_
#define _MEHEIGHTTERRAINTEXTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/skin/deSkinReference.h>

class meHeightTerrainSector;
class deEngine;
class deHeightTerrainTexture;



/**
 * @brief Height Terrain Texture.
 *
 * Texture layer in a height terrain.
 */
class meHeightTerrainTexture : public deObject{
private:
	deEngine *pEngine;
	deHeightTerrainTexture *pEngTexture;
	
	meHeightTerrainSector *pSector;
	
	decString pName;
	int pTypeNumber;
	
	decString pPathSkin;
	deSkinReference pSkin;
	
	// projection
	decVector2 pProjOffset;
	decVector2 pProjScaling;
	float pProjRotation;
	
	// mask
	decString pPathMask;
	deImageReference pMaskImage;
	bool pMaskChanged;
	bool pMaskSaved;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHeightTerrainTexture( deEngine *engine, const char *name );
	/** Cleans up the object. */
	virtual ~meHeightTerrainTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the texture or NULL. */
	inline deHeightTerrainTexture *GetEngineTexture() const{ return pEngTexture; }
	/** Creates an engine texture based on the settings in this texture. */
	deHeightTerrainTexture *CreateEngineTexture() const;
	/** Sets the texture or NULL. */
	void SetEngineTexture( deHeightTerrainTexture *engTexture );
	
	/** Retrieves the parent height terrain sector or NULL. */
	inline meHeightTerrainSector *GetSector() const{ return pSector; }
	/** Sets the parent height terrain sector or NULL. */
	void SetSector( meHeightTerrainSector *sector );
	
	/** Retrieves the name of the texture. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name of the texture. */
	void SetName( const char *name );
	/** Retrieves the type number. */
	inline int GetTypeNumber() const{ return pTypeNumber; }
	/** Sets the type number. */
	void SetTypeNumber( int typeNumber );
	
	/** Retrieves the skin file path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin file path. */
	void SetPathSkin( const char *path );
	
	/** Retrieves the projection offset in u direction. */
	inline float GetProjectionOffsetU() const{ return pProjOffset.x; }
	/** Sets the projection offset in u direction. */
	void SetProjectionOffsetU( float offset );
	/** Retrieves the projection offset in v direction. */
	inline float GetProjectionOffsetV() const{ return pProjOffset.y; }
	/** Sets the projection offset in v direction. */
	void SetProjectionOffsetV( float offset );
	/** Retrieves the projection scaling in u direction. */
	inline float GetProjectionScalingU() const{ return pProjScaling.x; }
	/** Sets the projection scaling in u direction. */
	void SetProjectionScalingU( float scaling );
	/** Retrieves the projection scaling in v direction. */
	inline float GetProjectionScalingV() const{ return pProjScaling.y; }
	/** Sets the projection scaling in v direction. */
	void SetProjectionScalingV( float scaling );
	/** Retrieves the projection rotation. */
	inline float GetProjectionRotation() const{ return pProjRotation; }
	/** Sets the projection rotation. */
	void SetProjectionRotation( float rotation );
	
	/** Retrieves the mask path. */
	inline const decString &GetPathMask() const{ return pPathMask; }
	/** Sets the mask path. */
	void SetPathMask( const char *path, bool load );
	/** Retrieves the mask image. */
	inline deImage *GetMaskImage() const{ return pMaskImage; }
	
	/** \brief Get mask image creating it if NULL. */
	deImage *GetOrAddMaskImage();
	
	/** Sets the mask image. */
	void SetMaskImage( deImage *image );
	/** Loads the mask image using the mask path. */
	void LoadMaskImage();
	
	/** Determines if the mask changed. */
	inline bool GetMaskChanged() const{ return pMaskChanged; }
	/** Sets if the mask changed. */
	void SetMaskChanged( bool changed );
	/** Determines if the mask has been saved. */
	inline bool GetMaskSaved() const{ return pMaskSaved; }
	/** Sets if the mask has been saved. */
	void SetMaskSaved( bool saved );
	
	/** Notifies that the texture changed. */
	void NotifyTextureChanged();
	/** Notifies that the texture mask changed. */
	void NotifyTextureMaskChanged();
	/*@}*/
	
private:
	void pCleanUp();
	void pLoadSkin();
};

// end of include only once
#endif
