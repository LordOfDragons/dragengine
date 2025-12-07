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

#ifndef _DEOGLMODELTEXTURE_H_
#define _DEOGLMODELTEXTURE_H_

#include <dragengine/common/string/decString.h>

#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"

class deoglRenderThread;


/**
 * Model texture.
 */
class deoglModelTexture{
private:
	int pFirstFace;
	int pFaceCount;
	bool pDoubleSided;
	bool pDecal;
	int pDecalOffset;
	deoglSharedSPBRTIGroupList::Ref pRTIGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture. */
	deoglModelTexture(deoglRenderThread &renderThread);
	
	/** Clean up texture. */
	~deoglModelTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** First face index. */
	inline int GetFirstFace() const{ return pFirstFace; }
	
	/** Set first face index. */
	void SetFirstFace(int faceIndex);
	
	/** Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** Set face count. */
	void SetFaceCount(int faceCount);
	
	/** Texture is double sided. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Set texture is double sided. */
	void SetDoubleSided(bool doubleSided);
	
	/** Texture is rendered as decal. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Set if texture is rendered as decal. */
	void SetDecal(bool decal);
	
	/** Decal offset. */
	inline int GetDecalOffset() const{ return pDecalOffset; }
	
	/** Set decal offset. */
	void SetDecalOffset(int offset);
	
	/** Render task instance groups. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroups(){ return pRTIGroups; }
	/*@}*/
};

#endif
