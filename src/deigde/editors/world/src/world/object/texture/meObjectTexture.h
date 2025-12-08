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

#ifndef _MEOBJECTTEXTURE_H_
#define _MEOBJECTTEXTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObject;

class igdeEnvironment;

#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/deSkin.h>



/**
 * \brief Object texture.
 */
class meObjectTexture : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<meObjectTexture> Ref;
	
	
private:
	igdeEnvironment *pEnvironment;
	
	meObject *pObject;
	
	decString pName;
	decString pSkinPath;
	deSkin::Ref pEngSkin;
	decVector2 pTexCoordOffset;
	decVector2 pTexCoordScaling;
	float pTexCoordRotation;
	
	decColor pColorTint;
	
	decStringDictionary pProperties;
	decString pActiveProperty;
	
	deDynamicSkin::Ref pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object texture. */
	meObjectTexture(igdeEnvironment *environment, const char *name);
	
	/** \brief Create copy of object texture. */
	meObjectTexture(const meObjectTexture &texture);
	
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
	void SetObject(meObject *object);
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Skin file path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin file path. */
	void SetSkinPath(const char *skinPath);
	
	/** \brief Load skin. */
	void LoadSkin();
	
	/** \brief Engine skin or \em NULL if not found. */
	inline const deSkin::Ref &GetEngineSkin() const{ return pEngSkin; }
	
	
	
	/** \brief Texture coordinates offset. */
	inline const decVector2 &GetTexCoordOffset() const{ return pTexCoordOffset; }
	
	/** \brief Set texture coordinates offset. */
	void SetTexCoordOffset(const decVector2 &offset);
	
	/** \brief Texture coordinates scaling. */
	inline const decVector2 &GetTexCoordScaling() const{ return pTexCoordScaling; }
	
	/** \brief Set texture coordinates scaling. */
	void SetTexCoordScaling(const decVector2 &scaling);
	
	/** \brief Texture coordinates rotation. */
	inline float GetTexCoordRotation() const{ return pTexCoordRotation; }
	
	/** \brief Set texture coordinates rotation. */
	void SetTexCoordRotation(float rotation);
	
	
	
	/** \brief Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** \brief Set color tint. */
	void SetColorTint(const decColor &color);
	
	
	
	/** \brief Dynamic skin or \em NULL if not used. */
	inline const deDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
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
	void SetProperty(const char *key, const char *value);
	
	/** \brief Set properties. */
	void SetProperties(const decStringDictionary &properties);
	
	/** \brief Remove property if present. */
	void RemoveProperty(const char *key);
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** \brief Set active property. */
	void SetActiveProperty(const char *property);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
