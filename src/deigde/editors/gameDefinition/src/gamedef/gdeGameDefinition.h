/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEGAMEDEFINITION_H_
#define _GDEGAMEDEFINITION_H_

#include "category/gdeCategoryList.h"
#include "objectClass/gdeObjectClassList.h"
#include "particleemitter/gdeParticleEmitterList.h"
#include "property/gdePropertyList.h"
#include "skin/gdeSkinList.h"
#include "sky/gdeSkyList.h"

#include <deigde/editableentity/igdeEditableEntity.h>

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
class gdeSkyController;

class igdeWSky;
class igdeWObject;
class igdeCamera;

class deWorld;
class deVirtualFileSystem;



/**
 * \brief Game definition.
 */
class gdeGameDefinition : public igdeEditableEntity{
public:
	/** \brief Selectec object type. */
	enum eObjectTypes{
		/** \brief No selection. */
		eotNoSelection,
		
		/** \brief Category object class. */
		eotCategoryObjectClass,
		
		/** \brief Category skin. */
		eotCategorySkin,
		
		/** \brief Category sky. */
		eotCategorySky,
		
		/** \brief Category particle emitter. */
		eotCategoryParticleEmitter,
		
		/** \brief Object class. */
		eotObjectClass,
		
		/** \brief Object class billboard. */
		eotOCBillboard,
		
		/** \brief Object class camera. */
		eotOCCamera,
		
		/** \brief Object class component. */
		eotOCComponent,
		
		/** \brief Object class environment map probe. */
		eotOCEnvMapProbe,
		
		/** \brief Object class light. */
		eotOCLight,
		
		/** \brief Object class navigation blocker. */
		eotOCNavigationBlocker,
		
		/** \brief Object class navigation space. */
		eotOCNavigationSpace,
		
		/** \brief Object class particle emitter. */
		eotOCParticleEmitter,
		
		/** \brief Object class force field. */
		eotOCForceField,
		
		/** \brief Object class snap point. */
		eotOCSnapPoint,
		
		/** \brief Object class speaker. */
		eotOCSpeaker,
		
		/** \brief Skin. */
		eotSkin,
		
		/** \brief Sky. */
		eotSky,
		
		/** \brief Particle emitter. */
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
	decStringSet pClassNameList;
	
	gdeParticleEmitterList pParticleEmitters;
	gdeParticleEmitter *pActiveParticleEmitter;
	
	gdeSkinList pSkins;
	gdeSkin *pActiveSkin;
	
	gdeSkyList pSkies;
	gdeSky *pActiveSky;
	
	gdePropertyList pWorldProperties;
	gdePropertyList pDecalProperties;
	
	decStringSet pAutoFindPathObjectClasses;
	decStringSet pAutoFindPathSkins;
	decStringSet pAutoFindPathSkies;
	
	eObjectTypes pSelectedObjectType;
	
	decStringSet pDefinedIDs;
	decStringSet pUsedIDs;
	
	decObjectSet pListeners;
	
	deVirtualFileSystem *pVFS;
	deVirtualFileSystem *pPreviewVFS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition. */
	gdeGameDefinition( igdeEnvironment *environment );
	
	/** \brief Clean up speech animation. */
	virtual ~gdeGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline deWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief View ratio used to properly align camera on preview objects. */
	inline float GetViewRatio() const{ return pViewRatio; }
	
	/** \brief Set view ratio used to properly align camera on preview objects. */
	void SetViewRatio( float viewRatio );
	
	/** \brief Sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	
	/** \brief Environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** \brief Is project game definition. */
	inline bool GetIsProjectGameDef() const{ return pIsProjectGameDef; }
	
	/** \brief Set if this is project game definition. */
	void SetIsProjectGameDef( bool isProjectGameDef );
	
	
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Get base path. */
	inline const decString &GetBasePath() const{ return pBasePath; }
	
	/** \brief Set base path. */
	void SetBasePath( const char *path );
	
	/** \brief Get vfs path. */
	inline const decString &GetVFSPath() const{ return pVFSPath; }
	
