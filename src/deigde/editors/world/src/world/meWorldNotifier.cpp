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
// 
#include "meWorldNotifier.h"



// Class meWorldNotifier
/////////////////////////

// Constructor, destructor
////////////////////////////

meWorldNotifier::meWorldNotifier(){ }
meWorldNotifier::~meWorldNotifier(){ }



// Notifications
//////////////////

void meWorldNotifier::WorldParametersChanged( meWorld* ){
}

void meWorldNotifier::SkyChanged( meWorld* ){
}

void meWorldNotifier::ModeChanged( meWorld* ){
}

void meWorldNotifier::StateChanged( meWorld* ){
}

void meWorldNotifier::UndoChanged( meWorld* ){
}

void meWorldNotifier::LumimeterChanged( meWorld* ){
}

void meWorldNotifier::PathFindTestChanged( meWorld* ){
}

void meWorldNotifier::MusicChanged( meWorld *world ){
}

void meWorldNotifier::LightingChanged( meWorld* ){
}

void meWorldNotifier::EditingChanged( meWorld* ){
}

void meWorldNotifier::HeightPaintChanged( meWorld* ){
}

void meWorldNotifier::MaskPaintChanged( meWorld* ){
}

void meWorldNotifier::VisibilityPaintChanged( meWorld* ){
}

void meWorldNotifier::TriggerTableChanged( meWorld* ){
}

void meWorldNotifier::ClassHideTagsChanged( meWorld* ){
}



void meWorldNotifier::PropertiesChanged( meWorld* ){
}

void meWorldNotifier::ActivePropertyChanged( meWorld* ){
}



void meWorldNotifier::HTStateChanged( meWorld* ){
}

void meWorldNotifier::HTChanged( meWorld* ){
}

void meWorldNotifier::HTSChanged( meWorld*, meHeightTerrainSector* ){
}

