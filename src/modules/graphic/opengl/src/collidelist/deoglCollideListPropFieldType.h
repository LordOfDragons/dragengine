/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deoglCollideListPropFieldType( deoglCollideListPropField &propField );
	
	/** Clean up prop field type. */
	~deoglCollideListPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear sector. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decVector &offset );
	
	
	
	/** Type. */
	inline deoglRPropFieldType *GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deoglRPropFieldType *type );
	
	/** Count of clusters. */
	inline int GetClusterCount() const{ return pClusterCount; }
	
	/** Cluster at index. */
	deoglCollideListPropFieldCluster &GetClusterAt( int index ) const;
	
	/** Add cluster. */
	deoglCollideListPropFieldCluster *AddCluster( deoglPropFieldCluster *cluster );
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Remove culled clusters. */
	void RemoveCulledClusters();
	
	
	
	/** Prop field type is empty. */
	inline bool GetIsEmpty() const{ return pClusterCount == 0; }
	
	/** Prop field type is not empty. */
	inline bool GetIsNotEmpty() const{ return pClusterCount > 0; }
	/*@}*/
};

#endif