	/** \brief Set vfs path. */
	void SetVFSPath( const char *path );
	
	/** \brief Script module this game definition is compatible with. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set script module this game definition is compatible with. */
	void SetScriptModule( const char *identifier );
	
	
	
	/** \brief Base game definition id list. */
	inline const decStringList &GetBaseGameDefinitionIDList() const{ return pBaseGameDefinitionIDList; }
	
	/** \brief Set base game definition id list. */
	void SetBaseGameDefinitionIDList( const decStringList &ids );
	
	/** \brief Update list of base game definitions. */
	void UpdateBaseGameDefinitions( gdeLoadSaveSystem &loadSaveSystem );
	
	/** \brief Number of base game definitions. */
	int GetBaseGameDefinitionCount() const;
	
	/** \brief Get base game definition at index. */
	const gdeGameDefinition &GetBaseGameDefinitionAt( int index ) const;
	
	
	
	/** \brief Path to object class. */
	inline const decString &GetDefaultObjectClass() const{ return pDefaultObjectClass; }
	
	/** \brief Set path to object class. */
	void SetDefaultObjectClass( const char *objectClass );
	
	/** \brief Path to skin. */
	inline const decString &GetDefaultSkin() const{ return pDefaultSkin; }
	
	/** \brief Set path to skin. */
	void SetDefaultSkin( const char *skin );
	
	/** \brief Path to sky. */
	inline const decString &GetDefaultSky() const{ return pDefaultSky; }
	
	/** \brief Set path to sky. */
	void SetDefaultSky( const char *sky );
	
	
	
	/** \brief Object class categories. */
	inline gdeCategoryList &GetCategoriesObjectClass(){ return pCategoriesObjectClass; }
	inline const gdeCategoryList &GetCategoriesObjectClass() const{ return pCategoriesObjectClass; }
	
	/** \brief Find object class category with path or \em NULL if absent. */
	const gdeCategory *FindCategoryObjectClass( const char *path ) const;
	
	/** \brief Object classes category names. */
	const decStringSet &GetObjectClassCategoryNameList();
	
	/** \brief Skin categories. */
	inline gdeCategoryList &GetCategoriesSkin(){ return pCategoriesSkin; }
	inline const gdeCategoryList &GetCategoriesSkin() const{ return pCategoriesSkin; }
	
	/** \brief Sky categories. */
	inline gdeCategoryList &GetCategoriesSky(){ return pCategoriesSky; }
	inline const gdeCategoryList &GetCategoriesSky() const{ return pCategoriesSky; }
	
	/** \brief Particle emitter categories. */
	inline gdeCategoryList &GetCategoriesParticleEmitter(){ return pCategoriesParticleEmitter; }
	inline const gdeCategoryList &GetCategoriesParticleEmitter() const{ return pCategoriesParticleEmitter; }
	
	/** \brief Active category or \em NULL if not set. */
	inline gdeCategory *GetActiveCategory() const{ return pActiveCategory; }
	
	/** \brief Set active category or \em NULL if not set. */
	void SetActiveCategory( gdeCategory *category );
	
	
	
	/** \brief Object class used tags. */
	inline decStringSet &GetUsedTagsObjectClass(){ return pUsedTagsObjectClass; }
	inline const decStringSet &GetUsedTagsObjectClass() const{ return pUsedTagsObjectClass; }
	
	/** \brief Update object class used tags. */
	void UpdateUsedTagsObjectClass();
	
	
	
	/** \brief World properties. */
	inline gdePropertyList &GetWorldProperties(){ return pWorldProperties; }
	inline const gdePropertyList &GetWorldProperties() const{ return pWorldProperties; }
	
	/** \brief Notify listeners world properties changed. */
	void NotifyWorldPropertiesChanged();
	
	/** \brief Notify listeners world property changed. */
	void NotifyWorldPropertyChanged( gdeProperty *property );
	
