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

// include only once
#ifndef _MEHTVEVALUATIONENVIRONMENT_H_
#define _MEHTVEVALUATIONENVIRONMENT_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class meHeightTerrainSector;
class meHeightTerrainPropField;
class meHeightTerrainTexture;
class meHTVegetationLayer;
class meBitArray;
class meObject;
class meWorld;



/**
 * @brief Height Terrain Vegetation Evaluation Environment.
 *
 * Provides an environment for evaluating height terrain vegetation
 * rules. Such an environment composes of the height terrain sector
 * in which the rules are evaluated as well as information about
 * the instance to be evaluated. The position and normal are the
 * position and normal of the instance to be evaluated. The HT
 * coordinates are the coordinates of the instance on the height
 * terrain measured in pixels of the height image. The int and frac
 * variants contain the same coordinates but split up into integer
 * and fractional part.
 */
class meHTVEvaluationEnvironment{
private:
	meWorld *pWorld;
	
	decDVector pPosition;
	decVector pNormal;
	
	meHeightTerrainSector *pHTSector;
	meHeightTerrainTexture *pHTDominantTexture;
	decVector2 pHTCoordinates;
	decPoint pHTCoordInt;
	decVector2 pHTCoordFract;
	
	meHTVegetationLayer *pVLayer;
	meHeightTerrainPropField *pPropField;
	meBitArray *pOccupation;
	
	meObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
	float pProbability;
	int pVariation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHTVEvaluationEnvironment();
	/** Cleans up the object. */
	~meHTVEvaluationEnvironment();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** Sets the world. */
	void SetWorld( meWorld *world );
	
	/** Retrieves the instance position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the instance position. */
	void SetPosition( const decDVector &position );
	/** Retrieves the instance normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	/** Sets the instance normal. */
	void SetNormal( const decVector &normal );
	/** Sets the instance parameters. */
	void SetParameters( const decDVector &position, const decVector &normal );
	
	/** Retrieves the height terrain sector. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	/** Sets the height terrain sector. */
	void SetHTSector( meHeightTerrainSector *htsector );
	/** Retrieves the dominant texture or NULL if none exists. */
	inline meHeightTerrainTexture *GetHTDominantTexture() const{ return pHTDominantTexture; }
	/** Sets the dominant texture or NULL if none exists. */
	void SetHTDominantTexture( meHeightTerrainTexture *texture );
	/** Retrieves the height terrain coordinates. */
	inline const decVector2 &GetHTCoordinates() const{ return pHTCoordinates; }
	/** Sets the height terrain coordinates. */
	void SetHTCoordinates( const decVector2 &coordinates );
	/** Sets the height terrain coordinates. */
	void SetHTCoordinates( float x, float y );
	/** Retrieves the integer part of the height terrain coordinates. */
	inline const decPoint &GetHTCoordInt() const{ return pHTCoordInt; }
	/** Retrieves the fractional part of the height terrain coordinates. */
	inline const decVector2 &GetHTCoordFract() const{ return pHTCoordFract; }
	
	/** Retrieves the vegetation layer. */
	inline meHTVegetationLayer *GetVLayer() const{ return pVLayer; }
	/** Sets the vegetation layer. */
	void SetVLayer( meHTVegetationLayer *vlayer );
	/** Retrieves the prop field. */
	inline meHeightTerrainPropField *GetPropField() const{ return pPropField; }
	/** Sets the prop field. */
	void SetPropField( meHeightTerrainPropField *propField );
	/** Retrieves the occupation bit array. */
	inline meBitArray *GetOccupation() const{ return pOccupation; }
	/** Sets the occupation bit array. */
	void SetOccupation( meBitArray *occupation );
	
	/** Retrieves the probability of the evaluation. */
	inline float GetProbability() const{ return pProbability; }
	/** Sets the probability of the evaluation. */
	void SetProbability( float probability );
	/** Retrieves the variation of the evaluation. */
	inline int GetVariation() const{ return pVariation; }
	/** Sets the variation of the evaluation. */
	void SetVariation( int variation );
	
	/**
	 * Prepares for evaluation. This populates the environment with objects
	 * required for evaluating rules on the given configuration.
	 */
	void Prepare();
	/** Evaluates the rules once. */
	void EvaluateRules();
	/*@}*/
	
	/** \name Objects */
	/*@{*/
	/** Retrieves the number of objects. */
	inline int GetObjectCount() const{ return pObjectCount; }
	/** Retrieves the object at the given position. */
	meObject *GetObjectAt( int index ) const;
	/** Adds an object. */
	void AddObject( meObject *object );
	/** Removes all objects. */
	void RemoveAllObjects();
	
	/**
	 * Populates environment with objects inside the boundaries of the prop
	 * field and the rules requiring props. For this to work prop field and
	 * world has to be not null. Beforehand all objects are removed no matter
	 * if thr world and prop field are null or not.
	 */
	void PopulateWithObjects();
	/*@}*/
};

// end of include only once
#endif
