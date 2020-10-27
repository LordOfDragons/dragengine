/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SESKIN_H_
#define _SESKIN_H_

#include "property/sePropertyList.h"
#include "texture/seTextureList.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/resources/light/deLightReference.h>

class seDynamicSkin;
class seDynamicSkinRenderable;
class seProperty;
class sePropertyNode;
class seSkinListener;
class seTexture;

class igdeWSky;
class igdeWObject;
class igdeCamera;

class deAnimator;
class deAnimatorInstance;
class deAnimatorRuleAnimation;
class deAnimatorRuleStateManipulator;
class deComponent;
class deDebugDrawer;
class deLogger;
class deParticleEmitter;
class deSkin;
class deWorld;



/**
 * @brief Editable Skin.
 */
class seSkin : public igdeEditableEntity{
public:
	enum ePreviewMode{
		epmModel,
		epmLight
	};
	
	
	
private:
	deWorld *pEngWorld;
	
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	
	deSkin *pEngSkin;
	deComponent *pEngComponent;
	deAnimator *pEngAnimator;
	deAnimatorInstance *pEngAnimatorInstance;
	deAnimatorRuleAnimation *pEngAnimatorAnim;
	deParticleEmitter *pEngParticleEmitter;
	deLightReference pEngLight;
	
	ePreviewMode pPreviewMode;
	
	decString pModelPath;
	decString pRigPath;
	decString pAnimationPath;
	decString pMoveName;
	float pMoveTime;
	bool pPlayback;
	
	igdeCamera *pCamera;
	
	seTextureList pTextureList;
	seTexture *pActiveTexture;
	
	seDynamicSkin *pDynamicSkin;
	
	bool pDirtySkin;
	bool pDirtySkinAssignment;
	int pRewindTextures;
	bool pEnableSkinUpdate;
	
	decObjectSet pListeners;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin. */
	seSkin( igdeEnvironment *environment );
	/** Cleans up the skin. */
	virtual ~seSkin();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** Retrieves the engine component. */
	inline deComponent *GetEngineComponent() const{ return pEngComponent; }
	/** Retrieves the engine animator. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	/** Retrieves the engine skin. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	/** Retrieves the particle emitter. */
	inline deParticleEmitter *GetEngineParticleEmitter() const{ return pEngParticleEmitter; }
	
	/** \brief Engine light or NULL. */
	inline deLight *GetEngineLight() const{ return pEngLight; }
	
	/** \brief Preview mode. */
	inline ePreviewMode GetPreviewMode() const{ return pPreviewMode; }
	
	/** \brief Set preview mode. */
	void SetPreviewMode( ePreviewMode mode );
	
	/** Retrieves the model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	/** Sets the model path. */
	void SetModelPath( const char *path );
	/** Retrieves the rig path. */
	inline const decString &GetRigPath() const{ return pRigPath; }
	/** Sets the rig path. */
	void SetRigPath( const char *path );
	/** Retrieves the animation path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	/** Sets the animation path. */
	void SetAnimationPath( const char *path );
	/** Retrieves the name of the move to display. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Sets the move name to display. */
	void SetMoveName( const char *moveName );
	/** Retrieves the move time to display. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** Sets the move time to display. */
	void SetMoveTime( float moveTime );
	/** Determines if the animation is played back. */
	inline bool GetPlayback() const{ return pPlayback; }
	/** Sets if the animation is played back. */
	void SetPlayback( bool playback );
	
	/** Retrieves the dynamic skin. */
	inline seDynamicSkin &GetDynamicSkin() const{ return *pDynamicSkin; }
	
	/** \brief Enable skin updating if changed. */
	inline bool GetEnableSkinUpdate() const{ return pEnableSkinUpdate; }
	
	/** \brief Set if skin updating if changed is enabled. */
	void SetEnableSkinUpdate( bool enableSkinUpdate );
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the skin. */
	void Update( float elapsed );
	/** Resets the skin. */
	void Reset();
	