	/** \brief Notify listeners world property name changed. */
	void NotifyWorldPropertyNameChanged( gdeProperty *property );
	
	
	
	/** \brief Decal properties. */
	inline gdePropertyList &GetDecalProperties(){ return pDecalProperties; }
	inline const gdePropertyList &GetDecalProperties() const{ return pDecalProperties; }
	
	/** \brief Notify listeners world properties changed. */
	void NotifyDecalPropertiesChanged();
	
	/** \brief Notify listeners decal property changed. */
	void NotifyDecalPropertyChanged( gdeProperty *property );
	
	/** \brief Notify listeners decal property name changed. */
	void NotifyDecalPropertyNameChanged( gdeProperty *property );
	
	
	
	/** \brief Auto find path for object classes. */
	inline const decStringSet &GetAutoFindPathObjectClasses() const{ return pAutoFindPathObjectClasses; }
	
	/** \brief Set auto find path for object classes. */
	void SetAutoFindPathObjectClasses( const decStringSet &list );
	
	/** \brief Notify listeners auto find path for object classes changed. */
	void NotifyAutoFindPathObjectClassesChanged();
	
	/** \brief Auto find path for skins. */
	inline const decStringSet &GetAutoFindPathSkins() const{ return pAutoFindPathSkins; }
	
	/** \brief Set auto find path for skins. */
	void SetAutoFindPathSkins( const decStringSet &list );
	
	/** \brief Notify listeners auto find path for skins changed. */
	void NotifyAutoFindPathSkinsChanged();
	
	/** \brief Auto find path for skies. */
	inline const decStringSet &GetAutoFindPathSkies() const{ return pAutoFindPathSkies; }
	
	/** \brief Set auto find path for skies. */
	void SetAutoFindPathSkies( const decStringSet &list );
	
	/** \brief Notify listeners auto find path for skies changed. */
	void NotifyAutoFindPathSkiesChanged();
	
	
	
	/** \brief Selected object type. */
	inline eObjectTypes GetSelectedObjectType() const{ return pSelectedObjectType; }
	
	/** \brief Set selected object type. */
	void SetSelectedObjectType( eObjectTypes objectType );
	
	
	
	/** \brief List of defined identifiers. */
	inline const decStringSet &GetDefinedIDs() const{ return pDefinedIDs; }
	
	/** \brief List of used identifiers. */
	inline const decStringSet &GetUsedIDs() const{ return pUsedIDs; }
	
	/** \brief Update all identifiers. */
	void UpdateDefinedUsedIDs();
	
	
	
	/** \brief Update. */
	void Update( float elapsed );
	
	
	
	/**
	 * \brief Virtual file system to use to load resources with.
	 * 
	 * If game definition is the project game definition the environment engine virtual
	 * file system is returned. Otherwise a local virtual file system is returned with
	 * a single container with the game definition base path.
	 */
	deVirtualFileSystem *GetPreviewVFS();
	/*@}*/
	
	
	
	
	/** \name Object classes */
	/*@{*/
	/** \brief Object classes. */
	inline const gdeObjectClassList &GetObjectClasses() const{ return pObjectClasses; }
	
	/** \brief Add object class. */
	void AddObjectClass( gdeObjectClass *objectClass );
	
	/** \brief Remove object class. */
	void RemoveObjectClass( gdeObjectClass *objectClass );
	
	/** \brief Remove all object classes. */
	void RemoveAllObjectClasses();
	
	/** \brief Find object class in game definition and all base game definitions. */
	const gdeObjectClass * const FindObjectClass( const char *name ) const;
	
	/** \brief Class name list. */
	const decStringSet &GetClassNameList();
	
	
	
	/** \brief Active object class or \em NULL if none is active. */
	inline gdeObjectClass *GetActiveObjectClass() const{ return pActiveObjectClass; }
	
	/** \brief Set active object class or \em NULL if none is active. */
	void SetActiveObjectClass( gdeObjectClass *objectClass );
	
	/** \brief Active object class is set. */
	bool HasActiveObjectClass() const;
	
	
	
