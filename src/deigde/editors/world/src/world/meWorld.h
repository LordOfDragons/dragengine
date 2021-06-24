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

#ifndef _MEWORLD_H_
#define _MEWORLD_H_

#include <deigde/triggersystem/igdeTriggerTargetList.h>
#include <deigde/triggersystem/igdeTriggerExpressionParser.h>
#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/utils/decUniqueID.h>

#include "object/meObjectList.h"
#include "object/meObjectSelection.h"
#include "objectshape/meObjectShapeSelection.h"
#include "decal/meDecalList.h"
#include "decal/meDecalSelection.h"
#include "navspace/meNavigationSpaceList.h"
#include "navspace/meNavigationSpaceSelection.h"
#include "idgroup/meIDGroupList.h"

class decCollisionFilter;
class meHeightTerrainNavSpaceType;
class meHeightTerrainNavSpace;
class deMicrophone;
class deEngine;
class deWorld;
class deImage;
class deCollider;

class meCamera;
class meLumimeter;
class mePathFindTest;
class meUndoSystem;
class meHeightTerrain;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class meHeightTerrainPropField;
class meHeightTerrainPFType;
class meHTVegetationLayer;
class meHTVVariation;
class meHTVRule;
class mePropField;
class mePropFieldType;

class meWorldNotifier;
class meNavigationSpace;
class meObject;
class meObjectTexture;
class meDecal;

class meWeather;

class meWorldGuiParameters;

class igdeEnvironment;
class igdeWSky;

class deForceField;

class deBaseScriptingCollider;
class deColliderVolume;
class decLayerMask;

class deVideo;
class deVideoPlayer;
class deComponent;
class deDynamicSkin;
class deLogger;



/**
 * @brief World Wrapper.
 * Stores information about a loaded world as well as wrapping the
 * engine side resources required for rendering. This class also
 * provides helper functions to deal with positions across sector
 * boundaries.
 */
class meWorld : public igdeEditableEntity{
public:
	/** Layer masks for colliders. */
	enum eColliderLayerMasks{
		/** Editing. */
		eclmEditing,
		
		/** Prop fields. */
		eclmPropFields,
		
		/** Height terrains. */
		eclmHeightTerrains,
		
		/** Objects. */
		eclmObjects,
		
		/** Decals. */
		eclmDecals,
		
		/** Wind. */
		      eclmForceField,
		
		/** AI. */
		eclmAI,
		
		/** Object Shape. */
		eclmObjectShape,
		
		/** Snap point. */
		eclmSnapPoint,
		
		/** Particles. */
		eclmParticles
	};
	
	/** Layer masks. */
	enum eLayerMasks {
		/** Camera view. */
		elmCamera,
		
		/** Environment map probes. */
		elmEnvMapProbes,
		
		/** Audio. */
		elmAudio
	};
	
private:
	deWorld *pDEWorld;
	deColliderVolume *pEngColCollider;
	igdeWSky *pSky;
	deMicrophone *pEngMicrophone;
	
	deForceField *pEngForceField;
	
	decDVector pSize;
	decVector pGravity;
	meHeightTerrain *pHeightTerrain;
	
	meObjectList pObjects;
	meDecalList pDecals;
	meNavigationSpaceList pNavSpaces;
	decStringDictionary pProperties;
	decString pActiveProperty;
	
	decObjectDictionary pObjectIDMap;
	
	meWeather *pWeather;
	
	meObjectShapeList pObjectShapes;
	
	meObjectSelection pSelectionObject;
	meObjectShapeSelection pSelectionObjectShape;
	meDecalSelection pSelectionDecal;
	meNavigationSpaceSelection pSelectionNavigationSpace;
	
	meCamera *pFreeRoamCamera;
	meCamera *pPlayerCamera;
	meCamera *pActiveCamera;
	
	meLumimeter *pLumimeter;
	mePathFindTest *pPathFindTest;
	
	bool pDepChanged;
	
	bool pFullBright;
	
	decString pPathNavTest;
	