	/** Invalidate skin. */
	void Invalidate();
	/** Assign texture skins to component textures. */
	void AssignTextureSkins();
	
	/**
	 * \brief Rewind all textures to the initial state as if added to the game world.
	 * \details Useful to test how the skin looks like in it's initial state. Required for
	 *          properties relying on elapsed time like videos.
	 */
	void RewindTextures();
	
	/**
	 * \brief Update all resources.
	 * \details Called after file path and directory changed for example after SetFilePath.
	 */
	void UpdateResources();
	/*@}*/
	
	/** @name Textures */
	/*@{*/
	/** Retrieves the texture list read-only. */
	inline const seTextureList &GetTextureList() const{ return pTextureList; }
	/** Adds a new texture. */
	void AddTexture( seTexture *texture );
	/** Removes a texture. */
	void RemoveTexture( seTexture *texture );
	/** Removes all textures. */
	void RemoveAllTextures();
	/** Retrieves the active texture or NULL if none is active. */
	inline seTexture *GetActiveTexture() const{ return pActiveTexture; }
	/** Determines if there is an active texture or not. */
	bool HasActiveTexture() const;
	/** Sets the active texture or NULL if none is active. */
	void SetActiveTexture( seTexture *texture );
	/*@}*/
	
	/** @name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener( seSkinListener *listener );
	/** Removes a listener. */
	void RemoveListener( seSkinListener *listener );
	
	/** Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** Notifies all that a skin parameter changed. */
	void NotifySkinChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notifies all that the view changed. */
	void NotifyViewChanged();
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	
	/** Notifies all that textures have been added or removed. */
	void NotifyTextureStructureChanged();
	/** Notifies all that a texture changed. */
	void NotifyTextureChanged( seTexture *texture );
	/** Notifies all that a texture name changed. */
	void NotifyTextureNameChanged( seTexture *texture );
	/** Active texture changed. */
	void NotifyActiveTextureChanged();
	
	/** Notifies all that properties have been added or removed. */
	void NotifyPropertyStructureChanged( seTexture *texture );
	/** Notifies all that a property changed. */
	void NotifyPropertyChanged( seTexture *texture, seProperty *property );
	/** Active property changed. */
	void NotifyActivePropertyChanged( seTexture *texture );
	
	/** \brief Property node structre changed. */
	void NotifyPropertyNodeStructureChanged( seTexture *texture, seProperty *property );
	
	/** \brief Property node changed. */
	void NotifyPropertyNodeChanged( seTexture *texture, seProperty *property, sePropertyNode *node );
	
	/** \brief Active property node changed. */
	void NotifyPropertyActiveNodeChanged( seTexture *texture, seProperty *property );
	
	/** \brief Property node selection changed. */
	void NotifyPropertyNodeSelectionChanged( seTexture *texture, seProperty *property );
	
	/** \brief Active property node group changed. */
	void NotifyPropertyActiveNodeGroupChanged( seTexture *texture, seProperty *property );
	
	/** \brief Active property node layer changed. */
	void NotifyPropertyActiveNodeLayerChanged( seTexture *texture, seProperty *property );
	
	/** \brief Dynamic skin renderables have been added or removed. */
	void NotifyDynamicSkinRenderableStructureChanged();
	
	/** \brief Dynamic skin renderable changed. */
	void NotifyDynamicSkinRenderableChanged( seDynamicSkinRenderable *renderable );
	
	/** \brief Dynamic skin renderable name changed. */
	void NotifyDynamicSkinRenderableNameChanged( seDynamicSkinRenderable *renderable );
	
	/** \brief Active dynamic skin renderable changed. */
	void NotifyDynamicSkinActiveRenderableChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateLight();
	void pCreateParticleEmitter();
	
	void pUpdateComponent();
	void pUpdateLight();
	void pUpdateAnimator();
	void pUpdateAnimatorMove();
	
	void pUpdateTextureDynamicSkins();
};

#endif
