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

#ifndef _MEOBJECTTEXTURE_H_
#define _MEOBJECTTEXTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObject;

class igdeEnvironment;

class deDynamicSkin;
class deSkin;



/**
 * \brief Object texture.
 */
class meObjectTexture : public deObject{
private:
	igdeEnvironment *pEnvironment;
	
	meObject *pObject;
	
	decString pName;
	decString pSkinPath;
	deSkin *pEngSkin;
	decVector2 pTexCoordOffset;
	decVector2 pTexCoordScaling;
	float pTexCoordRotation;
	
	decColor pColorTint;
	
	decStringDictionary pProperties;
	decString pActiveProperty;
	
	deDynamicSkin *pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object texture. */
	meObjectTexture( igdeEnvironment *environment, const char *name );
	
	/** \brief Create copy of object texture. */
	meObjectTexture( const meObjectTexture &texture );
	
	/** \brief Clean up object texture. */
	virtual ~meObjectTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Parent object or \em NULL if not set. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Set parent object or \em NULL if not set. */
	void SetObject( meObject *object );
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Skin file path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin file path. */
	void SetSkinPath( const char *skinPath );
	
	/** \brief Load skin. */
	void LoadSkin();
	
	/** \brief Engine skin or \em NULL if not found. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	
	
	
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
	
	
	
	/** \brief Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** \brief Set color tint. */
	void SetColorTint( const decColor &color );
	
	
	
	/** \brief Dynamic skin or \em NULL if not used. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Update dynamic skin. */
	void UpdateDynamicSkin();
	
	
	
	/** \brief Notify world texture changed. */
	void NotifyChanged();
	
	/** \brief Notify world texture properties changed. */
	void NotifyPropertiesChanged();
	
	/** \brief Notify world texture active property changed. */
	void NotifyActivePropertyChanged();
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set property. */
	void SetProperty( const char *key, const char *value );
	
	/** \brief Set properties. */
	void SetProperties( const decStringDictionary &properties );
	
	/** \brief Remove property if present. */
	void RemoveProperty( const char *key );
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** \brief Set active property. */
	void SetActiveProperty( const char *property );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
