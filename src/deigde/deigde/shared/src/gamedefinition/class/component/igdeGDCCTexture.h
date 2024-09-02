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

#ifndef _IGDEGDCCTEXTURE_H_
#define _IGDEGDCCTEXTURE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>



/**
 * \brief Game Definition Class Component Texture.
 */
class DE_DLL_EXPORT igdeGDCCTexture : public deObject{
private:
	decString pName;
	decString pPathSkin;
	decVector2 pOffset;
	decVector2 pScale;
	float pRotation;
	decColor pColorTint;
	decStringDictionary pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture definition. */
	igdeGDCCTexture();
	
	/** \brief Create copy of texture definition. */
	igdeGDCCTexture( const igdeGDCCTexture &texture );
	
	/** \brief Clean up texture definition. */
	virtual ~igdeGDCCTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of texture to replace. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of texture to replace. */
	void SetName( const char *name );
	
	/** \brief Path of skin to use. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	
	/** \brief Set path of skin to use. */
	void SetPathSkin( const char *path );
	
	/** \brief Texture coordinate offset. */
	inline const decVector2 &GetOffset() const{ return pOffset; }
	
	/** \brief Set texture coordinate offset. */
	void SetOffset( const decVector2 &offset );
	
	/** \brief Texture coordinate scale. */
	inline const decVector2 &GetScale() const{ return pScale; }
	
	/** \brief Set texture coordinate scale. */
	void SetScale( const decVector2 &scale );
	
	/** \brief Texture coordinate rotation. */
	inline float GetRotation() const{ return pRotation; }
	
	/** \brief Set texture coordinate rotation. */
	void SetRotation( float rotation );
	
	/** \brief Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** \brief Set color tint. */
	void SetColorTint( const decColor &color );
	
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set properties. */
	void SetProperties( const decStringDictionary &properties );
	/*@}*/
};

#endif
