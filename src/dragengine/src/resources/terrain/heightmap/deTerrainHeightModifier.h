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

#ifndef _DETERRAINHEIGHTMODIFIER_H_
#define _DETERRAINHEIGHTMODIFIER_H_

#include "../../../common/math/decMath.h"
#include <dragengine/resources/image/deImage.h>



/**
 * \brief Terrain Height Modifier.
 *
 * Defines a small grid of height modifiers. The grid is overlayed on an
 * existing height map terrain mesh and alters the heights by adding a
 * height difference from an image. Height modifiers can protrude outside
 * the the original height map. All points outside the valid range are
 * ignored. The height modifier works similar to the height map itself.
 * The position is the upper left corner of the height modifier which is
 * on the negative X and positive Z side if looked from the terrain.
 */
class DE_DLL_EXPORT deTerrainHeightModifier{
private:
	deImage::Ref pHeightImage;
	decPoint pPosition;
	float pScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new height modifier. */
	deTerrainHeightModifier(deImage *heightImage);
	
	/** \brief Clean up height modifier. */
	~deTerrainHeightModifier();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position of the upper left corner. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position of the upper left corner. */
	void SetPosition(const decPoint &position);
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling(float scaling);
	
	/** \brief Height image. */
	inline const deImage::Ref &GetHeightImage() const{ return pHeightImage; }
	/*@}*/
};

#endif
