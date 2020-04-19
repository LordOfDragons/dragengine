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

#ifndef _MEWINDOWVEGETATION_H_
#define _MEWINDOWVEGETATION_H_

#include <deigde/gui/nodeview/igdeNVBoard.h>

class meWindowMain;
class meWindowVegetationListener;
class meHTVRule;
class meWVNode;

class meHTVegetationLayer;
class meWorld;



/**
 * \brief Vegetation Window.
 */
class meWindowVegetation : public igdeNVBoard{
private:
	meWindowMain &pWindowMain;
	meWindowVegetationListener *pListener;
	
	meWorld *pWorld;
	decPoint pSector;
	meHTVegetationLayer *pVLayer;
	
	float pUnitsToPixel;
	float pPixelToUnits;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meWindowVegetation( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~meWindowVegetation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Sector coordinates. */
	inline const decPoint &GetSector() const{ return pSector; }
	
	/** \brief Set sector coordinates. */
	void SetSector( const decPoint &sector );
	
	/** \brief Vegetation layer. */
	inline meHTVegetationLayer *GetVLayer() const{ return pVLayer; }
	
	/** \brief Set vegetation layer. */
	void SetVLayer( meHTVegetationLayer *vlayer );
	
	/** \brief Active vegetation layer rule. */
	meHTVRule *GetVRule() const;
	
	
	
	/** \brief Update nodes from vegetation layer. */
	void UpdateNodesFromVLayer();
	
	/** \brief Update links from vegetation layer. */
	void UpdateLinksFromVLayer();
	
	/** \brief Select node matching active rule. */
	void SelectNodeOfActiveRule();
	
	/** \brief Units to pixel factor. */
	inline float GetUnitsToPixel() const{ return pUnitsToPixel; }
	
	/** \brief Pixel to units factor. */
	inline float GetPixelToUnits() const{ return pPixelToUnits; }
	
	/** \brief Node with rule. */
	meWVNode *GetNodeWithRule( meHTVRule *rule );
	
	/** \brief Update node with rule. */
	void UpdateNodeWithRule( meHTVRule *rule );
	
	/** \brief Node with rule moved. */
	void NodeWithRuleMoved( meHTVRule *rule );
	/*@}*/
};

#endif
