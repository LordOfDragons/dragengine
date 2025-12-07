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

#ifndef _DEOGLTEXUNITSCONFIGLIST_H_
#define _DEOGLTEXUNITSCONFIGLIST_H_

class deoglRenderThread;
class deoglTexUnitConfig;
class deoglTexUnitsConfig;
class deoglShaderParameterBlock;


/**
 * Texture Units Configuration List.
 */
class deoglTexUnitsConfigList{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitsConfig *pRootTUC;
	deoglTexUnitsConfig *pTailTUC;
	int pTUCCount;
	
	deoglTexUnitsConfig *pTUCEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglTexUnitsConfigList(deoglRenderThread &renderThread);
	
	/** Clean up list. */
	~deoglTexUnitsConfigList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Count of texture units configurations. */
	inline int GetCount() const{return pTUCCount;}
	
	/** Root texture units configuration. */
	inline deoglTexUnitsConfig *GetRoot() const{return pRootTUC;}
	
	/**
	 * Texture units configuration matching parameters. Create texture units configuration
	 * if absent. Adds usage to texture units configuration. To remove a usage use RemoveUsage.
	 */
	deoglTexUnitsConfig *GetWith(const deoglTexUnitConfig *units,
		int unitCount, deoglShaderParameterBlock *paramBlock);
	
	/** Empty texture units configuration. */
	deoglTexUnitsConfig *GetEmpty();
	
	/** Empty texture units configuration without adding usage. */
	deoglTexUnitsConfig *GetEmptyNoUsage() const;
	
	/** Remove texture units configuration. For use by deoglTexUnitsConfig only. */
	void Remove(deoglTexUnitsConfig *config);
	/*@}*/
};

#endif
