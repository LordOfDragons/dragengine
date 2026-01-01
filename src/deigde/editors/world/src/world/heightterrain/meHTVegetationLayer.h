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

#ifndef _MEHEIGHTTERRAINVEGETATION_H_
#define _MEHEIGHTTERRAINVEGETATION_H_

#include "meHTVVariation.h"
#include "rules/meHTVRule.h"
#include "rules/meHTVRLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class meHeightTerrain;
class meHTVEvaluationEnvironment;

class deEngine;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * @brief Height Terrain Vegetation.
 *
 * Defines a layer of vegetation on a height terrain. A vegetation layer
 * composes of a model, skin and optional a rig defining the plant to
 * render and a population rule. To populate the height terrain with plants
 * from this vegetation layer the population rule is evaluated for various
 * points scattered across the height terrain. The population rule returns
 * true if a plant has to be set or false otherwise.
 */
class meHTVegetationLayer : public deObject{
private:
	meHeightTerrain *pHeightTerrain;
	
	deEngine *pEngine;
	
	decString pName;
	
	meHTVVariation::List pVariations;
	meHTVVariation::Ref pActiveVariation;
	
	meHTVRule::List pRules;
	meHTVRule::Ref pActiveRule;
	
	meHTVRLink::List pLinks;
	
	decVector2 pViewCenter;
	
public:
	typedef deTObjectReference<meHTVegetationLayer> Ref;
	typedef decTObjectOrderedSet<meHTVegetationLayer> List;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHTVegetationLayer(deEngine *engine, const char *name = "");
	
protected:
	/** Cleans up the object. */
	virtual ~meHTVegetationLayer();
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain or nullptr. */
	inline meHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set parent height terrain or nullptr. */
	void SetHeightTerrain(meHeightTerrain *heightTerrain);
	
	/** Retrieves the center of the nodes in the nodes view. */
	inline const decVector2 &GetViewCenter() const{ return pViewCenter; }
	/** Sets the center of the nodes in the nodes view. */
	void SetViewCenter(const decVector2 &center);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName(const char *name);
	/*@}*/
	
	/** \name Variation */
	/*@{*/
	/** Variations. */
	inline const meHTVVariation::List &GetVariations() const{ return pVariations; }
	
	/** Adds a variation. */
	void AddVariation(meHTVVariation *variation);
	
	/** Inserts a variation. */
	void InsertVariation(int before, meHTVVariation *variation);
	
	/** Moves a variation to a new position. */
	void MoveVariation(meHTVVariation *variation, int moveTo);
	
	/** Removes a variation. */
	void RemoveVariation(meHTVVariation *variation);
	
	/** Removes all variations. */
	void RemoveAllVariations();
	
	/** Retrieves the active variation or nullptr. */
	inline const meHTVVariation::Ref &GetActiveVariation() const{ return pActiveVariation; }
	
	/** Sets the active variation or nullptr. */
	void SetActiveVariation(meHTVVariation *variation);
	/*@}*/
	
	/** \name Rule */
	/*@{*/
	/** Rules. */
	inline const meHTVRule::List &GetRules() const{ return pRules; }
	
	/** Adds a rule. */
	void AddRule(meHTVRule *rule);
	
	/** Removes a rule. */
	void RemoveRule(meHTVRule *rule);
	
	/** Removes all rules. */
	void RemoveAllRules();
	
	/** Retrieves the active rule or nullptr. */
	inline const meHTVRule::Ref &GetActiveRule() const{ return pActiveRule; }
	
	/** Sets the active rule or nullptr. */
	void SetActiveRule(meHTVRule *rule);
	
	/** Notifies the world that a rule changed. */
	void NotifyRuleChanged(meHTVRule *rule);
	
	/** Notifies the world that a rule moved. */
	void NotifyRuleMoved(meHTVRule *rule);
	
	/** Evaluates the rules. */
	void EvaluateRules(meHTVEvaluationEnvironment &evalEnv);
	/*@}*/
	
	/** \name Link */
	/*@{*/
	/** Links. */
	inline const meHTVRLink::List &GetLinks() const{ return pLinks; }
	
	/** Adds a link. */
	void AddLink(meHTVRLink *link);
	
	/** Removes a link. */
	void RemoveLink(meHTVRLink *link);
	
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Determines if adding a given link would produce a loop. */
	bool LinkProducesLoop(meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
