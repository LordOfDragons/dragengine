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

#ifndef _DEOGLCOLLIDELISTPROPFIELDTYPE_H_
#define _DEOGLCOLLIDELISTPROPFIELDTYPE_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglCollideListPropField;
class deoglCollideListPropFieldCluster;
class deoglOcclusionTest;
class deoglRPropFieldType;
class deoglPropFieldCluster;


/**
 * Collide List Prop Field Type.
 */
class deoglCollideListPropFieldType{
private:
	deoglCollideListPropField &pPropField;
	
	deoglRPropFieldType *pType;
	
	decPointerList pClusters;
	int pClusterCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create prop field type. */
	deoglCollideListPropFieldType(deoglCollideListPropField &propField);
	
	/** Clean up prop field type. */
	~deoglCollideListPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear sector. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decVector &offset);
	
	
	
	/** Type. */
	inline deoglRPropFieldType *GetType() const{return pType;}
	
	/** Set type. */
	void SetType(deoglRPropFieldType *type);
	
	/** Count of clusters. */
	inline int GetClusterCount() const{return pClusterCount;}
	
	/** Cluster at index. */
	deoglCollideListPropFieldCluster &GetClusterAt(int index) const;
	
	/** Add cluster. */
	deoglCollideListPropFieldCluster *AddCluster(deoglPropFieldCluster *cluster);
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Remove culled clusters. */
	void RemoveCulledClusters();
	
	
	
	/** Prop field type is empty. */
	inline bool GetIsEmpty() const{return pClusterCount == 0;}
	
	/** Prop field type is not empty. */
	inline bool GetIsNotEmpty() const{return pClusterCount > 0;}
	/*@}*/
};

#endif
