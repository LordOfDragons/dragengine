/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SETEXTURE_H_
#define _SETEXTURE_H_

#include "../property/sePropertyList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class seSkin;

class deEngine;
class deSkin;



/**
 * @brief Skin Texture.
 */
class seTexture : public deObject{
private:
	deEngine *pEngine;
	deSkin *pEngSkin;
	
	seSkin *pSkin;
	
	decString pName;
	
	sePropertyList pPropertyList;
	seProperty *pActiveProperty;
	
	decVector2 pTexCoordOffset;
	decVector2 pTexCoordScaling;
	float pTexCoordRotation;
	
	bool pSelected;
	bool pActive;
	
	bool pDirtySkin;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture. */
	seTexture( deEngine *engine, const char *name = "Texture" );
	/** Creates a new texture as a copy of another texture. */
	seTexture( const seTexture &texture );
	/** Cleans up the texture. */
	virtual ~seTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** \brief Retrieves the texture engine skin or NULL if not created. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	
	/** Retrieves the parent skin or NULL if there is none. */
	inline seSkin *GetSkin() const{ return pSkin; }
	/** Sets the parent skin or NULL if there is none. */
	void SetSkin( seSkin *skin );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	
	/** Determines if the layer is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if the layer is the active one. */
	void SetActive( bool active );
	/** Determines if the layer is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** Sets if the layer is selected. */
	void SetSelected( bool selected );
	
	/** Invalidate engine skin. */
	void InvalidateEngineSkin();
	/** Update engine skin if required. */
	void UpdateEngineSkin();
	/** Assign skin to the matching component texture if possible. */
	void AssignSkinToComponentTexture();
	
	/** \brief Assign skin to light if possible. */
	void AssignSkinToLight();
	
	/** Notifies the listeners that the texture changed. */
	void NotifyChanged();
	/** Notifies the listeners that the texture name changed. */
	void NotifyNameChanged();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the property list read-only. */
	inline const sePropertyList &GetPropertyList() const{ return pPropertyList; }
	/** Adds a new property. */
	void AddProperty( seProperty *property );
	/** Removes a property. */
	void RemoveProperty( seProperty *property );
	/** Removes all properties. */
	void RemoveAllProperties();
	/** Retrieves the active property or NULL if none is active. */
	inline seProperty *GetActiveProperty() const{ return pActiveProperty; }
	/** Determines if there is an active property or not. */
	bool HasActiveProperty() const;
	/** Sets the active property or NULL if none is active. */
	void SetActiveProperty( seProperty *property );
	
	/**
	 * \brief Update all resources.
	 * \details Called after skin file path and directory changed for example after SetFilePath.
	 */
	void UpdateResources();
	/*@}*/
	
	
	
	/** \name Preview Parameters */
	/*@{*/
	/** \brief Texture coordinates offset. */
	inline const decVector2 &GetTexCoordOffset() const{ return pTexCoordOffset; }
	
	/** \brief Set texture coordinates offset. */
	void SetTexCoordOffset( const decVector2 &offset );
	
	/** \brief Texture coordinates scaling. */
	inline const decVector2 &GetTexCoordScaling() const{ return pTexCoordScaling; }
	
	/** \brief Set texture coordinates scaling. */
	void SetTexCoordScaling( const decVector2 &scaling );
	
	/** \brief Texture coordinates rotation. */
	inline float GetTexCoordRotation() const{ return pTexCoordRotation; }
	
	/** \brief Set texture coordinates rotation. */
	void SetTexCoordRotation( float rotation );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateTexCoordTransform();
	decTexMatrix2 pCalcTexCoordTransform() const;
};

#endif
