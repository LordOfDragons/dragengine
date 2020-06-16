/* 
 * FBX Modules
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

#ifndef _FBXSCENE_H_
#define _FBXSCENE_H_


#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>


class decBaseFileReader;
class decBaseFileWriter;
class deBaseModule;
class fbxNode;


/**
 * \brief FBX Scene.
 */
class fbxScene{
public:
	/** \brief Axis. */
	enum eAxis{
		eaXPos,
		eaXNeg,
		eaYPos,
		eaYNeg,
		eaZPos,
		eaZNeg
	};
	
	/** \brief Mapping information type. */
	enum eMappingInformationType{
		emitAllSame,
		emitByPolygon,
		emitByVertex,
		emitByPolygonVertex
	};
	
	/** \brief Reference information type. */
	enum eReferenceInformationType{
		eritDirect,
		eritIndexToDirect
	};
	
	/** \brief Rotation order. */
	enum eRotationOrder{
		eroXYZ,
		eroXZY,
		eroYZX,
		eroYXZ,
		eroZXY,
		eroZYX,
		eroSphericXYZ
	};
	
	
	
private:
	int pVersion;
	fbxNode *pNode;
	
	// global settings
	eAxis pUpAxis;
	eAxis pFrontAxis;
	eAxis pCoordAxis;
	float pUnitScaleFactor;
	float pScaleFactor;
	
	fbxNode *pNodeObjects;
	fbxNode *pNodeConnections;
	decObjectOrderedSet pConnections;
	
	decMatrix pTransformation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scene. */
	fbxScene();
	
	/** \brief Load scene. */
	fbxScene( decBaseFileReader &reader );
	
	/** \brief Clean up scene. */
	~fbxScene();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Version in the form 7300 = 7.3 . */
	inline int GetVersion() const{ return pVersion; }
	
	/** \brief Set version in the form 7300 = 7.3 . */
	void SetVersion( int version );
	
	
	
	/** \brief Root node. */
	inline fbxNode *GetNode() const{ return pNode; }
	
	/** \brief Scale factor. */
	inline float GetScaleFactor() const{ return pScaleFactor; }
	
	/** \brief Unit scale factor. */
	inline float GetUnitScaleFactor() const{ return pUnitScaleFactor; }
	
	/** \brief Scene transformation. */
	inline const decMatrix &GetTransformation() const{ return pTransformation; }
	
	
	
	/** \brief First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamed( const char *name ) const;
	
	/** \brief First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamedOrNull( const char *name ) const;
	
	
	
	/** \brief All connections containing ID either as source or target. */
	void FindConnections( int64_t id, decPointerList &list ) const;
	
	/** \brief Object with ID. */
	fbxNode *NodeWithID( int64_t id ) const;
	
	/** \brief Object with ID or NULL if absent. */
	fbxNode *NodeWithIDOrNull( int64_t id ) const;
	
	
	
	/** \brief Convert UV. */
	static decVector2 ConvUVFbxToDe( const decVector2 &uv );
	
	/** \brief Convert mapping information type. */
	static eMappingInformationType ConvMappingInformationType( const fbxNode &node );
	
	/** \brief Convert refernece information type. */
	static eReferenceInformationType ConvReferenceInformationType( const fbxNode &node );
	
	/** \brief Convert rotation order. */
	static eRotationOrder ConvRotationOrder( int value );
	
	/** \brief Create rotation matrix. */
	static decMatrix CreateRotationMatrix( const decVector &rotation, eRotationOrder rotationOrder );
	
	
	
	/** \brief Prepare after reading. */
	void Prepare( deBaseModule &module );
	
	/** \brief Save to file. */
	void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print scene structure. */
	void DebugPrintStructure( deBaseModule &logger, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	eAxis pGetAxis( int axisType, int axisSign ) const;
};

#endif