	/** \brief Active object class billboard or \em NULL if none is active. */
	inline gdeOCBillboard *GetActiveOCBillboard() const{ return pActiveOCBillboard; }
	
	/** \brief Set active object class billboard or \em NULL if none is active. */
	void SetActiveOCBillboard( gdeOCBillboard *billboard );
	
	/** \brief Active object class billboard is set. */
	bool HasActiveOCBillboard() const;
	
	
	
	/** \brief Active object class camera or \em NULL if none is active. */
	inline gdeOCCamera *GetActiveOCCamera() const{ return pActiveOCCamera; }
	
	/** \brief Set active object class camera or \em NULL if none is active. */
	void SetActiveOCCamera( gdeOCCamera *camera );
	
	/** \brief Active object class camera is set. */
	bool HasActiveOCCamera() const;
	
	
	
	/** \brief Active object class component or \em NULL if none is active. */
	inline gdeOCComponent *GetActiveOCComponent() const{ return pActiveOCComponent; }
	
	/** \brief Set active object class component or \em NULL if none is active. */
	void SetActiveOCComponent( gdeOCComponent *component );
	
	/** \brief Active object class component is set. */
	bool HasActiveOCComponent() const;
	
	
	
	/** \brief Active object class environment map probe or \em NULL if none is active. */
	inline gdeOCEnvMapProbe *GetActiveOCEnvMapProbe() const{ return pActiveOCEnvMapProbe; }
	
	/** \brief Set active object class environment map probe or \em NULL if none is active. */
	void SetActiveOCEnvMapProbe( gdeOCEnvMapProbe *envMapProbe );
	
	/** \brief Active object class environment map probe is set. */
	bool HasActiveOCEnvMapProbe() const;
	
	
	
	/** \brief Active object class light or \em NULL if none is active. */
	inline gdeOCLight *GetActiveOCLight() const{ return pActiveOCLight; }
	
	/** \brief Set active object class light or \em NULL if none is active. */
	void SetActiveOCLight( gdeOCLight *light );
	
	/** \brief Active object class light is set. */
	bool HasActiveOCLight() const;
	
	
	
	/** \brief Active object class navigation blocker or \em NULL if none is active. */
	inline gdeOCNavigationBlocker *GetActiveOCNavigationBlocker() const{ return pActiveOCNavigationBlocker; }
	
	/** \brief Set active object class navigation blocker or \em NULL if none is active. */
	void SetActiveOCNavigationBlocker( gdeOCNavigationBlocker *navblocker );
	
	/** \brief Active object class navigation blocker is set. */
	bool HasActiveOCNavigationBlocker() const;
	
	
	
	/** \brief Active object class navigation space or \em NULL if none is active. */
	inline gdeOCNavigationSpace *GetActiveOCNavigationSpace() const{ return pActiveOCNavigationSpace; }
	
	/** \brief Set active object class navigation space or \em NULL if none is active. */
	void SetActiveOCNavigationSpace( gdeOCNavigationSpace *navspace );
	
	/** \brief Active object class navigation space is set. */
	bool HasActiveOCNavigationSpace() const;
	
	
	
	/** \brief Active object class particle emitter or \em NULL if none is active. */
	inline gdeOCParticleEmitter *GetActiveOCParticleEmitter() const{ return pActiveOCParticleEmitter; }
	
	/** \brief Set active object class particle emitter or \em NULL if none is active. */
	void SetActiveOCParticleEmitter( gdeOCParticleEmitter *emitter );
	
	/** \brief Active object class particle emitter is set. */
	bool HasActiveOCParticleEmitter() const;
	
	
	
	/** \brief Active object class force field or \em NULL if none is active. */
	inline gdeOCForceField *GetActiveOCForceField() const{ return pActiveOCForceField; }
	
	/** \brief Set active object class force field or \em NULL if none is active. */
	void SetActiveOCForceField( gdeOCForceField *field );
	
	/** \brief Active object class force field is set. */
	bool HasActiveOCForceField() const;
	
	
	