void meWorldNotifier::HTSHeightChanged( meWorld *world, meHeightTerrainSector *sector ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSVisibilityChanged( meWorld *world, meHeightTerrainSector *sector ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSTextureCountChanged( meWorld *world, meHeightTerrainSector *sector ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSActiveTextureChanged( meWorld*, meHeightTerrainSector* ){
}

void meWorldNotifier::HTSTextureChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainTexture* ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSTextureMaskChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainTexture* ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSPropFieldCountChanged( meWorld *world, meHeightTerrainSector *sector ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSActivePropFieldChanged( meWorld*, meHeightTerrainSector* ){
}

void meWorldNotifier::HTSPropFieldChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainPropField* ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSPropFieldMaskChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainPropField* ){
	HTSChanged( world, sector );
}

void meWorldNotifier::HTSPFTypeCountChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainPropField *propField ){
	HTSPropFieldChanged( world, sector, propField );
}

void meWorldNotifier::HTSPFTypeChanged( meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainPropField *propField, meHeightTerrainPFType* ){
	HTSPropFieldChanged( world, sector, propField );
}



void meWorldNotifier::HTNavSpaceCountChanged( meWorld* ){
}

void meWorldNotifier::HTActiveNavSpaceChanged( meWorld* ){
}

void meWorldNotifier::HTNavSpaceChanged( meWorld*, meHeightTerrainNavSpace* ){
}

void meWorldNotifier::HTNavSpaceTypeCountChanged( meWorld *world, meHeightTerrainNavSpace *navspace ){
	HTNavSpaceChanged( world, navspace );
}

void meWorldNotifier::HTNavSpaceActiveTypeChanged( meWorld*, meHeightTerrainNavSpace* ){
}

void meWorldNotifier::HTNavSpaceTypeChanged( meWorld *world, meHeightTerrainNavSpace *navspace,
meHeightTerrainNavSpaceType* ){
	HTNavSpaceChanged( world, navspace );
}

void meWorldNotifier::HTNavSpaceFacesChanged( meWorld*, meHeightTerrainNavSpace* ){
}

void meWorldNotifier::HTNavSpaceSelectedPointsChanged( meWorld* ){
}



void meWorldNotifier::HTVLayerCountChanged( meWorld *world ){
	HTChanged( world );
}

void meWorldNotifier::HTActiveVLayerChanged( meWorld* ){
}

void meWorldNotifier::HTVLayerChanged( meWorld *world, meHTVegetationLayer* ){
	HTChanged( world );
}

void meWorldNotifier::HTVLVariationCountChanged( meWorld *world, meHTVegetationLayer *vlayer ){
	HTVLayerChanged( world, vlayer );
}

void meWorldNotifier::HTVLActiveVariationChanged( meWorld*, meHTVegetationLayer* ){
}

void meWorldNotifier::HTVLVariationChanged( meWorld *world, meHTVegetationLayer *vlayer, meHTVVariation* ){
	HTVLayerChanged( world, vlayer );
}

void meWorldNotifier::HTVLRuleCountChanged( meWorld *world, meHTVegetationLayer *vlayer ){
	HTVLayerChanged( world, vlayer );
}

void meWorldNotifier::HTVLActiveRuleChanged( meWorld*, meHTVegetationLayer* ){
}

void meWorldNotifier::HTVLRuleChanged( meWorld *world, meHTVegetationLayer *vlayer, meHTVRule* ){
	HTVLayerChanged( world, vlayer );
}

void meWorldNotifier::HTVLRuleMoved( meWorld *world, meHTVegetationLayer *vlayer, meHTVRule *rule ){
	HTVLRuleChanged( world, vlayer, rule );
}

void meWorldNotifier::HTVLLinkCountChanged( meWorld *world, meHTVegetationLayer *vlayer ){
	HTVLayerChanged( world, vlayer );
}



void meWorldNotifier::ObjectSelectionChanged( meWorld* ){
}

void meWorldNotifier::ObjectChanged( meWorld*, meObject* ){
}

void meWorldNotifier::ObjectClassChanged( meWorld *world, meObject *object ){
	ObjectChanged( world, object );
}

void meWorldNotifier::ObjectGeometryChanged( meWorld *world, meObject *object ){
	ObjectChanged( world, object );
}

void meWorldNotifier::ObjectPropertiesChanged( meWorld *world, meObject *object ){
	ObjectChanged( world, object );
}

void meWorldNotifier::ObjectActivePropertyChanged( meWorld*, meObject* ){
}

void meWorldNotifier::ObjectAdded( meWorld*, meObject* ){
}

void meWorldNotifier::ObjectRemoved( meWorld*, meObject* ){
}

void meWorldNotifier::ObjectTextureCountChanged( meWorld *world, meObject *object ){
	ObjectChanged( world, object );
}

void meWorldNotifier::ObjectActiveTextureChanged( meWorld*, meObject* ){
}

void meWorldNotifier::ObjectTextureChanged( meWorld *world, meObject *object, meObjectTexture* ){
	ObjectChanged( world, object );
}

void meWorldNotifier::ObjectTexturePropertiesChanged( meWorld *world, meObject *object,
meObjectTexture *texture ){
	ObjectTextureChanged( world, object, texture );
}

void meWorldNotifier::ObjectTextureActivePropertyChanged( meWorld*, meObject*, meObjectTexture* ){
}



void meWorldNotifier::ObjectShapeListChanged( meWorld* ){
}

void meWorldNotifier::ObjectShapeSelectionChanged( meWorld* ){
}



void meWorldNotifier::DecalSelectionChanged( meWorld* ){
}

void meWorldNotifier::DecalCountChanged( meWorld* ){
}

void meWorldNotifier::DecalChanged( meWorld*, meDecal* ){
}

void meWorldNotifier::DecalGeometryChanged( meWorld *world, meDecal *decal ){
	DecalChanged( world, decal );
}

void meWorldNotifier::DecalPropertiesChanged( meWorld *world, meDecal *decal ){
	DecalChanged( world, decal );
}

void meWorldNotifier::DecalActivePropertyChanged( meWorld*, meDecal* ){
}



void meWorldNotifier::NavSpaceSelectionChanged( meWorld* ){
}

void meWorldNotifier::NavSpaceCountChanged( meWorld* ){
}

void meWorldNotifier::NavSpaceChanged( meWorld*, meNavigationSpace* ){
}

void meWorldNotifier::NavSpaceGeometryChanged( meWorld *world, meNavigationSpace *navspace ){
	NavSpaceChanged( world, navspace );
}

void meWorldNotifier::NavSpaceUsedCostTypesChanged( meWorld*, meNavigationSpace* ){
}



void meWorldNotifier::PFSelectionChanged( meWorld* ){
}

void meWorldNotifier::PFCountChanged( meWorld* ){
}

void meWorldNotifier::PFChanged( meWorld*, mePropField* ){
}

void meWorldNotifier::PFTypeCountChanged( meWorld*, mePropField* ){
}

void meWorldNotifier::PFActiveTypeChanged( meWorld*, mePropField* ){
}

void meWorldNotifier::PFTypeChanged( meWorld *world, mePropField *field, mePropFieldType* ){
	PFChanged( world, field );
}



void meWorldNotifier::ActiveCameraChanged( meWorld* ){
}

void meWorldNotifier::CameraChanged( meWorld*, meCamera* ){
}
