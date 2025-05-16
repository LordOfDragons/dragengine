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

#ifndef _GDEGAMEDEFINITION_H_
#define _GDEGAMEDEFINITION_H_

#include "category/gdeCategoryList.h"
#include "objectClass/gdeObjectClassList.h"
#include "particleemitter/gdeParticleEmitterList.h"
#include "property/gdePropertyList.h"
#include "skin/gdeSkinList.h"
#include "sky/gdeSkyList.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include "objectClass/world/gdeOCWorld.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringSet.h>

class gdeGameDefinitionListener;
class gdeLoadSaveSystem;
class gdeOCInherit;
class gdeOCBillboard;
class gdeOCCamera;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeOCEnvMapProbe;
class gdeOCLight;
class gdeOCNavigationBlocker;
class gdeOCNavigationSpace;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeOCWorld;
class gdeSkyController;

class igdeWSky;
class igdeWObject;
class igdeCamera;

class deWorld;
class deVirtualFileSystem;



/**
 * Game definition.
 */
class gdeGameDefinition : public igdeEditableEntity{
public:
	/** Selectec object type. */
	enum eObjectTypes{
		/** No selection. */
		eotNoSelection,
		
		/** Category object class. */
		eotCategoryObjectClass,
		
		/** Category skin. */
		eotCategorySkin,
		
		/** Category sky. */
		eotCategorySky,
		
		/** Category particle emitter. */
		eotCategoryParticleEmitter,
		
		/** Object class. */
		eotObjectClass,
		
		/** Object class billboard. */
		eotOCBillboard,
		
		/** Object class camera. */
		eotOCCamera,
		
		/** Object class component. */
		eotOCComponent,
		
		/** Object class environment map probe. */
		eotOCEnvMapProbe,
		
		/** Object class light. */
		eotOCLight,
		
		/** Object class navigation blocker. */
		eotOCNavigationBlocker,
		
		/** Object class navigation space. */
		eotOCNavigationSpace,
		
		/** Object class particle emitter. */
		eotOCParticleEmitter,
		
		/** Object class force field. */
		eotOCForceField,
		
		/** Object class snap point. */
		eotOCSnapPoint,
		
		/** Object class speaker. */
		eotOCSpeaker,
		
		/** Object class world. */
		eotOCWorld,
		
		/** Skin. */
		eotSkin,
		
		/** Sky. */
		eotSky,
		
		/** Particle emitter. */
		eotParticleEmitter
	};
	
	
	
private:
	deWorld *pWorld;
	
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	
	igdeCamera *pCamera;
	float pViewRatio;
	
	bool pIsProjectGameDef;
	
	decString pID;
	decString pDescription;
	decString pBasePath;
	decString pVFSPath;
	decString pScriptModule;
	
	decStringList pBaseGameDefinitionIDList;
	decObjectList pBaseGameDefinitions;
	
	decString pDefaultObjectClass;
	decString pDefaultSkin;
	decString pDefaultSky;
	
	gdeCategoryList pCategoriesObjectClass;
	decStringSet pObjectClassCategoryNameList;
	gdeCategoryList pCategoriesSkin;
	gdeCategoryList pCategoriesSky;
	gdeCategoryList pCategoriesParticleEmitter;
	gdeCategory *pActiveCategory;
	
	decStringSet pUsedTagsObjectClass;
	
	gdeObjectClassList pObjectClasses;
	gdeObjectClass *pActiveObjectClass;
	gdeOCBillboard *pActiveOCBillboard;
	gdeOCCamera *pActiveOCCamera;
	gdeOCComponent *pActiveOCComponent;
	gdeOCEnvMapProbe *pActiveOCEnvMapProbe;
	gdeOCLight *pActiveOCLight;
	gdeOCNavigationBlocker *pActiveOCNavigationBlocker;
	gdeOCNavigationSpace *pActiveOCNavigationSpace;
	gdeOCParticleEmitter *pActiveOCParticleEmitter;
	gdeOCForceField *pActiveOCForceField;
	gdeOCSnapPoint *pActiveOCSnapPoint;
	gdeOCSpeaker *pActiveOCSpeaker;
	gdeOCWorld::Ref pActiveOCWorld;
	decStringSet pClassNameList;
	
