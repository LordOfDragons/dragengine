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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRule> Ref;
	
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRule(int type, int slotCount);
	
protected:
	/** \brief Create copy of rule. */
	meHTVRule(const meHTVRule &rule);
	
	/** Cleans up the rule. */
	virtual ~meHTVRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the type. */
	inline int GetType() const{return pType;}
	/** Retrieves the position. */
	inline const decVector2 &GetPosition() const{return pPosition;}
	/** Sets the position. */
	void SetPosition(const decVector2 &position);
	/** Determines if parameters are shown. */
	inline bool GetShowParameters() const{return pShowParameters;}
	/** Sets if parameters are shown. */
	void SetShowParameters(bool showParameters);
	
	/** \brief Count of slots. */
	inline int GetSlotCount() const{return pSlotCount;}
	
	/** \brief Slot at index. */
	meHTVRSlot &GetSlotAt(int slot) const;
	
	/**
	 * Determines if this rule depends directly or indirectly on another rule.
	 * A rule depends on another rule if the given node can be reached while
	 * moving along the input slot links.
	 */
	bool DependsOn(meHTVRule *rule) const;
	
	/** Resets the rule state. */
	virtual void Reset();
	/** Evaluate rule. */
	virtual void Evaluate(meHTVEvaluationEnvironment &evalEnv);
	/** Retrieves the value of a given output slot. */
	virtual float GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const = 0;
	/*@}*/
};

// end of include only once
#endif