	meWorldGuiParameters *pGuiParams;
	igdeTriggerTargetList pTriggerTable;
	igdeTriggerExpressionParser pTriggerExpressionParser;
	meIDGroupList pIDGroupList;
	
	decUniqueID pNextObjectID;
	
	meWorldNotifier **pNotifiers;
	int pNotifierCount;
	int pNotifierSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new world object linked to the given engine. */
	meWorld( igdeEnvironment *environment );
	
protected:
	/** Cleans up the world object. */
	virtual ~meWorld();
	/*@}*/
	
	
	
public:
	/** @name Management */
	/*@{*/
	/** Retrieves the engine side world resource. */
	inline deWorld *GetEngineWorld() const{ return pDEWorld; }
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the microphone. */
	inline deMicrophone *GetMicrophone() const{ return pEngMicrophone; }
	
	/** Retrieves the height terrain. */
	inline meHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	/** Retrieves the weather. */
	inline meWeather *GetWeather() const{ return pWeather; }
	/** Retrieves the gui parameters. */
	inline meWorldGuiParameters &GetGuiParameters() const{ return *pGuiParams; }
	/** Retrieves the trigger table. */
	inline igdeTriggerTargetList &GetTriggerTable(){ return pTriggerTable; }
	inline const igdeTriggerTargetList &GetTriggerTable() const{ return pTriggerTable; }
	/** Retrieves the trigger expression parser. */
	inline igdeTriggerExpressionParser &GetTriggerExpressionParser(){ return pTriggerExpressionParser; }
	inline const igdeTriggerExpressionParser &GetTriggerExpressionParser() const{ return pTriggerExpressionParser; }
	
	/** Identifier group list. */
	inline meIDGroupList &GetIDGroupList(){ return pIDGroupList; }
	inline const meIDGroupList &GetIDGroupList() const{ return pIDGroupList; }
	
	
	
	/** Next object ID. */
	inline const decUniqueID &GetNextObjectID() const { return pNextObjectID; }
	
	/** Set next object ID. */
	void SetNextObjectID( const decUniqueID &id );
	
	/** Next object ID and increment. */
	decUniqueID NextObjectID();
	
	
	
	/** Dispose of the world. */
	void Dispose();
	/** Resets the world. */
	void Reset();
	
	/** Init the delegates. */
	void InitDelegates();
	/*@}*/
	
	/** @name Collision Detection */
	/*@{*/
	/** Tests for collisions of a box. */
	void CollisionTestBox( const decDVector &position, const decQuaternion &orientation, const decVector &halfExtends,
		deBaseScriptingCollider *listener, const decCollisionFilter &filter );
	
	void CollisionTestBox( const decDVector &position, const decVector &minExtend, const decVector &maxExtend,
		const decQuaternion &orientation, deBaseScriptingCollider *listener, const decCollisionFilter &filter );
	/*@}*/
	
	/** @name Editing */
	/*@{*/
	/** Determines if a world dependency has changed. */
	inline bool GetDepChanged() const{ return pDepChanged; }
	/** Sets if a world dependency has changed. */
	void SetDepChanged( bool changed );
	
	/** World changed or world dependency changed. */
	inline bool GetAnyChanged() const{ return GetChanged() || pDepChanged; }
	
	/** Checks the changed state of all sectors adjusting the world changed state. */
	void CheckChanged();
	/** Checks the dep-changed state of all sectors adjusting the world dep-changed state. */
	void CheckDepChanged();
	
	/** Forces an update of the vegetation. */
	void ForceUpdateVegetation( bool fullUpdate );
	/** Clears vegetation. */
	void ClearVegetation();
	
	/** Element mode changed. */
	void ElementModeChanged();
	
	/** Element visibility changed. */
	void ElementVisibilityChanged();
	
	/** Enable GI changed. */
	void EnableGIChanged();
	
	/** Clear scaling of non-scaled elements. */
	void ClearScalingOfNonScaledElements();
	