	gdeParticleEmitterList pParticleEmitters;
	gdeParticleEmitter *pActiveParticleEmitter;
	
	gdeSkinList pSkins;
	gdeSkin *pActiveSkin;
	
	gdeSkyList pSkies;
	gdeSky *pActiveSky;
	
	gdePropertyList pWorldProperties;
	gdePropertyList pDecalProperties;
	
	decStringList pAutoFindPathObjectClasses;
	decStringList pAutoFindPathSkins;
	decStringList pAutoFindPathSkies;
	
	eObjectTypes pSelectedObjectType;
	
	decStringSet pDefinedIDs;
	decStringSet pUsedIDs;
	
	decObjectSet pListeners;
	
	deVirtualFileSystem *pVFS;
	deVirtualFileSystem *pPreviewVFS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create game definition. */
	gdeGameDefinition( igdeEnvironment *environment );
	
	/** Clean up speech animation. */
	virtual ~gdeGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** World. */
	inline deWorld *GetWorld() const{ return pWorld; }
	
	/** Camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** View ratio used to properly align camera on preview objects. */
	inline float GetViewRatio() const{ return pViewRatio; }
	
	/** Set view ratio used to properly align camera on preview objects. */
	void SetViewRatio( float viewRatio );
	
	/** Sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	
	/** Environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** Is project game definition. */
	inline bool GetIsProjectGameDef() const{ return pIsProjectGameDef; }
	
	/** Set if this is project game definition. */
	void SetIsProjectGameDef( bool isProjectGameDef );
	
	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** Set description. */
	void SetDescription( const char *description );
	
	/** Get base path. */
	inline const decString &GetBasePath() const{ return pBasePath; }
	
	/** Set base path. */
	void SetBasePath( const char *path );
	
	/** Get vfs path. */
	inline const decString &GetVFSPath() const{ return pVFSPath; }
	
	/** Set vfs path. */
	void SetVFSPath( const char *path );
	
	/** Script module this game definition is compatible with. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** Set script module this game definition is compatible with. */
	void SetScriptModule( const char *identifier );
	
	
	
	/** Base game definition id list. */
	inline const decStringList &GetBaseGameDefinitionIDList() const{ return pBaseGameDefinitionIDList; }
	
	/** Set base game definition id list. */
	void SetBaseGameDefinitionIDList( const decStringList &ids );
	
	/** Update list of base game definitions. */
	void UpdateBaseGameDefinitions( gdeLoadSaveSystem &loadSaveSystem );
	
	/** Number of base game definitions. */
	int GetBaseGameDefinitionCount() const;
	
	/** Get base game definition at index. */
	const gdeGameDefinition &GetBaseGameDefinitionAt( int index ) const;
	
	
	
	/** Path to object class. */
	inline const decString &GetDefaultObjectClass() const{ return pDefaultObjectClass; }
	
	/** Set path to object class. */
	void SetDefaultObjectClass( const char *objectClass );
	
	/** Path to skin. */
	inline const decString &GetDefaultSkin() const{ return pDefaultSkin; }
	
	/** Set path to skin. */
	void SetDefaultSkin( const char *skin );
	
	/** Path to sky. */
	inline const decString &GetDefaultSky() const{ return pDefaultSky; }
	
	/** Set path to sky. */
	void SetDefaultSky( const char *sky );
	
	
	
	/** Object class categories. */
	inline gdeCategoryList &GetCategoriesObjectClass(){ return pCategoriesObjectClass; }
	inline const gdeCategoryList &GetCategoriesObjectClass() const{ return pCategoriesObjectClass; }
	
	/** Find object class category with path or \em NULL if absent. */
	const gdeCategory *FindCategoryObjectClass( const char *path ) const;
	
	/** Object classes category names. */
	const decStringSet &GetObjectClassCategoryNameList();
	
	/** Skin categories. */
	inline gdeCategoryList &GetCategoriesSkin(){ return pCategoriesSkin; }
	inline const gdeCategoryList &GetCategoriesSkin() const{ return pCategoriesSkin; }
	
	/** Sky categories. */
	inline gdeCategoryList &GetCategoriesSky(){ return pCategoriesSky; }
	inline const gdeCategoryList &GetCategoriesSky() const{ return pCategoriesSky; }
	
