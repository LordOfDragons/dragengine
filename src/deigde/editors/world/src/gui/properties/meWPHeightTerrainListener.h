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
	meWPHeightTerrainListener(meWPHeightTerrain &panel);
	
	/** \brief Clean up listener. */
	~meWPHeightTerrainListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Height paint parameters changed. */
	void HeightPaintChanged(meWorld *world) override;
	
	/** \brief Mask paint parameters changed. */
	void MaskPaintChanged(meWorld *world) override;
	
	/** \brief Visibility paint parameters changed. */
	void VisibilityPaintChanged(meWorld *world) override;
	
	
	
	/** \brief Active sector changed. */
	virtual void ActiveSectorChanged(meWorld *world);
	
	
	
	/** \brief Height terrain sector state ( changed, saved, filename ) changed. */
	void HTStateChanged(meWorld *world) override;
	
	/** \brief Height terrain changed. */
	void HTChanged(meWorld *world) override;
	
	/** \brief Height terrain sector changed. */
	void HTSChanged(meWorld *world, meHeightTerrainSector *sector) override;
	
	/** \brief Height terrain sector height changed. */
	void HTSHeightChanged(meWorld *world, meHeightTerrainSector *sector) override;
	
	/** \brief Height terrain sector visibility changed. */
	void HTSVisibilityChanged(meWorld *world, meHeightTerrainSector *sector) override;
	
	
	
	/** \brief Height terrain sector texture count changed. */
	void HTSTextureCountChanged(meWorld *world, meHeightTerrainSector *sector) override;
	
	/** \brief Height terrain sector active texture changed. */
	void HTSActiveTextureChanged(meWorld *world, meHeightTerrainSector *sector) override;
	
	/** \brief Height terrain sector texture changed. */
	void HTSTextureChanged(meWorld *world, meHeightTerrainSector *sector,
		meHeightTerrainTexture *texture) override;
	
	/** \brief Height terrain sector texture mask changed. */
	void HTSTextureMaskChanged(meWorld *world, meHeightTerrainSector *sector,
		meHeightTerrainTexture *texture) override;
	
	
	
	/** \brief Height terrain navigation space count changed. */
	void HTNavSpaceCountChanged(meWorld *world) override;
	
	/** \brief Height terrain active navigation space changed. */
	void HTActiveNavSpaceChanged(meWorld *world) override;
	
	/** \brief Notify listeners height terrain navigation space changed. */
	void HTNavSpaceChanged(meWorld *world, meHeightTerrainNavSpace *navspace) override;
	
	/** \brief Height terrain navigation space type count changed. */
	void HTNavSpaceTypeCountChanged(meWorld *world, meHeightTerrainNavSpace *navspace) override;
	
	/** \brief Height terrain navigation space active type changed. */
	void HTNavSpaceActiveTypeChanged(meWorld *world, meHeightTerrainNavSpace *navspace) override;
	
	/** \brief Notify listeners height terrain navigation space type changed. */
	void HTNavSpaceTypeChanged(meWorld *world, meHeightTerrainNavSpace *navspace,
		meHeightTerrainNavSpaceType *type) override;
	
	/** \brief Notify listeners height terrain navigation space faces changed. */
	void HTNavSpaceFacesChanged(meWorld *world, meHeightTerrainNavSpace *navspace) override;
	
	/** \brief Height terrain selected navigation space points changed. */
	void HTNavSpaceSelectedPointsChanged(meWorld *world) override;
	
	
	
	/** \brief Height terrain sector vegetation layer count changed. */
	void HTVLayerCountChanged(meWorld *world) override;
	
	/** \brief Height terrain sector active vegetation layer changed. */
	void HTActiveVLayerChanged(meWorld *world) override;
	
	/** \brief Height terrain sector vegetation layer changed. */
	void HTVLayerChanged(meWorld *world, meHTVegetationLayer *vlayer) override;
	
	
	
	/** \brief Height terrain sector vegetation layer variation count changed. */
	void HTVLVariationCountChanged(meWorld *world, meHTVegetationLayer *vlayer) override;
	
	/** \brief Height terrain sector active vegetation layer variation changed. */
	void HTVLActiveVariationChanged(meWorld *world, meHTVegetationLayer *vlayer) override;
	
	/** \brief Height terrain sector vegetation layer variation changed. */
	void HTVLVariationChanged(meWorld *world, meHTVegetationLayer *vlayer, meHTVVariation *variation) override;
	/*@}*/
};

#endif