	/** \brief Active object class snap point or \em NULL if none is active. */
	inline gdeOCSnapPoint *GetActiveOCSnapPoint() const{ return pActiveOCSnapPoint; }
	
	/** \brief Set active object class snap point or \em NULL if none is active. */
	void SetActiveOCSnapPoint( gdeOCSnapPoint *snappoint );
	
	/** \brief Active object class snap point is set. */
	bool HasActiveOCSnapPoint() const;
	
	
	
	/** \brief Active object class speaker or \em NULL if none is active. */
	inline gdeOCSpeaker *GetActiveOCSpeaker() const{ return pActiveOCSpeaker; }
	
	/** \brief Set active object class speaker or \em NULL if none is active. */
	void SetActiveOCSpeaker( gdeOCSpeaker *speaker );
	
	/** \brief Active object class speaker is set. */
	bool HasActiveOCSpeaker() const;
	/*@}*/
	
	
	
	
	/** \name Particle Emitters */
	/*@{*/
	/** \brief Particle emitters. */
	inline const gdeParticleEmitterList &GetParticleEmitters() const{ return pParticleEmitters; }
	
	/** \brief Add particle emitter. */
	void AddParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** \brief Remove particle emitter. */
	void RemoveParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** \brief Remove all particle emitters. */
	void RemoveAllParticleEmitters();
	
	/** \brief Active particle emitter or \em NULL if none is active. */
	inline gdeParticleEmitter *GetActiveParticleEmitter() const{ return pActiveParticleEmitter; }
	
	/** \brief Set active particle emitter or \em NULL if none is active. */
	void SetActiveParticleEmitter( gdeParticleEmitter *particleEmitter );
	
	/** \brief Active particle emitter is set. */
	bool HasActiveParticleEmitter() const;
	/*@}*/
	
	
	
	
	/** \name Skins */
	/*@{*/
	/** \brief Skins. */
	inline const gdeSkinList &GetSkins() const{ return pSkins; }
	
	/** \brief Add skin. */
	void AddSkin( gdeSkin *skin );
	
	/** \brief Remove skin. */
	void RemoveSkin( gdeSkin *skin );
	
	/** \brief Remove all skins. */
	void RemoveAllSkins();
	
	/** \brief Active skin or \em NULL if none is active. */
	inline gdeSkin *GetActiveSkin() const{ return pActiveSkin; }
	
	/** \brief Set active skin or \em NULL if none is active. */
	void SetActiveSkin( gdeSkin *skin );
	
	/** \brief Active skin is set. */
	bool HasActiveSkin() const;
	/*@}*/
	
	
	
	
	/** \name Skins */
	/*@{*/
	/** \brief Skins. */
	inline const gdeSkyList &GetSkies() const{ return pSkies; }
	
	/** \brief Add sky. */
	void AddSky( gdeSky *sky );
	
	/** \brief Remove sky. */
	void RemoveSky( gdeSky *sky );
	
	/** \brief Remove all skies. */
	void RemoveAllSkies();
	
	/** \brief Active sky or \em NULL if none is active. */
	inline gdeSky *GetActiveSky() const{ return pActiveSky; }
	
	/** \brief Set active sky or \em NULL if none is active. */
	void SetActiveSky( gdeSky *sky );
	