	/** Particle emitter categories. */
	inline gdeCategoryList &GetCategoriesParticleEmitter(){ return pCategoriesParticleEmitter; }
	inline const gdeCategoryList &GetCategoriesParticleEmitter() const{ return pCategoriesParticleEmitter; }
	
	/** Active category or \em NULL if not set. */
	inline gdeCategory *GetActiveCategory() const{ return pActiveCategory; }
	
	/** Set active category or \em NULL if not set. */
	void SetActiveCategory( gdeCategory *category );
	
	
	
	/** Object class used tags. */
	inline decStringSet &GetUsedTagsObjectClass(){ return pUsedTagsObjectClass; }
	inline const decStringSet &GetUsedTagsObjectClass() const{ return pUsedTagsObjectClass; }
	
	/** Update object class used tags. */
	void UpdateUsedTagsObjectClass();
	
	
	
	/** World properties. */
	inline gdePropertyList &GetWorldProperties(){ return pWorldProperties; }
	inline const gdePropertyList &GetWorldProperties() const{ return pWorldProperties; }
	
	/** Notify listeners world properties changed. */
	void NotifyWorldPropertiesChanged();
	
	/** Notify listeners world property changed. */
	void NotifyWorldPropertyChanged( gdeProperty *property );
	
	/** Notify listeners world property name changed. */
	void NotifyWorldPropertyNameChanged( gdeProperty *property );
	
	
	
	/** Decal properties. */
	inline gdePropertyList &GetDecalProperties(){ return pDecalProperties; }
	inline const gdePropertyList &GetDecalProperties() const{ return pDecalProperties; }
	
	/** Notify listeners world properties changed. */
	void NotifyDecalPropertiesChanged();
	
	/** Notify listeners decal property changed. */
	void NotifyDecalPropertyChanged( gdeProperty *property );
	
	/** Notify listeners decal property name changed. */
	void NotifyDecalPropertyNameChanged( gdeProperty *property );
	
	
	
	/** Auto find path for object classes. */
	inline const decStringList &GetAutoFindPathObjectClasses() const{ return pAutoFindPathObjectClasses; }
	
	/** Set auto find path for object classes. */
	void SetAutoFindPathObjectClasses( const decStringList &list );
	
	/** Notify listeners auto find path for object classes changed. */
	void NotifyAutoFindPathObjectClassesChanged();
	
	/** Auto find path for skins. */
	inline const decStringList &GetAutoFindPathSkins() const{ return pAutoFindPathSkins; }
	
	/** Set auto find path for skins. */
	void SetAutoFindPathSkins( const decStringList &list );
	
	/** Notify listeners auto find path for skins changed. */
	void NotifyAutoFindPathSkinsChanged();
	
	/** Auto find path for skies. */
	inline const decStringList &GetAutoFindPathSkies() const{ return pAutoFindPathSkies; }
	
	/** Set auto find path for skies. */
	void SetAutoFindPathSkies( const decStringList &list );
	
	/** Notify listeners auto find path for skies changed. */
	void NotifyAutoFindPathSkiesChanged();
	
	
	
	/** Selected object type. */
	inline eObjectTypes GetSelectedObjectType() const{ return pSelectedObjectType; }
	
	/** Set selected object type. */
	void SetSelectedObjectType( eObjectTypes objectType );
	
	
	
	/** List of defined identifiers. */
	inline const decStringSet &GetDefinedIDs() const{ return pDefinedIDs; }
	
	/** List of used identifiers. */
	inline const decStringSet &GetUsedIDs() const{ return pUsedIDs; }
	
	/** Update all identifiers. */
	void UpdateDefinedUsedIDs();
	
	
	
	/** Update. */
	void Update( float elapsed );
	
	
	
	/**
	 * Virtual file system to use to load resources with.
	 * 
	 * If game definition is the project game definition the environment engine virtual
	 * file system is returned. Otherwise a local virtual file system is returned with
	 * a single container with the game definition base path.
	 */
	deVirtualFileSystem *GetPreviewVFS();
	/*@}*/
	
	
	
	
	/** \name Object classes */
	/*@{*/
	/** Object classes. */
	inline const gdeObjectClassList &GetObjectClasses() const{ return pObjectClasses; }
	
