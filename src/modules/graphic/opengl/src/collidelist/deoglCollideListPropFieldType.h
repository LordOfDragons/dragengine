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

class deoglPropFieldCluster;



/**
 * Collide List Prop Field Type.
 */
class deoglCollideListPropFieldType{
private:
	deoglPropFieldCluster **pClusters;
	int pClusterCount;
	int pClusterSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list prop field type. */
	deoglCollideListPropFieldType();
	
	/** Clean up collide list prop field type. */
	~deoglCollideListPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field type is empty. */
	inline bool GetIsEmpty() const{ return pClusterCount == 0; }
	
	/** Prop field type is not empty. */
	inline bool GetIsNotEmpty() const{ return pClusterCount > 0; }
	
	/** Count of clusters. */
	inline int GetClusterCount() const{ return pClusterCount; }
	
	/** Clusters direct access. */
	inline deoglPropFieldCluster **GetClusters(){ return pClusters; }
	inline const deoglPropFieldCluster **GetClusters() const{ return ( const deoglPropFieldCluster ** )pClusters; }
	
	/** Add cluster. */
	void AddCluster( deoglPropFieldCluster *cluster );
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	/*@}*/
};

#endif
