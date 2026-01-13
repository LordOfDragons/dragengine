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
#include "../../common/collection/decTList.h"


/**
 * \brief Model Texture Coordinates Set.
 * Defines an additional named set of texture coordinates for all faces
 * in the model.
 */
class DE_DLL_EXPORT deModelTextureCoordinatesSet{
public:
	/** \brief List typedef. */
	using List = decTList<deModelTextureCoordinatesSet>;
	
	/** \brief List of texture coordinates. */
	using TextureCoordinatesList = decTList<decVector2>;
	
	
private:
	TextureCoordinatesList pTextureCoordinates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model texture coordinate set. */
	deModelTextureCoordinatesSet();
	
	/** \brief Copy model texture coordinate set. */
	deModelTextureCoordinatesSet(const deModelTextureCoordinatesSet &set);
	deModelTextureCoordinatesSet(deModelTextureCoordinatesSet &&set) noexcept;
	
	/** \brief Move texture coordinates set. */
	deModelTextureCoordinatesSet &operator=(deModelTextureCoordinatesSet &&set) noexcept;
	deModelTextureCoordinatesSet &operator=(const deModelTextureCoordinatesSet &set);
	
	/** \brief Clean up model texture coordinate set. */
	~deModelTextureCoordinatesSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Texture coordinates list. */
	inline const TextureCoordinatesList &GetTextureCoordinates() const{ return pTextureCoordinates; }
	
	/** \brief Texture coordinates list. */
	inline TextureCoordinatesList &GetTextureCoordinates(){ return pTextureCoordinates; }
	/*@}*/
};

#endif