	/** Add object class. */
	void AddObjectClass( gdeObjectClass *objectClass );
	
	/** Remove object class. */
	void RemoveObjectClass( gdeObjectClass *objectClass );
	
	/** Remove all object classes. */
	void RemoveAllObjectClasses();
	
	/** Find object class in game definition and all base game definitions. */
	const gdeObjectClass * const FindObjectClass( const char *name ) const;
	
	/** Class name list. */
	const decStringSet &GetClassNameList();
	
	
	
	/** Active object class or \em NULL if none is active. */
	inline gdeObjectClass *GetActiveObjectClass() const{ return pActiveObjectClass; }
	
	/** Set active object class or \em NULL if none is active. */
	void SetActiveObjectClass( gdeObjectClass *objectClass );
	
	/** Active object class is set. */
	bool HasActiveObjectClass() const;
	
	
	
	/** Active object class billboard or \em NULL if none is active. */
	inline gdeOCBillboard *GetActiveOCBillboard() const{ return pActiveOCBillboard; }
	
	/** Set active object class billboard or \em NULL if none is active. */
	void SetActiveOCBillboard( gdeOCBillboard *billboard );
	
	/** Active object class billboard is set. */
	bool HasActiveOCBillboard() const;
	
	
	
	/** Active object class camera or \em NULL if none is active. */
	inline gdeOCCamera *GetActiveOCCamera() const{ return pActiveOCCamera; }
	
	/** Set active object class camera or \em NULL if none is active. */
	void SetActiveOCCamera( gdeOCCamera *camera );
	
	/** Active object class camera is set. */
	bool HasActiveOCCamera() const;
	
	
	
	/** Active object class component or \em NULL if none is active. */
	inline gdeOCComponent *GetActiveOCComponent() const{ return pActiveOCComponent; }
	
	/** Set active object class component or \em NULL if none is active. */
	void SetActiveOCComponent( gdeOCComponent *component );
	
	/** Active object class component is set. */
	bool HasActiveOCComponent() const;
	
	
	
	/** Active object class environment map probe or \em NULL if none is active. */
	inline gdeOCEnvMapProbe *GetActiveOCEnvMapProbe() const{ return pActiveOCEnvMapProbe; }
	
	/** Set active object class environment map probe or \em NULL if none is active. */
	void SetActiveOCEnvMapProbe( gdeOCEnvMapProbe *envMapProbe );
	
	/** Active object class environment map probe is set. */
	bool HasActiveOCEnvMapProbe() const;
	
	
	
	/** Active object class light or \em NULL if none is active. */
	inline gdeOCLight *GetActiveOCLight() const{ return pActiveOCLight; }
	
	/** Set active object class light or \em NULL if none is active. */
	void SetActiveOCLight( gdeOCLight *light );
	
	/** Active object class light is set. */
	bool HasActiveOCLight() const;
	
	
	
	/** Active object class navigation blocker or \em NULL if none is active. */
	inline gdeOCNavigationBlocker *GetActiveOCNavigationBlocker() const{ return pActiveOCNavigationBlocker; }
	
	/** Set active object class navigation blocker or \em NULL if none is active. */
	void SetActiveOCNavigationBlocker( gdeOCNavigationBlocker *navblocker );
	
	/** Active object class navigation blocker is set. */
	bool HasActiveOCNavigationBlocker() const;
	
	
	
	/** Active object class navigation space or \em NULL if none is active. */
	inline gdeOCNavigationSpace *GetActiveOCNavigationSpace() const{ return pActiveOCNavigationSpace; }
	
	/** Set active object class navigation space or \em NULL if none is active. */
	void SetActiveOCNavigationSpace( gdeOCNavigationSpace *navspace );
	
	/** Active object class navigation space is set. */
	bool HasActiveOCNavigationSpace() const;
	
	
	
	/** Active object class particle emitter or \em NULL if none is active. */
	inline gdeOCParticleEmitter *GetActiveOCParticleEmitter() const{ return pActiveOCParticleEmitter; }
	
	/** Set active object class particle emitter or \em NULL if none is active. */
	void SetActiveOCParticleEmitter( gdeOCParticleEmitter *emitter );
	
	/** Active object class particle emitter is set. */
	bool HasActiveOCParticleEmitter() const;
	
	
	