	/** Retrieves the editing object shape list. */
	inline meObjectShapeList &GetObjectShapes(){ return pObjectShapes; }
	inline const meObjectShapeList &GetObjectShapes() const{ return pObjectShapes; }
	/*@}*/
	
	
	
	/** \name Objects */
	/*@{*/
	/** List of objects. */
	inline const meObjectList &GetObjects() const{ return pObjects; }
	
	/** Add object. */
	void AddObject( meObject *object );
	
	/** Remove object. */
	void RemoveObject( meObject *object );
	
	/** Remove all objects. */
	void RemoveAllObjects();
	
	/** Reassin object identifiers. */
	void ReassignObjectIDs();
	
	/** Object by ID or \em NULL if absent. */
	meObject *GetObjectWithID( const decUniqueID &id ) const;
	
	/** Object by ID in hex format or \em NULL if absent. */
	meObject *GetObjectWithID( const char *hexID ) const;
	/*@}*/
	
	
	
	/** \name Decals */
	/*@{*/
	/** List of decals. */
	inline const meDecalList &GetDecals() const{ return pDecals; }
	
	/** Add decal. */
	void AddDecal( meDecal *decal );
	
	/** Remove decal. */
	void RemoveDecal( meDecal *decal );
	
	/** Remove all decals. */
	void RemoveAllDecals();
	/*@}*/
	
	
	
	/** \name Navigation Spaces */
	/*@{*/
	/** List of navigation spaces. */
	inline const meNavigationSpaceList &GetNavSpaces() const{ return pNavSpaces; }
	
	/** Add navigation space. */
	void AddNavSpace( meNavigationSpace *navspace );
	
	/** Remove navigation space. */
	void RemoveNavSpace( meNavigationSpace *navspace );
	
	/** Remove all navigation spaces. */
	void RemoveAllNavSpaces();
	/*@}*/
	
	
	
	/** @name World Parameters */
	/*@{*/
	
	/** Size of world in meters. */
	inline const decDVector &GetSize() const{ return pSize; }
	
	/** Set size of world in meters. */
	void SetSize( const decDVector &size );
	
	/** World gravity. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/** Set world gravity. */
	void SetGravity( const decVector &gravity );
	
	/** Determines if the world is rendered in full bright mode. */
	inline bool GetFullBright() const{ return pFullBright; }
	/** Sets if the world is rendered in full bright mode. */
	void SetFullBright( bool fullBright );
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** Set property. */
	void SetProperty( const char *key, const char *value );
	
	/** Set properties. */
	void SetProperties( const decStringDictionary &properties );
	
	/** Remove property if present. */
	void RemoveProperty( const char *key );
	
	/** Removes all properties. */
	void RemoveAllProperties();
	
	/** Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** Set active property. */
	void SetActiveProperty( const char *property );
	/*@}*/
	
	
	
	/** @name Selection */
	/*@{*/
	/** Retrieves the object selection. */
	inline meObjectSelection &GetSelectionObject(){ return pSelectionObject; }
	inline const meObjectSelection &GetSelectionObject() const{ return pSelectionObject; }
	/** Retrieves the object shape selection. */
	inline meObjectShapeSelection &GetSelectionObjectShape(){ return pSelectionObjectShape; }
	inline const meObjectShapeSelection &GetSelectionObjectShape() const{ return pSelectionObjectShape; }
	/** Retrieves the decal selection. */
	inline meDecalSelection &GetSelectionDecal(){ return pSelectionDecal; }
	inline const meDecalSelection &GetSelectionDecal() const{ return pSelectionDecal; }
	/** Retrieves the navigation space selection. */
	inline meNavigationSpaceSelection &GetSelectionNavigationSpace(){ return pSelectionNavigationSpace; }
	inline const meNavigationSpaceSelection &GetSelectionNavigationSpace() const{ return pSelectionNavigationSpace; }
	/*@}*/
	
