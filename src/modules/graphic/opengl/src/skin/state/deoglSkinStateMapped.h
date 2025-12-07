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

#ifndef _DEOGLSKINSTATEMAPPED_H_
#define _DEOGLSKINSTATEMAPPED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSkinState;
class deoglRComponent;
class deoglSkinMapped;

class deComponent;


/**
 * Skin mapped state.
 */
class deoglSkinStateMapped{
private:
	float pValue;
	const deoglSkinMapped *pMapped;
	const deoglRComponent *pComponent;
	int pBone;
	float pBoneInputValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state mapped. */
	deoglSkinStateMapped();
	
	/** Clean up skin state mapped. */
	~deoglSkinStateMapped();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Mapped value. */
	inline float GetValue() const{return pValue;}
	
	/** Set mapped value. */
	void SetValue(float value);
	
	/** Skin mapped. */
	const deoglSkinMapped *GetMapped() const{return pMapped;}
	
	/** Set skin mapped. */
	void SetMapped(const deoglSkinMapped *mapped);
	
	/** Component for bone calculate or nullptr. */
	inline const deoglRComponent *GetComponent() const{return pComponent;}
	
	/** Set component for bone calculate or nullptr. */
	void SetComponent(const deoglRComponent *component);
	
	/** Bone index or -1 if not found. */
	inline int GetBone() const{return pBone;}
	
	/** Bone input value. */
	inline float GetBoneInputValue() const{return pBoneInputValue;}
	
	/** Map bone. */
	void MapBone(const deComponent &component);
	
	/** Update bone. */
	void UpdateBone(const deComponent &component);
	
	/** Update. */
	void Update(deoglSkinState &skinState);
	/*@}*/
};

#endif
