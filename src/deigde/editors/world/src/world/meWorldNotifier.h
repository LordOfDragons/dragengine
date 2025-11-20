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
#ifndef _MEWORLDNOTIFIER_H_
#define _MEWORLDNOTIFIER_H_

// includes
#include "dragengine/deObject.h"
#include "dragengine/common/math/decMath.h"

class meHeightTerrainNavSpaceType;
class meHeightTerrainNavSpace;
// predefinitions
class meWorld;
class meObject;
class meObjectTexture;
class meDecal;
class meNavigationSpace;
class meCamera;
class meSkyLayer;
class meHTVRule;
class meHeightTerrain;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class meHeightTerrainPropField;
class meHeightTerrainPFType;
class meHTVegetationLayer;
class meHTVVariation;
class mePropField;
class mePropFieldType;



/**
 * @brief World Notifier.
 *
 * Notifier for changes in a world.
 */
class meWorldNotifier : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin notifier. */
	meWorldNotifier();
	/** Cleans up the skin notifier. */
	virtual ~meWorldNotifier();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** World parameters changed. */
	virtual void WorldParametersChanged( meWorld *world );
	
	/** Sky changed. */
	virtual void SkyChanged( meWorld *world );
	/** Background object changed. */
	virtual void BgObjectChanged( meWorld *world );
	
	/** Limit box changed. */
	virtual void LimitBoxChanged(meWorld *world);
	
	/** Element or workd mode changed. */
	virtual void ModeChanged( meWorld *world );
	/** Changed or saved state changed. */
	virtual void StateChanged( meWorld *world );
	/** Undos changed. */
	virtual void UndoChanged( meWorld *world );
	/** Lumimeter changed. */
	virtual void LumimeterChanged( meWorld *world );
	/** Path find test changed. */
	virtual void PathFindTestChanged( meWorld *world );
	
	/** Music changed. */
	virtual void MusicChanged( meWorld *world );
	
	/** Lighting parameters changed. */
	virtual void LightingChanged( meWorld *world );
	/** Editing parameters changed. */
	virtual void EditingChanged( meWorld *world );
	/** Height paint parameters changed. */
	virtual void HeightPaintChanged( meWorld *world );
	/** Mask paint parameters changed. */
	virtual void MaskPaintChanged( meWorld *world );
	/** Visibility paint parameters changed. */
	virtual void VisibilityPaintChanged( meWorld *world );
	/** Trigger table changed. */
	virtual void TriggerTableChanged( meWorld *world );
	/** Class (partial) hide tags changed. */
	virtual void ClassHideTagsChanged( meWorld *world );
	
	/** \brief World properties changed. */
	virtual void PropertiesChanged( meWorld *world );
	
	/** \brief World active property changed. */
	virtual void ActivePropertyChanged( meWorld *world );
	
	/** Height terrain sector state ( changed, saved, filename ) changed. */
	virtual void HTStateChanged( meWorld *world );
	
	/** \brief Height terrain changed. */
	virtual void HTChanged( meWorld *world );
	
	/** Height terrain sector changed. */
	virtual void HTSChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector height changed. */
	virtual void HTSHeightChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector visibility changed. */
	virtual void HTSVisibilityChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector texture count changed. */
	virtual void HTSTextureCountChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector active texture changed. */
	virtual void HTSActiveTextureChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector texture changed. */
	virtual void HTSTextureChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture );
	/** Height terrain sector texture mask changed. */
	virtual void HTSTextureMaskChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture );
	
	/** Height terrain sector prop field count changed. */
	virtual void HTSPropFieldCountChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector active prop field changed. */
	virtual void HTSActivePropFieldChanged( meWorld *world, meHeightTerrainSector *sector );
	/** Height terrain sector prop field changed. */
	virtual void HTSPropFieldChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	/** Height terrain sector prop field mask changed. */
	virtual void HTSPropFieldMaskChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	/** Height terrain sector prop field type count changed. */
	virtual void HTSPFTypeCountChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	/** Height terrain sector prop field type changed. */
	virtual void HTSPFTypeChanged( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainPropField *propField, meHeightTerrainPFType *type );
	
	
	
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
	
	
	
	/** Height terrain sector vegetation layer count changed. */
	virtual void HTVLayerCountChanged( meWorld *world );
	/** Height terrain sector active vegetation layer changed. */
	virtual void HTActiveVLayerChanged( meWorld *world );
	/** Height terrain sector vegetation layer changed. */
	virtual void HTVLayerChanged( meWorld *world, meHTVegetationLayer *vlayer );
	
	/** Height terrain sector vegetation layer variation count changed. */
	virtual void HTVLVariationCountChanged( meWorld *world, meHTVegetationLayer *vlayer );
	/** Height terrain sector active vegetation layer variation changed. */
	virtual void HTVLActiveVariationChanged( meWorld *world, meHTVegetationLayer *vlayer );
	/** Height terrain sector vegetation layer variation changed. */
	virtual void HTVLVariationChanged( meWorld *world, meHTVegetationLayer *vlayer, meHTVVariation *variation );
	
	/** Height terrain sector vegetation layer rule count changed. */
	virtual void HTVLRuleCountChanged( meWorld *world, meHTVegetationLayer *vlayer );
	/** Height terrain sector active vegetation layer rule changed. */
	virtual void HTVLActiveRuleChanged( meWorld *world, meHTVegetationLayer *vlayer );
	/** Height terrain sector vegetation layer rule changed. */
	virtual void HTVLRuleChanged( meWorld *world, meHTVegetationLayer *vlayer, meHTVRule *rule );
	/** Height terrain sector vegetation layer rule moved. */
	virtual void HTVLRuleMoved( meWorld *world, meHTVegetationLayer *vlayer, meHTVRule *rule );
	
	/** Height terrain sector vegetation layer link count changed. */
	virtual void HTVLLinkCountChanged( meWorld *world, meHTVegetationLayer *vlayer );
	
	/** Object selection changed. */
	virtual void ObjectSelectionChanged( meWorld *world );
	/** Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	/** Object class changed. */
	virtual void ObjectClassChanged( meWorld *world, meObject *object );
	/** Object geometry changed. */
	virtual void ObjectGeometryChanged( meWorld *world, meObject *object );
	/** Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	/** Object active property changed. */
	virtual void ObjectActivePropertyChanged( meWorld *world, meObject *object );
	
	/** Object attach behaviors changed. */
	virtual void ObjectAttachBehaviorsChanged(meWorld *world, meObject *object);
	
	/** Object active attach behavior changed. */
	virtual void ObjectActiveAttachBehaviorChanged(meWorld *world, meObject *object);
	
	/** Object added. */
	virtual void ObjectAdded( meWorld *world, meObject *object );
	/** Object removed. */
	virtual void ObjectRemoved( meWorld *world, meObject *object );
	/** Object texture count changed. */
	virtual void ObjectTextureCountChanged( meWorld *world, meObject *object );
	/** Object active texture changed. */
	virtual void ObjectActiveTextureChanged( meWorld *world, meObject *object );
	/** Object texture changed. */
	virtual void ObjectTextureChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	/** Object texture properties changed. */
	virtual void ObjectTexturePropertiesChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	/** Object texture active property changed. */
	virtual void ObjectTextureActivePropertyChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	
	/** Object shape list changed. */
	virtual void ObjectShapeListChanged( meWorld *world );
	/** Object shape selection changed. */
	virtual void ObjectShapeSelectionChanged( meWorld *world );
	
	/** Decal selection changed. */
	virtual void DecalSelectionChanged( meWorld *world );
	/** Decal count changed. */
	virtual void DecalCountChanged( meWorld *world );
	/** Decal changed. */
	virtual void DecalChanged( meWorld *world, meDecal *decal );
	/** Decal geometry changed. */
	virtual void DecalGeometryChanged( meWorld *world, meDecal *decal );
	/** Decal properties changed. */
	virtual void DecalPropertiesChanged( meWorld *world, meDecal *decal );
	/** Decal active property changed. */
	virtual void DecalActivePropertyChanged( meWorld *world, meDecal *decal );
	
	/** Navigation space selection changed. */
	virtual void NavSpaceSelectionChanged( meWorld *world );
	/** Navigation space count changed. */
	virtual void NavSpaceCountChanged( meWorld *world );
	/** Navigation space changed. */
	virtual void NavSpaceChanged( meWorld *world, meNavigationSpace *navspace );
	/** Navigation space geometry changed. */
	virtual void NavSpaceGeometryChanged( meWorld *world, meNavigationSpace *navspace );
	/** Navigation space used cost types changed. */
	virtual void NavSpaceUsedCostTypesChanged( meWorld *world, meNavigationSpace *navspace );
	
	/** Notifies all that the prop field selection changed. */
	virtual void PFSelectionChanged( meWorld *world );
	/** Notifies all that the prop field count changed. */
	virtual void PFCountChanged( meWorld *world );
	/** Notifies all that a prop field changed. */
	virtual void PFChanged( meWorld *world, mePropField *field );
	/** Notifies all that a prop field type count changed. */
	virtual void PFTypeCountChanged( meWorld *world, mePropField *field );
	/** Notifies all that a prop field active type changed. */
	virtual void PFActiveTypeChanged( meWorld *world, mePropField *field );
	/** Notifies all that a prop field type changed. */
	virtual void PFTypeChanged( meWorld *world, mePropField *field, mePropFieldType *type );
	
	/** Active camera changed. */
	virtual void ActiveCameraChanged( meWorld *world );
	/** Camera changed. */
	virtual void CameraChanged( meWorld *world, meCamera *camera );
	/*@}*/
};

// end of include only once
#endif
