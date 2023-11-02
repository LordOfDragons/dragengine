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
class fbxObjectMap;
class fbxConnectionMap;


/**
 * FBX Scene.
 */
class fbxScene{
public:
	/** Axis. */
	enum eAxis{
		eaXPos,
		eaXNeg,
		eaYPos,
		eaYNeg,
		eaZPos,
		eaZNeg
	};
	
	/** Mapping information type. */
	enum eMappingInformationType{
		emitAllSame,
		emitByPolygon,
		emitByVertex,
		emitByPolygonVertex
	};
	
	/** Reference information type. */
	enum eReferenceInformationType{
		eritDirect,
		eritIndexToDirect
	};
	
	/** Rotation order. */
	enum eRotationOrder{
		eroXYZ,
		eroXZY,
		eroYZX,
		eroYXZ,
		eroZXY,
		eroZYX,
		eroSphericXYZ
	};
	
	/** Weight mode. */
	enum eWeightMode{
		ewmTotal1
	};
	
	
	
private:
	int pVersion;
	fbxNode *pNode;
	
	// global settings
	eAxis pUpAxis;
	eAxis pFrontAxis;
	eAxis pCoordAxis;
	float pUnitScaleFactor;
	
	fbxNode *pNodeObjects;
	fbxNode *pNodeConnections;
	fbxObjectMap *pObjectMap;
	decObjectOrderedSet pConnections;
	fbxConnectionMap *pConnectionMap;
	
	decMatrix pTransformation;
	decMatrix pAxisTransformation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create scene. */
	fbxScene();
	
	/** Load scene. */
	fbxScene( decBaseFileReader &reader );
	
	/** Clean up scene. */
	~fbxScene();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Version in the form 7300 = 7.3 . */
	inline int GetVersion() const{ return pVersion; }
	
	/** Set version in the form 7300 = 7.3 . */
	void SetVersion( int version );
	
	
	
	/** Root node. */
	inline fbxNode *GetNode() const{ return pNode; }
	
	/** Root node. */
	inline fbxNode *GetNodeObjects() const{ return pNodeObjects; }
	
	/** Scene transformation. */
	inline const decMatrix &GetTransformation() const{ return pTransformation; }
	
	/** Scene axis transformation. */
	inline const decMatrix &GetAxisTransformation() const{ return pAxisTransformation; }
	
	/** Transform matrix. */
	decMatrix TransformMatrix( const decMatrix &matrix ) const;
	
	
	
	/** First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamed( const char *name ) const;
	
	/** First node matching name or NULL if absent. */
	fbxNode *FirstNodeNamedOrNull( const char *name ) const;
	
	/** Find all nodes matching name. */
	void FindNodesNamed( decPointerList &list, const char *name ) const;
	
	
	
	/** All connections containing ID either as source or target. */
	void FindConnections( int64_t id, decPointerList &list ) const;
	
	/** Object with ID. */
	fbxNode *NodeWithID( int64_t id ) const;
	
	/** Object with ID or NULL if absent. */
	fbxNode *NodeWithIDOrNull( int64_t id ) const;
	
	
	
	/** Convert UV. */
	static decVector2 ConvUVFbxToDe( const decVector2 &uv );
	
	/** Convert mapping information type. */
	static eMappingInformationType ConvMappingInformationType( const fbxNode &node );
	
	/** Convert refernece information type. */
	static eReferenceInformationType ConvReferenceInformationType( const fbxNode &node );
	
	/** Convert rotation order. */
	static eRotationOrder ConvRotationOrder( int value );
	
	/** Convert weight mode. */
	static eWeightMode ConvWeightMode( const fbxNode &node );
	
	/** Create rotation matrix. */
	static decMatrix CreateRotationMatrix( const decVector &rotation, eRotationOrder rotationOrder );
	
	/** Convert rotation. */
	static decVector ConvRotation( const decVector &rotation );
	
	
	
	/** Prepare after reading. */
	void Prepare( deBaseModule &module );
	
	/** Save to file. */
	void Save( decBaseFileWriter &writer );
	
	/** Debug print scene structure. */
	void DebugPrintStructure( deBaseModule &logger, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	eAxis pGetAxis( int axisType, int axisSign ) const;
};

#endif
