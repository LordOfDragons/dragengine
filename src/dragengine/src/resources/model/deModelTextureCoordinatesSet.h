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

#ifndef _DEMODELTEXTURECOORDINATESSET_H_
#define _DEMODELTEXTURECOORDINATESSET_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model Texture Coordinates Set.
 * Defines an additional named set of texture coordinates for all faces
 * in the model.
 */
class DE_DLL_EXPORT deModelTextureCoordinatesSet{
private:
	decVector2 *pTextureCoordinates;
	int pTextureCoordinatesCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model texture coordinate set. */
	deModelTextureCoordinatesSet();
	
	/** \brief Clean up model texture coordinate set. */
	~deModelTextureCoordinatesSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of texture coordinates. */
	inline int GetTextureCoordinatesCount() const{ return pTextureCoordinatesCount; }
	
	/** \brief Set number of texture coordinates. */
	void SetTextureCoordinatesCount(int count);
	
	/** \brief Texture coordinates by index. */
	const decVector2 &GetTextureCoordinatesAt(int index) const;
	
	/** \brief Set texture coordinates  by index. */
	void SetTextureCoordinatesAt(int index, const decVector2 &textureCoordinates);
	
	/** \brief Pointer to the texture coordinates. */
	inline decVector2 *GetTextureCoordinates() const{ return pTextureCoordinates; }
	/*@}*/
};

#endif
