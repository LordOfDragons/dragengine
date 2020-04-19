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

#ifndef _MEWPHEIGHTTERRAINLISTENER_H_
#define _MEWPHEIGHTTERRAINLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWPHeightTerrain;



/**
 * \brief Height terrain properties listener.
 */
class meWPHeightTerrainListener : public meWorldNotifier{
private:
	meWPHeightTerrain &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPHeightTerrainListener( meWPHeightTerrain &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPHeightTerrainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Height paint parameters changed. */
	virtual void HeightPaintChanged( meWorld *world );
	
	/** \brief Mask paint parameters changed. */
	virtual void MaskPaintChanged( meWorld *world );
	
	/** \brief Visibility paint parameters changed. */
	virtual void VisibilityPaintChanged( meWorld *world );
	
	
	
	/** \brief Active sector changed. */
	virtual void ActiveSectorChanged( meWorld *world );
	
	
	
	/** \brief Height terrain sector state ( changed, saved, filename ) changed. */
	virtual void HTStateChanged( meWorld *world );
	
	/** \brief Height terrain changed. */
	virtual void HTChanged( meWorld *world );
	
	/** \brief Height terrain sector changed. */
	virtual void HTSChanged( meWorld *world, meHeightTerrainSector *sector );
	
	/** \brief Height terrain sector height changed. */
	virtual void HTSHeightChanged( meWorld *world, meHeightTerrainSector *sector );
	
	/** \brief Height terrain sector visibility changed. */
	virtual void HTSVisibilityChanged( meWorld *world, meHeightTerrainSector *sector );
	
	
	
	/** \brief Height terrain sector texture count changed. */
	virtual void HTSTextureCountChanged( meWorld *world, meHeightTerrainSector *sector );
	
	/** \brief Height terrain sector active texture changed. */
	virtual void HTSActiveTextureChanged( meWorld *world, meHeightTerrainSector *sector );
	
	/** \brief Height terrain sector texture changed. */
	virtual void HTSTextureChanged( meWorld *world, meHeightTerrainSector *sector,
		meHeightTerrainTexture *texture );
	
	/** \brief Height terrain sector texture mask changed. */
	virtual void HTSTextureMaskChanged( meWorld *world, meHeightTerrainSector *sector,
		meHeightTerrainTexture *texture );
	
	
	
	/** \brief Height terrain navigation space count changed. */
	virtual void HTNavSpaceCountChanged( meWorld *world );
	
	/** \brief Height terrain active navigation space changed. */
	virtual void HTActiveNavSpaceChanged( meWorld *world );
	
	/** \brief Notify listeners height terrain navigation space changed. */
	virtual void HTNavSpaceChanged( meWorld *world, meHeightTerrainNavSpace *navspace );
	
	/** \brief Height terrain navigation space type count changed. */
	virtual void HTNavSpaceTypeCountChanged( meWorld *world, meHeightTerrainNavSpace *navspace );
	
	/** \brief Height terrain navigation space active type changed. */
	virtual void HTNavSpaceActiveTypeChanged( meWorld *world, meHeightTerrainNavSpace *navspace );
	
	/** \brief Notify listeners height terrain navigation space type changed. */
	virtual void HTNavSpaceTypeChanged( meWorld *world, meHeightTerrainNavSpace *navspace,
		meHeightTerrainNavSpaceType *type );
	
	/** \brief Notify listeners height terrain navigation space faces changed. */
	virtual void HTNavSpaceFacesChanged( meWorld *world, meHeightTerrainNavSpace *navspace );
	
	/** \brief Height terrain selected navigation space points changed. */
	virtual void HTNavSpaceSelectedPointsChanged( meWorld *world );
	
	
	
	/** \brief Height terrain sector vegetation layer count changed. */
	virtual void HTVLayerCountChanged( meWorld *world );
	
	/** \brief Height terrain sector active vegetation layer changed. */
	virtual void HTActiveVLayerChanged( meWorld *world );
	
	/** \brief Height terrain sector vegetation layer changed. */
	virtual void HTVLayerChanged( meWorld *world, meHTVegetationLayer *vlayer );
	
	
	
	/** \brief Height terrain sector vegetation layer variation count changed. */
	virtual void HTVLVariationCountChanged( meWorld *world, meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector active vegetation layer variation changed. */
	virtual void HTVLActiveVariationChanged( meWorld *world, meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer variation changed. */
	virtual void HTVLVariationChanged( meWorld *world, meHTVegetationLayer *vlayer, meHTVVariation *variation );
	/*@}*/
};

#endif
