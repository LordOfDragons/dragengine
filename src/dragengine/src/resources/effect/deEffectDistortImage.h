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

#ifndef _DEEFFEDISTORTIMAGE_H_
#define _DEEFFEDISTORTIMAGE_H_

#include "deEffect.h"
#include "../image/deImageReference.h"
#include "../../common/math/decMath.h"

class deEffectChain;


/**
 * \brief Effect Distort Image Class.
 *
 * The distort image effect applies a distortion to the rendered image using a
 * normal map type image. The red and green channel are used as an offset to
 * look up the pixel from the rendered image. The offset is first multiplied
 * by the strength factor.
 */
class DE_DLL_EXPORT deEffectDistortImage : public deEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffectDistortImage> Ref;
	
	
	
private:
	deImageReference pImage;
	decVector2 pTexCoords[ 4 ];
	float pStrengthU;
	float pStrengthV;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect distort image. */
	deEffectDistortImage( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up effect distort image.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectDistortImage();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Image or null if not set. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or null to remove it. */
	void SetImage( deImage *image );
	
	/** \brief Texture coordinates of the given corner. */
	const decVector2 &GetTextureCoordinatesFor( int corner ) const;
	
	/** \brief Set texture coordinates if the given corner. */
	void SetTextureCoordinatesFor( int corner, const decVector2 &textureCoordinates );
	
	/** \brief Strength in u direction. */
	inline float GetStrengthU() const{ return pStrengthU; }
	
	/** \brief Set strength in u direction. */
	void SetStrengthU( float strength );
	
	/** \brief Strength in v direction. */
	inline float GetStrengthV() const{ return pStrengthV; }
	
	/** \brief Set strength in v direction. */
	void SetStrengthV( float strength );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
};

#endif