	/** @name Camera */
	/*@{*/
	/** Retrieves the free roaming camera. */
	inline meCamera *GetFreeRoamingCamera() const{ return pFreeRoamCamera; }
	/** Retrieves the player camera. */
	inline meCamera *GetPlayerCamera() const{ return pPlayerCamera; }
	/** Retrieves the active camera. */
	inline meCamera *GetActiveCamera() const{ return pActiveCamera; }
	/** Sets the active camera. */
	void SetActiveCamera( meCamera *camera );
	/*@}*/
	
	/** @name Sensors */
	/*@{*/
	/** Retrieves the lumimeter sensor. */
	inline meLumimeter *GetLumimeter() const{ return pLumimeter; }
	/** Updates sensors. */
	void UpdateSensors();
	/*@}*/
	
	/** @name Testing */
	/*@{*/
	/** Retrieves the path find test. */
	inline mePathFindTest *GetPathFindTest() const{ return pPathFindTest; }
	/** Retrieves the last path used for loading/saving navigation test files. */
	inline const decString &GetPathNavTest() const{ return pPathNavTest; }
	/** Sets the last path used for loading/saving navigation test files. */
	void SetPathNavTest( const char *path );
	/*@}*/
	
	/** Activate microphone. */
	void ActivateMicrophone();
	
	void UpdateDEWorld( float elapsed );
	
	/** Game definition. */
	void GameDefChanged();
	
	/** @name Notifiers */
	/*@{*/
	/** Retrieves the number of notifiers. */
	inline int GetNotifierCount() const{ return pNotifierCount; }
	/** Retrieves the notifier at the given index. */
	meWorldNotifier *GetNotifierAt( int index ) const;
	/** Retrieves the index of the notifier or -1 if not found. */
	int IndexOfNotifier( meWorldNotifier *notifier ) const;
	/** Determines if the notifier exists. */
	bool HasNotifier( meWorldNotifier *notifier ) const;
	/** Adds a new notifier. */
	void AddNotifier( meWorldNotifier *notifier );
	/** Removes the given notifier. */
	void RemoveNotifier( meWorldNotifier *notifier );
	/** Removes all notifiers. */
	void RemoveAllNotifiers();
	
	/** Notify world parameters changed. */
	void NotifyWorldParametersChanged();
	
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the element or work mode changed. */
	void NotifyModeChanged();
	/** Notifies all that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all that the undos changed. */
	virtual void NotifyUndoChanged();
	/** Notifies all that the lumimeter changed. */
	void NotifyLumimeterChanged();
	/** Notifies all that the path find test changed. */
	void NotifyPathFindTestChanged();
	/** Notifies all that the lighting changed. */
	void NotifyLightingChanged();
	/** Notifies all that the editing parameters changed. */
	void NotifyEditingChanged();
	/** Notifies all that the height paint parameters changed. */
	void NotifyHeightPaintChanged();
	/** Notifies all that the mask paint parameters changed. */
	void NotifyMaskPaintChanged();
	/** Notifies all that the visibility paint parameters changed. */
	void NotifyVisibilityPaintChanged();
	/** Notifies all that the trigger table changed. */
	void NotifyTriggerTableChanged();
	/** Notifies all that the class (partial) hide tags changed. */
	void NotifyClassHideTagsChanged();
	
	/** Notify all world changed properties. */
	void NotifyPropertiesChanged();
	
	/** Notify all world active property changed. */
	void NotifyActivePropertyChanged();
	
	/** Notifies all that a height terrain sector state ( changed, saved, filename ) changed. */
	void NotifyHTChanged();
	/** Notifies all that a height terrain sector state ( changed, saved, filename ) changed. */
	void NotifyHTStateChanged();
	/** Notifies all that a height terrain sector changed. */
	void NotifyHTSChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector height changed. */
	void NotifyHTSHeightChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector visibility changed. */
	void NotifyHTSVisibilityChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector texture count changed. */
	void NotifyHTSTextureCountChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector active texture changed. */
	void NotifyHTSActiveTextureChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector texture changed. */
	void NotifyHTSTextureChanged( meHeightTerrainSector *sector, meHeightTerrainTexture *texture );
	/** Notifies all that a height terrain sector texture mask changed. */
	void NotifyHTSTextureMaskChanged( meHeightTerrainSector *sector, meHeightTerrainTexture *texture );
	
