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
public:
	typedef deTObjectReference<seTexture> Ref;
	
	
	
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
