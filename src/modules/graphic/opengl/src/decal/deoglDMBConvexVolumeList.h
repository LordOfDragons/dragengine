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

#ifndef _DEOGLDMBCONVEXVOLUMELIST_H_
#define _DEOGLDMBCONVEXVOLUMELIST_H_

#include <dragengine/common/math/decConvexVolumeList.h>



/**
 * OpenGL decal mesh builder convex volume list.
 * 
 * Specialized version of the convex volume list used by the decal mesh builder.
 */
class deoglDMBConvexVolumeList : public decConvexVolumeList{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decal mesh builder convex volume list. */
	deoglDMBConvexVolumeList();
	
	/** Clean up decal mesh builder convex volume list. */
	virtual ~deoglDMBConvexVolumeList();
	/*@}*/
	
	
	
	/** \name Subclassing */
	/*@{*/
	/**
	 * Creates a new convex volume face that originates from the given face.
	 * It is possible that the face is NULL in which case a default face
	 * has to be created. NULL faces typically occure if a split is done using
	 * a split plane instead of a split volume. Overwrite this function to
	 * create convex volume face of specialized subclasses. The default
	 * implementation creates a convex volume face of type decConvexVolumeFace.
	 */
	virtual decConvexVolumeFace *CreateVolumeFace(decConvexVolumeFace *face);
	/*@}*/
};

#endif