	/** Notify listener height terrain navigation space count changed. */
	void NotifyHTNavSpaceCountChanged();
	
	/** Notify listeners height terrain active navigation space changed. */
	void NotifyHTActiveNavSpaceChanged();
	
	/** Notify listeners height terrain navigation space changed. */
	void NotifyHTNavSpaceChanged( meHeightTerrainNavSpace *navspace );
	
	/** Height terrain navigation space type count changed. */
	void NotifyHTNavSpaceTypeCountChanged( meHeightTerrainNavSpace *navspace );
	
	/** Height terrain navigation space active type changed. */
	void NotifyHTNavSpaceActiveTypeChanged( meHeightTerrainNavSpace *navspace );
	
	/** Notify listeners height terrain navigation space type changed. */
	void NotifyHTNavSpaceTypeChanged( meHeightTerrainNavSpace *navspace, meHeightTerrainNavSpaceType *type );
	
	/** Notify listeners height terrain navigation space faces changed. */
	void NotifyHTNavSpaceFacesChanged( meHeightTerrainNavSpace *navspace );
	
	/** Height terrain selected navigation space points changed. */
	void NotifyHTNavSpaceSelectedPointsChanged();
	
	/** Notifies all that a height terrain sector prop field count changed. */
	void NotifyHTSPropFieldCountChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector active prop field changed. */
	void NotifyHTSActivePropFieldChanged( meHeightTerrainSector *sector );
	/** Notifies all that a height terrain sector prop field changed. */
	void NotifyHTSPropFieldChanged( meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	/** Notifies all that a height terrain sector prop field mask changed. */
	void NotifyHTSPropFieldMaskChanged( meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	
	/** Notifies all that a height terrain sector prop field type count changed. */
	void NotifyHTSPFTypeCountChanged( meHeightTerrainSector *sector, meHeightTerrainPropField *propField );
	/** Notifies all that a height terrain sector prop field type changed. */
	void NotifyHTSPFTypeChanged( meHeightTerrainSector *sector, meHeightTerrainPropField *propField, meHeightTerrainPFType *type );
	
	/** Notifies all that a height terrain sector vegetation layer count changed. */
	void NotifyHTVLayerCountChanged();
	/** Notifies all that a height terrain sector active vegetation layer changed. */
	void NotifyHTActiveVLayerChanged();
	/** Notifies all that a height terrain sector vegetation layer changed. */
	void NotifyHTVLayerChanged( meHTVegetationLayer *vlayer );
	/** Notifies all that a height terrain sector vegetation layer variation count changed. */
	void NotifyHTVLVariationCountChanged( meHTVegetationLayer *vlayer );
	/** Notifies all that a height terrain sector active vegetation layer variation changed. */
	void NotifyHTVLActiveVariationChanged( meHTVegetationLayer *vlayer );
	/** Notifies all that a height terrain sector vegetation layer variation changed. */
	void NotifyHTVLVariationChanged( meHTVegetationLayer *vlayer, meHTVVariation *variation );
	/** Notifies all that a height terrain sector vegetation layer rule count changed. */
	void NotifyHTVLRuleCountChanged( meHTVegetationLayer *vlayer );
	/** Notifies all that a height terrain sector active vegetation layer rule changed. */
	void NotifyHTVLActiveRuleChanged( meHTVegetationLayer *vlayer );
	/** Notifies all that a height terrain sector vegetation layer rule changed. */
	void NotifyHTVLRuleChanged( meHTVegetationLayer *vlayer, meHTVRule *rule );
	/** Notifies all that a height terrain sector vegetation layer rule moved. */
	void NotifyHTVLRuleMoved( meHTVegetationLayer *vlayer, meHTVRule *rule );
	/** Notifies all that a height terrain sector vegetation layer link count changed. */
	void NotifyHTVLLinkCountChanged( meHTVegetationLayer *vlayer );
	
	/** Notifies all that the object selection changed. */
	void NotifyObjectSelectionChanged();
	/** Notifies all that an object has changed. */
	void NotifyObjectChanged( meObject *object );
	/** Notifies all that an object changed class. */
	void NotifyObjectClassChanged( meObject *object );
	/** Notifies all that an object changed geometry. */
	void NotifyObjectGeometryChanged( meObject *object );
	/** Notifies all that an object changed properties. */
	void NotifyObjectPropertiesChanged( meObject *object );
	/** Notifies all that a object active property changed. */
	void NotifyObjectActivePropertyChanged( meObject *object );
	/** Notifies all that an object has been added. */
	void NotifyObjectAdded( meObject *object );
	/** Notifies all that an object has been removed. */
	void NotifyObjectRemoved( meObject *object );
	/** Notifies all that a object texture count changed. */
	void NotifyObjectTextureCountChanged( meObject *object );
	/** Notifies all that a object active texture changed. */
	void NotifyObjectActiveTextureChanged( meObject *object );
	/** Notifies all that a object texture changed. */
	void NotifyObjectTextureChanged( meObject *object, meObjectTexture *texture );
	/** Notifies all that an object texture changed properties. */
	void NotifyObjectTexturePropertiesChanged( meObject *object, meObjectTexture *texture );
	/** Notifies all that an object texture active property changed. */
	void NotifyObjectTextureActivePropertyChanged( meObject *object, meObjectTexture *texture );
	
	/** Notifies all that the object shape list changed. */
	void NotifyObjectShapeListChanged();
	/** Notifies all that the object shape list selection changed. */
	void NotifyObjectShapeSelectionChanged();
	
	/** Notifies all that the decal selection changed. */
	void NotifyDecalSelectionChanged();
	/** Notifies all that the decal count changed. */
	void NotifyDecalCountChanged();
	/** Notifies all that an decal has changed. */
	void NotifyDecalChanged( meDecal *decal );
	/** Notifies all that a decal geometry changed. */
	void NotifyDecalGeometryChanged( meDecal *decal );
	/** Notifies all that an decal changed properties. */
	void NotifyDecalPropertiesChanged( meDecal *decal );
	/** Notifies all that a decal active property changed. */
	void NotifyDecalActivePropertyChanged( meDecal *decal );
	
	/** Notifies all that the navigation space selection changed. */
	void NotifyNavSpaceSelectionChanged();
	/** Notifies all that the navigation space count changed. */
	void NotifyNavSpaceCountChanged();
	/** Notifies all that an navigation space has changed. */
	void NotifyNavSpaceChanged( meNavigationSpace *navspace );
	/** Notifies all that a navigation space geometry changed. */
	void NotifyNavSpaceGeometryChanged( meNavigationSpace *navspace );
	/** Notifies all that a navigation space used cost types changed. */
	void NotifyNavSpaceUsedCostTypesChanged( meNavigationSpace *navspace );
	
	/** Notifies all that the prop field selection changed. */
	void NotifyPFSelectionChanged();
	/** Notifies all that the prop field count changed. */
	void NotifyPFCountChanged();
	/** Notifies all that a prop field changed. */
	void NotifyPFChanged( mePropField *field );
	/** Notifies all that a prop field type count changed. */
	void NotifyPFTypeCountChanged( mePropField *field );
	/** Notifies all that a prop field active type changed. */
	void NotifyPFActiveTypeChanged( mePropField *field );
	/** Notifies all that a prop field type changed. */
	void NotifyPFTypeChanged( mePropField *field, mePropFieldType *type );
	
	/** Notifies all that the active camera changed. */
	void NotifyActiveCameraChanged();
	/** Notifies all that the camera parameters changed. */
	void NotifyCameraChanged( meCamera *camera );
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateAmbientLight();
	void pShowStateChanged();
};

// end of include only once
#endif
