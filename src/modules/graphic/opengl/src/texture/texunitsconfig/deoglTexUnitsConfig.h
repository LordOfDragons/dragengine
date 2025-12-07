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

#ifndef _DEOGLTEXUNITSCONFIG_H_
#define _DEOGLTEXUNITSCONFIG_H_

#include "../../deoglBasics.h"

class deoglCubeMap;
class deoglRenderPlan;
class deoglRenderTaskSharedTexture;
class deoglRenderThread;
class deoglShaderParameterBlock;
class deoglTexture;
class deoglTexUnitConfig;


/**
 * Stores the configuration of up to 8 texture units and an optional shader parameter block.
 * This class does not check for errors. The user of the texture units configuration is
 * responsible to ensure a correct setup. The hash code is calculated from the opengl texture
 * names assigned to the units. This works no matter what texture is used since every texture
 * in opengl has a unique name no matter what type of texture it is.
 */
class deoglTexUnitsConfig{
public:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitConfig *pUnits;
	int pUnitCount;
	deoglShaderParameterBlock *pParamBlock;
	
	int pMaterialIndex;
	int pMaterialUsageCount;
	
	int pUsageCount;
	unsigned int pUnitsHashCode;
	unsigned int pUniqueKey;
	
	deoglRenderTaskSharedTexture *pRTSTexture;
	
	deoglTexUnitsConfig *pLLPrev;
	deoglTexUnitsConfig *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture units configuration. */
	deoglTexUnitsConfig(deoglRenderThread &renderThread);
	
	/** Clean up texture units configuration. */
	~deoglTexUnitsConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of units. */
	inline int GetUnitCount() const{ return pUnitCount; }
	
	/** Set count of units. Disabled all units. */
	void SetUnitCount(int count);
	
	/** Texture units pointer for fast access without index checking. */
	inline deoglTexUnitConfig *GetUnits() const{ return pUnits; }
	
	/** Texture unit. */
	deoglTexUnitConfig &GetUnitAt(int index) const;
	
	/** Set units. */
	void SetUnits(const deoglTexUnitConfig *units, int unitCount);
	
	/** Shader parameter block or NULL. */
	inline deoglShaderParameterBlock *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or NULL. */
	void SetParameterBlock(deoglShaderParameterBlock *paramBlock);
	
	
	
	/** Apply texture units configuration. */
	void Apply() const;
	
	
	
	/** Material index or -1. */
	inline int GetMaterialIndex() const{ return pMaterialIndex; }
	
	/** Set material index or -1. */
	void SetMaterialIndex(int index);
	
	/** Material usage count. */
	inline int GetMaterialUsageCount() const{ return pMaterialUsageCount; }
	
	/** Increment material usage count by one. */
	void AddMaterialUsage();
	
	/** Decrease material usage count by one. */
	void RemoveMaterialUsage();
	
	
	
	/** Texture units configuration matches another one. */
	bool Equals(const deoglTexUnitsConfig &tuc) const;
	
	/** Texture units configuration matches another one. */
	bool Equals(const deoglTexUnitConfig *units, int unitCount,
		deoglShaderParameterBlock *paramBlock) const;
	
	/** Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** Increment usage count by one. */
	void AddUsage();
	
	/**
	 * Decrease usage count by one. If the usage count reaches 0 the object is removed
	 * from the texture units configuration manager and deleted.
	 */
	void RemoveUsage();
	
	
	
	/** Hash code. */
	inline unsigned int GetUnitsHashCode() const{ return pUnitsHashCode; }
	
	/** Calculate hash code. */
	void CalcUnitsHashCode();
	
	/** Calculate hash code for set of unit configurations. */
	static unsigned int CalcUnitsHashCodeForUnits(const deoglTexUnitConfig *units, int unitCount);
	
	
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	
	
	
	/** Render task shared texture or NULL. */
	inline deoglRenderTaskSharedTexture *GetRTSTexture() const{ return pRTSTexture; }
	
	/** Ensure render task shared texture is present. */
	void EnsureRTSTexture();
	
	
	
	/** Previous entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLPrev() const{ return pLLPrev; }
	
	/** Set previous entry in the linked list. */
	void SetLLPrev(deoglTexUnitsConfig *entry);
	
	/** Next entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLNext() const{ return pLLNext; }
	
	/** Set next entry in the linked list. */
	void SetLLNext(deoglTexUnitsConfig *entry);
	/*@}*/
};

#endif