	/** \brief Active sky is set. */
	bool HasActiveSky() const;
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** \brief Add listener. */
	void AddListener( gdeGameDefinitionListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( gdeGameDefinitionListener *listener );
	
	
	
	/** \brief Notify listeners changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify listeners view properties changed. */
	void NotifyViewChanged();
	
	/** \brief Notify listeners game definition changed. */
	void NotifyGameDefinitionChanged();
	
	/** \brief Notify listeners base or vfs path changed. */
	void NotifyBasePathChanged();
	
	/** \brief Notify listeners base game definitions changed. */
	void NotifyBaseGameDefinitionsChanged();
	
	/** \brief Notify listeners sky changed. */
	void NotifySkyChanged();
	
	/** \brief Notify listeners environment object changed. */
	void NotifyEnvObjectChanged();
	
	/** \brief Notify listeners camera changed. */
	void NotifyCameraChanged();
	
	/** \brief Notify selected object changed. */
	void NotifySelectedObjectChanged();
	
	
	
	/** \brief Notify listeners object class categories changed. */
	void NotifyObjectClassCategoriesChanged();
	
	/** \brief Notify listeners skin categories changed. */
	void NotifySkinCategoriesChanged();
	
	/** \brief Notify listeners sky categories changed. */
	void NotifySkyCategoriesChanged();
	
	/** \brief Notify listeners particle emitters categories changed. */
	void NotifyParticleEmitterCategoriesChanged();
	
	/** \brief Notify listeners active category changed. */
	void NotifyActiveCategoryChanged();
	
	
	
	/** \brief Notify listeners object class used tags changed. */
	void NotifyObjectClassUsedTagsChanged();
	
	/** \brief Notify listeners defined or used IDs changed. */
	void NotifyDefinedUsedIDsChanged();
	
	
	
	/** \brief Notify listeners object class count or order changed. */
	void NotifyObjectClassStructureChanged();
	
	/** \brief Notify listeners object class changed. */
	void NotifyObjectClassChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class name changed. */
	void NotifyObjectClassNameChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class property changed. */
	void NotifyOCPropertyChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Notify listeners object class property name changed. */
	void NotifyOCPropertyNameChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Notify listeners object class properties changed. */
	void NotifyOCPropertiesChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class property values changed. */
	void NotifyOCPropertyValuesChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class texture property changed. */
	void NotifyOCTexturePropertyChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Notify listeners object class texture property name changed. */
	void NotifyOCTexturePropertyNameChanged( gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Notify listeners object class texture properties changed. */
	void NotifyOCTexturePropertiesChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class inherit changed. */
	void NotifyOCInheritChanged( gdeObjectClass *objectClass, gdeOCInherit *inherit );
	
	/** \brief Notify listeners object class inherits changed. */
	void NotifyOCInheritsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class billboards changed. */
	void NotifyOCBillboardsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class billboard changed. */
	void NotifyOCBillboardChanged( gdeObjectClass *objectClass, gdeOCBillboard *billboard );
	
	/** \brief Notify listeners object class cameras changed. */
	void NotifyOCCamerasChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class camera changed. */
	void NotifyOCCameraChanged( gdeObjectClass *objectClass, gdeOCCamera *camera );
	
	/** \brief Notify listeners object class components changed. */
	void NotifyOCComponentsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class component changed. */
	void NotifyOCComponentChanged( gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Notify listeners object class component active texture changed. */
	void NotifyOCComponentActiveTextureChanged( gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Notify listeners object class component texture changed. */
	void NotifyOCComponentTextureChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Notify listeners object class component texture name changed. */
	void NotifyOCComponentTextureNameChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Notify listeners object class component texture properties changed. */
	void NotifyOCComponentTexturePropertiesChanged( gdeObjectClass *objectClass,
		gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Notify listeners object class environment map probes changed. */
	void NotifyOCEnvMapProbesChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class environment map probe changed. */
	void NotifyOCEnvMapProbeChanged( gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe );
	
	/** \brief Notify listeners object class lights changed. */
	void NotifyOCLightsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class light changed. */
	void NotifyOCLightChanged( gdeObjectClass *objectClass, gdeOCLight *light );
	
	/** \brief Notify listeners object class navigation blockers changed. */
	void NotifyOCNavigationBlockersChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class navigation blocker changed. */
	void NotifyOCNavigationBlockerChanged( gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker );
	
	/** \brief Notify listeners object class navigation spaces changed. */
	void NotifyOCNavigationSpacesChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class navigation space changed. */
	void NotifyOCNavigationSpaceChanged( gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace );
	
	/** \brief Notify listeners object class particle emitters changed. */
	void NotifyOCParticleEmittersChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class particle emitter changed. */
	void NotifyOCParticleEmitterChanged( gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter );
	
	/** \brief Notify listeners object class force fields changed. */
	void NotifyOCForceFieldsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class force field changed. */
	void NotifyOCForceFieldChanged( gdeObjectClass *objectClass, gdeOCForceField *field );
	
	/** \brief Notify listeners object class snap points changed. */
	void NotifyOCSnapPointsChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class snap point changed. */
	void NotifyOCSnapPointChanged( gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** \brief Notify listeners object class snap point name changed. */
	void NotifyOCSnapPointNameChanged( gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** \brief Notify listeners object class speakers changed. */
	void NotifyOCSpeakersChanged( gdeObjectClass *objectClass );
	
	/** \brief Notify listeners object class speaker changed. */
	void NotifyOCSpeakerChanged( gdeObjectClass *objectClass, gdeOCSpeaker *speaker );
	
	/** \brief Notify listeners active object class changed. */
	void NotifyActiveObjectClassChanged();
	
	/** \brief Notify listeners active object class billboard changed. */
	void NotifyActiveOCBillboardChanged();
	
	/** \brief Notify listeners active object class camera changed. */
	void NotifyActiveOCCameraChanged();
	
	/** \brief Notify listeners active object class component changed. */
	void NotifyActiveOCComponentChanged();
	
	/** \brief Notify listeners active object class environment map probe changed. */
	void NotifyActiveOCEnvMapProbeChanged();
	
	/** \brief Notify listeners active object class light changed. */
	void NotifyActiveOCLightChanged();
	
	/** \brief Notify listeners active object class navigation blocker changed. */
	void NotifyActiveOCNavigationBlockerChanged();
	
	/** \brief Notify listeners active object class navigation space changed. */
	void NotifyActiveOCNavigationSpaceChanged();
	
	/** \brief Notify listeners active object class particle emitter changed. */
	void NotifyActiveOCParticleEmitterChanged();
	
	/** \brief Notify listeners active object class force field changed. */
	void NotifyActiveOCForceFieldChanged();
	
	/** \brief Notify listeners active object class snap point changed. */
	void NotifyActiveOCSnapPointChanged();
	
	/** \brief Notify listeners active object class speaker changed. */
	void NotifyActiveOCSpeakerChanged();
	
	
	
	/** \brief Notify listeners particle emitter count or order changed. */
	void NotifyParticleEmitterStructureChanged();
	
	/** \brief Notify listeners particle emitter changed. */
	void NotifyParticleEmitterChanged( gdeParticleEmitter *particleEmitter );
	
	/** \brief Notify listeners particle emitter name changed. */
	void NotifyParticleEmitterNameChanged( gdeParticleEmitter *particleEmitter );
	
	/** \brief Notify listeners active particle emitter changed. */
	void NotifyActiveParticleEmitterChanged();
	
	
	
	/** \brief Notify listeners skin count or order changed. */
	void NotifySkinStructureChanged();
	
	/** \brief Notify listeners skin changed. */
	void NotifySkinChanged( gdeSkin *skin );
	
	/** \brief Notify listeners skin name changed. */
	void NotifySkinNameChanged( gdeSkin *skin );
	
	/** \brief Notify listeners active skin changed. */
	void NotifyActiveSkinChanged();
	
	
	
	/** \brief Notify listeners sky count or order changed. */
	void NotifySkyStructureChanged();
	
	/** \brief Notify listeners sky changed. */
	void NotifySkyChanged( gdeSky *sky );
	
	/** \brief Notify listeners sky name changed. */
	void NotifySkyNameChanged( gdeSky *sky );
	
	/** \brief Notify listeners active sky changed. */
	void NotifyActiveSkyChanged();
	
	/** \brief Notify listeners sky controller count or order changed. */
	void NotifySkyControllerStructureChanged( gdeSky *sky );
	
	/** \brief Notify listeners sky controller changed. */
	void NotifySkyControllerChanged( gdeSky *sky, gdeSkyController *controller );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
