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
#ifndef _MEHEIGHTTERRAINVEGETATION_H_
#define _MEHEIGHTTERRAINVEGETATION_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

// predefinitions
class meHeightTerrain;
class meHTVRLink;
class meHTVRule;
class meHTVVariation;
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
	
	meHTVVariation **pVariations;
	int pVariationCount;
	int pVariationSize;
	meHTVVariation *pActiveVariation;
	
	meHTVRule **pRules;
	int pRuleCount;
	int pRuleSize;
	meHTVRule *pActiveRule;
	
	meHTVRLink **pLinks;
	int pLinkCount;
	int pLinkSize;
	
	decVector2 pViewCenter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHTVegetationLayer( deEngine *engine, const char *name = "" );
	/** Cleans up the object. */
	virtual ~meHTVegetationLayer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain or NULL. */
	inline meHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set parent height terrain or NULL. */
	void SetHeightTerrain( meHeightTerrain *heightTerrain );
	
	/** Retrieves the center of the nodes in the nodes view. */
	inline const decVector2 &GetViewCenter() const{ return pViewCenter; }
	/** Sets the center of the nodes in the nodes view. */
	void SetViewCenter( const decVector2 &center );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/*@}*/
	
	/** \name Variation */
	/*@{*/
	/** Retrieves the number of variations. */
	inline int GetVariationCount() const{ return pVariationCount; }
	/** Retrieves the variation at the given index. */
	meHTVVariation *GetVariationAt( int index ) const;
	/** Retrieves the index of the variation or -1 if not found. */
	int IndexOfVariation( meHTVVariation *variation ) const;
	/** Determines if the variation exists. */
	bool HasVariation( meHTVVariation *variation ) const;
	/** Adds a variation. */
	void AddVariation( meHTVVariation *variation );
	/** Inserts a variation. */
	void InsertVariation( int before, meHTVVariation *variation );
	/** Moves a variation to a new position. */
	void MoveVariation( meHTVVariation *variation, int moveTo );
	/** Removes a variation. */
	void RemoveVariation( meHTVVariation *variation );
	/** Removes all variations. */
	void RemoveAllVariations();
	/** Retrieves the active variation or NULL. */
	inline meHTVVariation *GetActiveVariation() const{ return pActiveVariation; }
	/** Sets the active variation or NULL. */
	void SetActiveVariation( meHTVVariation *variation );
	/*@}*/
	
	/** \name Rule */
	/*@{*/
	/** Retrieves the number of rules. */
	inline int GetRuleCount() const{ return pRuleCount; }
	/** Retrieves the rule at the given index. */
	meHTVRule *GetRuleAt( int index ) const;
	/** Retrieves the index of the rule or -1 if not found. */
	int IndexOfRule( meHTVRule *rule ) const;
	/** Determines if the rule exists. */
	bool HasRule( meHTVRule *rule ) const;
	/** Adds a rule. */
	void AddRule( meHTVRule *rule );
	/** Removes a rule. */
	void RemoveRule( meHTVRule *rule );
	/** Removes all rules. */
	void RemoveAllRules();
	/** Retrieves the active rule or NULL. */
	inline meHTVRule *GetActiveRule() const{ return pActiveRule; }
	/** Sets the active rule or NULL. */
	void SetActiveRule( meHTVRule *rule );
	/** Notifies the world that a rule changed. */
	void NotifyRuleChanged( meHTVRule *rule );
	/** Notifies the world that a rule moved. */
	void NotifyRuleMoved( meHTVRule *rule );
	
	/** Evaluates the rules. */
	void EvaluateRules( meHTVEvaluationEnvironment &evalEnv );
	/*@}*/
	
	/** \name Link */
	/*@{*/
	/** Retrieves the number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	/** Retrieves the link at the given index. */
	meHTVRLink *GetLinkAt( int index ) const;
	/** Retrieves the index of the link or -1 if not found. */
	int IndexOfLink( meHTVRLink *link ) const;
	/** Determines if the link exists. */
	bool HasLink( meHTVRLink *link ) const;
	/** Adds a link. */
	void AddLink( meHTVRLink *link );
	/** Removes a link. */
	void RemoveLink( meHTVRLink *link );
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Determines if adding a given link would produce a loop. */
	bool LinkProducesLoop( meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
