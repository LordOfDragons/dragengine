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

#ifndef _DEOGLCUBEHELPER_H_
#define _DEOGLCUBEHELPER_H_

#include <dragengine/common/math/decMath.h>



/**
 * Cube calculation helpers.
 */
class deoglCubeHelper{
public:
	/** \name Helpers */
	/*@{*/
	/**
	 * Calculate cube face visibility.
	 * 
	 * Calculates for each face of a cube map if the object is potentially visible.
	 * Used to optimize rendering cube maps with geometry shaders by not rendering
	 * object faces on cube map faces where the object is not visible.
	 * 
	 * \param[in] boxMin Minimum extend of box to test relative to cube origin.
	 * \param[in] boxMax Maximum extend of box to test relative to cube origin.
	 * \param[out] result Set to visibility for each face. Has to be array of 6 size.
	 *                    Faces are in the order X+, X-, Y-, Y+, Z+, Z- as suitable
	 *                    for special render parameter blocks.
	 */
	static void CalcFaceVisibility(const decDVector &boxMin,
		const decDVector &boxMax, bool *result);
	
	static int CalcFaceVisibility(const decDVector &boxMin, const decDVector &boxMax);
	/*@}*/
};

#endif