	/** Active object class force field or \em NULL if none is active. */
	inline gdeOCForceField *GetActiveOCForceField() const{ return pActiveOCForceField; }
	
	/** Set active object class force field or \em NULL if none is active. */
	void SetActiveOCForceField( gdeOCForceField *field );
	
	/** Active object class force field is set. */
	bool HasActiveOCForceField() const;
	
	
	
	/** Active object class snap point or \em NULL if none is active. */
	inline gdeOCSnapPoint *GetActiveOCSnapPoint() const{ return pActiveOCSnapPoint; }
	
	/** Set active object class snap point or \em NULL if none is active. */
	void SetActiveOCSnapPoint( gdeOCSnapPoint *snappoint );
	
	/** Active object class snap point is set. */
	bool HasActiveOCSnapPoint() const;
	
	
	
	/** Active object class speaker or \em NULL if none is active. */
	inline gdeOCSpeaker *GetActiveOCSpeaker() const{ return pActiveOCSpeaker; }
	
	/** Set active object class speaker or \em NULL if none is active. */
	void SetActiveOCSpeaker( gdeOCSpeaker *speaker );
	
	/** Active object class speaker is set. */
	bool HasActiveOCSpeaker() const;
	
	
	
	/** Active object class world or nullptr if none is active. */
	inline const gdeOCWorld::Ref &GetActiveOCWorld() const{ return pActiveOCWorld; }
	
	/** Set active object class world or nullptr if none is active. */
	void SetActiveOCWorld(gdeOCWorld *world);
	
	/** Active object class world is set. */
	bool HasActiveOCWorld() const;
	/*@}*/
	
	
	
	
	/** \name Particle Emitters */
	/*@{*/
	/** Particle emitters. */
	inline const gdeParticleEmitterList &GetParticleEmitters() const{ return pParticleEmitters; }
	
	/** Add particle emitter. */
	void AddParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** Remove particle emitter. */
	void RemoveParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** Remove all particle emitters. */
	void RemoveAllParticleEmitters();
	
	/** Active particle emitter or \em NULL if none is active. */
	inline gdeParticleEmitter *GetActiveParticleEmitter() const{ return pActiveParticleEmitter; }
	
	/** Set active particle emitter or \em NULL if none is active. */
	void SetActiveParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** Active particle emitter is set. */
	bool HasActiveParticleEmitter() const;
	/*@}*/
	
	
	
	
	/** \name Skins */
	/*@{*/
	/** Skins. */
	inline const gdeSkinList &GetSkins() const{ return pSkins; }
	
	/** Add skin. */
	void AddSkin( gdeSkin *skin );
	
	/** Remove skin. */
	void RemoveSkin( gdeSkin *skin );
	
	/** Remove all skins. */
	void RemoveAllSkins();
	
	/** Active skin or \em NULL if none is active. */
	inline gdeSkin *GetActiveSkin() const{ return pActiveSkin; }
	
	/** Set active skin or \em NULL if none is active. */
	void SetActiveSkin( gdeSkin *skin );
	
	/** Active skin is set. */
	bool HasActiveSkin() const;
	/*@}*/
	
	
	
	
	/** \name Skins */
	/*@{*/
	/** Skins. */
	inline const gdeSkyList &GetSkies() const{ return pSkies; }
	
	/** Add sky. */
	void AddSky( gdeSky *sky );
	
	/** Remove sky. */
	void RemoveSky( gdeSky *sky );
	
	/** Remove all skies. */
	void RemoveAllSkies();
	
	/** Active sky or \em NULL if none is active. */
	inline gdeSky *GetActiveSky() const{ return pActiveSky; }
	
	/** Set active sky or \em NULL if none is active. */
	void SetActiveSky( gdeSky *sky );
	
	/** Active sky is set. */
	bool HasActiveSky() const;
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add listener. */
	void AddListener( gdeGameDefinitionListener *listener );
	
	/** Remove listener. */
	void RemoveListener( gdeGameDefinitionListener *listener );
	
	
	
	/** Notify listeners changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** Notify listeners view properties changed. */
	void NotifyViewChanged();
	
	/** Notify listeners game definition changed. */
	void NotifyGameDefinitionChanged();
	
