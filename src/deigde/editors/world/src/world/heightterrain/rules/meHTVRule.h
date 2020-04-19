/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEHTVRULE_H_
#define _MEHTVRULE_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVRSlot;
class meHTVRLink;
class meHTVEvaluationEnvironment;



/**
 * @brief Height Terrain Vegetation Rule.
 *
 * Rule to define the population of a height terrain vegetation layer.
 * Each rule has a set of input and output slots. Input slots can be
 * of single or multiple type. Single type slots can have only one
 * other rule attached while multiple type slots can have zero, one
 * or more rules attached. Output slots can attach to exactly one
 * other rule.
 */
class meHTVRule : public deObject{
public:
	/** Rule types. */
	enum eRuleTypes{
		/** Result Rule. */
		ertResult,
		/** Geometry Rule. */
		ertGeometry,
		/** Components Rule. */
		ertComponents,
		/** Combine Rule. */
		ertCombine,
		/** Mapping Rule. */
		ertMapping,
		/** Curve Rule. */
		ertCurve,
		/** Math Rule. */
		ertMath,
		/** Multi Math Rule. */
		ertMultiMath,
		/** Vector Math Rule. */
		ertVectorMath,
		/** Closest Vegetation Rule. */
		ertClosestVegetation,
		/** Closest Prop Rule. */
		ertClosestProp,
		/** Random Rule. */
		ertRandom,
		/** Constant Rule. */
		ertConstant,
		/** Prop Count Rule. */
		ertPropCount,
		/** Dummy: Number of rule types. */
		ERT_COUNT
	};
	
private:
	decString pName;
	int pType;
	decVector2 pPosition;
	bool pShowParameters;
	
	int pSlotCount;
	meHTVRSlot *pSlots;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRule( int type, int slotCount );
	
protected:
	/** \brief Create copy of rule. */
	meHTVRule( const meHTVRule &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRule();
	/*@}*/
	
	
	
public:
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the type. */
	inline int GetType() const{ return pType; }
	/** Retrieves the position. */
	inline const decVector2 &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector2 &position );
	/** Determines if parameters are shown. */
	inline bool GetShowParameters() const{ return pShowParameters; }
	/** Sets if parameters are shown. */
	void SetShowParameters( bool showParameters );
	
	/** \brief Count of slots. */
	inline int GetSlotCount() const{ return pSlotCount; }
	
	/** \brief Slot at index. */
	meHTVRSlot &GetSlotAt( int slot ) const;
	
	/**
	 * Determines if this rule depends directly or indirectly on another rule.
	 * A rule depends on another rule if the given node can be reached while
	 * moving along the input slot links.
	 */
	bool DependsOn( meHTVRule *rule ) const;
	
	/** Resets the rule state. */
	virtual void Reset();
	/** Evaluate rule. */
	virtual void Evaluate( meHTVEvaluationEnvironment &evalEnv );
	/** Retrieves the value of a given output slot. */
	virtual float GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const = 0;
	/*@}*/
};

// end of include only once
#endif
