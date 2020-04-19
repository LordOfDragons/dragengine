/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAISPACEMESHLINK_H_
#define _DEDAISPACEMESHLINK_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceMesh;



/**
 * \brief Space mesh link.
 */
class dedaiSpaceMeshLink{
private:
	dedaiSpaceMesh *pMesh;
	unsigned short pFace;
	unsigned short pCorner;
	decMatrix pTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh link. */
	dedaiSpaceMeshLink();
	
	/** \brief Clean up space mesh link. */
	~dedaiSpaceMeshLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Space mesh. */
	inline dedaiSpaceMesh *GetMesh() const{ return pMesh; }
	
	/** \brief Set space mesh. */
	void SetMesh( dedaiSpaceMesh *mesh );
	
	/** \brief Face index. */
	inline unsigned short GetFace() const{ return pFace; }
	
	/** \brief Set face index. */
	void SetFace( unsigned short face );
	
	/** \brief Face corner index. */
	inline unsigned short GetCorner() const{ return pCorner; }
	
	/** \brief Set face corner index. */
	void SetCorner( unsigned short corner );
	
	/** \brief Transformation matrix from this navigation mesh to the linked one. */
	inline const decMatrix &GetTransform() const{ return pTransform; }
	
	/** \brief Set transformation matrix from this navigation mesh to the linked one. */
	void SetTransform( const decMatrix &transform );
	
	
	
	/** \brief Link matches. */
	bool Matches( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner ) const;
	
	/** \brief Foreign link from parameters. */
	dedaiSpaceMeshLink &GetForeignLink() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brier Copy corner. */
	dedaiSpaceMeshLink &operator=( const dedaiSpaceMeshLink &link );
	/*@}*/
};

#endif