	/** Notify listeners base or vfs path changed. */
	void NotifyBasePathChanged();
	
	/** Notify listeners base game definitions changed. */
	void NotifyBaseGameDefinitionsChanged();
	
	/** Notify listeners sky changed. */
	void NotifySkyChanged();
	
	/** Notify listeners environment object changed. */
	void NotifyEnvObjectChanged();
	
	/** Notify listeners camera changed. */
	void NotifyCameraChanged();
	
	/** Notify selected object changed. */
	void NotifySelectedObjectChanged();
	
	
	
	/** Notify listeners object class categories changed. */
	void NotifyObjectClassCategoriesChanged();
	
	/** Notify listeners skin categories changed. */
	void NotifySkinCategoriesChanged();
	
	/** Notify listeners sky categories changed. */
	void NotifySkyCategoriesChanged();
	
	/** Notify listeners particle emitters categories changed. */
	void NotifyParticleEmitterCategoriesChanged();
	
	/** Notify listeners active category changed. */
	void NotifyActiveCategoryChanged();
	
	
	
	/** Notify listeners object class used tags changed. */
	void NotifyObjectClassUsedTagsChanged();
	
	/** Notify listeners defined or used IDs changed. */
	void NotifyDefinedUsedIDsChanged();
	
	
	
	/** Notify listeners object class count or order changed. */
	void NotifyObjectClassStructureChanged();
	
	/** Notify listeners object class changed. */
	void NotifyObjectClassChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class name changed. */
	void NotifyObjectClassNameChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class property changed. */
	void NotifyOCPropertyChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Notify listeners object class property name changed. */
	void NotifyOCPropertyNameChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Notify listeners object class properties changed. */
	void NotifyOCPropertiesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class property values changed. */
	void NotifyOCPropertyValuesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class texture property changed. */
	void NotifyOCTexturePropertyChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Notify listeners object class texture property name changed. */
	void NotifyOCTexturePropertyNameChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Notify listeners object class texture properties changed. */
	void NotifyOCTexturePropertiesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class inherit changed. */
	void NotifyOCInheritChanged( gdeObjectClass *objectClass, gdeOCInherit *inherit );
	
	/** Notify listeners object class inherits changed. */
	void NotifyOCInheritsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class billboards changed. */
	void NotifyOCBillboardsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class billboard changed. */
	void NotifyOCBillboardChanged( gdeObjectClass *objectClass, gdeOCBillboard *billboard );
	
	/** Notify listeners object class cameras changed. */
	void NotifyOCCamerasChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class camera changed. */
	void NotifyOCCameraChanged( gdeObjectClass *objectClass, gdeOCCamera *camera );
	
