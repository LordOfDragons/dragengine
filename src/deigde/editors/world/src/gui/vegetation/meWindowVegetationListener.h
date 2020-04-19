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

#ifndef _MEWINDOWVEGETATIONLISTENER_H_
#define _MEWINDOWVEGETATIONLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWindowVegetation;



/**
 * \brief Vegetation Editing Window Listener.
 */
class meWindowVegetationListener : public meWorldNotifier{
private:
	meWindowVegetation &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWindowVegetationListener( meWindowVegetation &window );
	
	/** \brief Clean up object. */
	virtual ~meWindowVegetationListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active sector changed. */
	virtual void ActiveSectorChanged( meWorld *world );
	
	/** \brief Height terrain sector vegetation layer count changed. */
	virtual void HTVLayerCountChanged( meWorld *world );
	
	/** \brief Height terrain sector active vegetation layer changed. */
	virtual void HTActiveVLayerChanged( meWorld *world );
	
	/** \brief Height terrain sector vegetation layer changed. */
	virtual void HTVLayerChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer variation count changed. */
	virtual void HTVLVariationCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector active vegetation layer variation changed. */
	virtual void HTVLActiveVariationChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer variation changed. */
	virtual void HTVLVariationChanged( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVVariation *variation );
	
	/** \brief Height terrain sector vegetation layer rule count changed. */
	virtual void HTVLRuleCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector active vegetation layer rule changed. */
	virtual void HTVLActiveRuleChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer rule changed. */
	virtual void HTVLRuleChanged( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVRule *rule );
	
	/** \brief Height terrain sector vegetation layer rule moved. */
	virtual void HTVLRuleMoved( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVRule *rule );
	
	/** \brief Height terrain sector vegetation layer link count changed. */
	virtual void HTVLLinkCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	/*@}*/
};

#endif