	/** Notify listeners object class components changed. */
	void NotifyOCComponentsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class component changed. */
	void NotifyOCComponentChanged( gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** Notify listeners object class component active texture changed. */
	void NotifyOCComponentActiveTextureChanged( gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** Notify listeners object class component texture changed. */
	void NotifyOCComponentTextureChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** Notify listeners object class component texture name changed. */
	void NotifyOCComponentTextureNameChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** Notify listeners object class component texture properties changed. */
	void NotifyOCComponentTexturePropertiesChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** Notify listeners object class environment map probes changed. */
	void NotifyOCEnvMapProbesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class environment map probe changed. */
	void NotifyOCEnvMapProbeChanged( gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe );
	
	/** Notify listeners object class lights changed. */
	void NotifyOCLightsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class light changed. */
	void NotifyOCLightChanged( gdeObjectClass *objectClass, gdeOCLight *light );
	
	/** Notify listeners object class navigation blockers changed. */
	void NotifyOCNavigationBlockersChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class navigation blocker changed. */
	void NotifyOCNavigationBlockerChanged( gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker );
	
	/** Notify listeners object class navigation spaces changed. */
	void NotifyOCNavigationSpacesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class navigation space changed. */
	void NotifyOCNavigationSpaceChanged( gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace );
	
	/** Notify listeners object class particle emitters changed. */
	void NotifyOCParticleEmittersChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class particle emitter changed. */
	void NotifyOCParticleEmitterChanged( gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter );
	
	/** Notify listeners object class force fields changed. */
	void NotifyOCForceFieldsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class force field changed. */
	void NotifyOCForceFieldChanged( gdeObjectClass *objectClass, gdeOCForceField *field );
	
	/** Notify listeners object class snap points changed. */
	void NotifyOCSnapPointsChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class snap point changed. */
	void NotifyOCSnapPointChanged( gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** Notify listeners object class snap point name changed. */
	void NotifyOCSnapPointNameChanged( gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** Notify listeners object class speakers changed. */
	void NotifyOCSpeakersChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class speaker changed. */
	void NotifyOCSpeakerChanged( gdeObjectClass *objectClass, gdeOCSpeaker *speaker );
	
	/** Notify listeners object class world changed. */
	void NotifyOCWorldsChanged(gdeObjectClass *objectClass);
	
	/** Notify listeners object class world changed. */
	void NotifyOCWorldChanged(gdeObjectClass *objectClass, gdeOCWorld *world);
	
	/** Notify listeners object class textures changed. */
	void NotifyOCTexturesChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners object class texture changed. */
	void NotifyOCTextureChanged( gdeObjectClass *objectClass, gdeOCComponentTexture *texture );
	
	/** Notify listeners object class active texture changed. */
	void NotifyOCActiveTextureChanged( gdeObjectClass *objectClass );
	
	/** Notify listeners active object class changed. */
	void NotifyActiveObjectClassChanged();
	
	/** Notify listeners active object class billboard changed. */
	void NotifyActiveOCBillboardChanged();
	
	/** Notify listeners active object class camera changed. */
	void NotifyActiveOCCameraChanged();
	
	/** Notify listeners active object class component changed. */
	void NotifyActiveOCComponentChanged();
	
	/** Notify listeners active object class environment map probe changed. */
	void NotifyActiveOCEnvMapProbeChanged();
	
	/** Notify listeners active object class light changed. */
	void NotifyActiveOCLightChanged();
	
	/** Notify listeners active object class navigation blocker changed. */
	void NotifyActiveOCNavigationBlockerChanged();
	
	/** Notify listeners active object class navigation space changed. */
	void NotifyActiveOCNavigationSpaceChanged();
	
	/** Notify listeners active object class particle emitter changed. */
	void NotifyActiveOCParticleEmitterChanged();
	
	/** Notify listeners active object class force field changed. */
	void NotifyActiveOCForceFieldChanged();
	
	/** Notify listeners active object class snap point changed. */
	void NotifyActiveOCSnapPointChanged();
	
	/** Notify listeners active object class speaker changed. */
	void NotifyActiveOCSpeakerChanged();
	
	/** Notify listeners active object class world changed. */
	void NotifyActiveOCWorldChanged();
	
	
	
	/** Notify listeners particle emitter count or order changed. */
	void NotifyParticleEmitterStructureChanged();
	
	/** Notify listeners particle emitter changed. */
	void NotifyParticleEmitterChanged( gdeParticleEmitter *particleEmitter );
	
	/** Notify listeners particle emitter name changed. */
	void NotifyParticleEmitterNameChanged( gdeParticleEmitter *particleEmitter );
	
	/** Notify listeners active particle emitter changed. */
	void NotifyActiveParticleEmitterChanged();
	
	
	
	/** Notify listeners skin count or order changed. */
	void NotifySkinStructureChanged();
	
	/** Notify listeners skin changed. */
	void NotifySkinChanged( gdeSkin *skin );
	
	/** Notify listeners skin name changed. */
	void NotifySkinNameChanged( gdeSkin *skin );
	
	/** Notify listeners active skin changed. */
	void NotifyActiveSkinChanged();
	
	
	
	/** Notify listeners sky count or order changed. */
	void NotifySkyStructureChanged();
	
	/** Notify listeners sky changed. */
	void NotifySkyChanged( gdeSky *sky );
	
	/** Notify listeners sky name changed. */
	void NotifySkyNameChanged( gdeSky *sky );
	
	/** Notify listeners active sky changed. */
	void NotifyActiveSkyChanged();
	
	/** Notify listeners sky controller count or order changed. */
	void NotifySkyControllerStructureChanged( gdeSky *sky );
	
	/** Notify listeners sky controller changed. */
	void NotifySkyControllerChanged( gdeSky *sky, gdeSkyController *controller );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
