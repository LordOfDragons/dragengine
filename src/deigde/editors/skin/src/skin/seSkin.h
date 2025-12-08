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

#ifndef _SESKIN_H_
#define _SESKIN_H_

#include "property/sePropertyList.h"
#include "texture/seTextureList.h"
#include "mapped/seMappedList.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/resources/light/deLight.h>

class seDynamicSkin;
class seDynamicSkinRenderable;
class seProperty;
class sePropertyNode;
class seSkinListener;

class igdeWSky;
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
	typedef deTObjectReference<seSkin> Ref;
	
	
	enum ePreviewMode{
		epmModel,
		epmLight
	};
	
	
	
private:
	deWorld::Ref pEngWorld;
	
	igdeWSky *pSky;
	igdeWObject::Ref pEnvObject;
	
	deSkin::Ref pEngSkin;
	deComponent::Ref pEngComponent;
	deAnimator::Ref pEngAnimator;
	deAnimatorInstance::Ref pEngAnimatorInstance;
	deAnimatorRuleAnimation *pEngAnimatorAnim;
	deParticleEmitter *pEngParticleEmitter;
	deLight::Ref pEngLight;
	
	ePreviewMode pPreviewMode;
	
	decString pModelPath;
	decString pRigPath;
	decString pAnimationPath;
	decString pMoveName;
	float pMoveTime;
	bool pPlayback;
	
	igdeCamera *pCamera;
	
	seMappedList pMappedList;
	seMapped::Ref pActiveMapped;
	
	seTextureList pTextureList;
	seTexture::Ref pActiveTexture;
	
	seDynamicSkin *pDynamicSkin;
	
	bool pDirtySkin;
	bool pDirtySkinAssignment;
	int pRewindTextures;
	bool pEnableSkinUpdate;
	
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin. */
	seSkin(igdeEnvironment *environment);
	/** Cleans up the skin. */
	virtual ~seSkin();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the environment wrapper object. */
	inline const igdeWObject::Ref &GetEnvObject() const{ return pEnvObject; }
	
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
	void SetPreviewMode(ePreviewMode mode);
	
	/** Retrieves the model path. */
	inline const decString &GetModelPath() const{ return pModelPath; }
	/** Sets the model path. */
	void SetModelPath(const char *path);
	/** Retrieves the rig path. */
	inline const decString &GetRigPath() const{ return pRigPath; }
	/** Sets the rig path. */
	void SetRigPath(const char *path);
	/** Retrieves the animation path. */
	inline const decString &GetAnimationPath() const{ return pAnimationPath; }
	/** Sets the animation path. */
	void SetAnimationPath(const char *path);
	/** Retrieves the name of the move to display. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Sets the move name to display. */
	void SetMoveName(const char *moveName);
	/** Retrieves the move time to display. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** Sets the move time to display. */
	void SetMoveTime(float moveTime);
	/** Determines if the animation is played back. */
	inline bool GetPlayback() const{ return pPlayback; }
	/** Sets if the animation is played back. */
	void SetPlayback(bool playback);
	
	/** Retrieves the dynamic skin. */
	inline seDynamicSkin &GetDynamicSkin() const{ return *pDynamicSkin; }
	
	/** \brief Enable skin updating if changed. */
	inline bool GetEnableSkinUpdate() const{ return pEnableSkinUpdate; }
	
	/** \brief Set if skin updating if changed is enabled. */
	void SetEnableSkinUpdate(bool enableSkinUpdate);
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the skin. */
	void Update(float elapsed);
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
	
	
	
	/** \name Mapped */
	/*@{*/
	/** Mapped list. */
	inline const seMappedList &GetMappedList() const{ return pMappedList; }
	
	/** Add mapped. */
	void AddMapped(seMapped::Ref mapped);
	
	/** Remove mapped. */
	void RemoveMapped(seMapped::Ref mapped);
	
	/** Remove all mappeds. */
	void RemoveAllMapped();
	
	/** Active mapped or nullptr. */
	inline seMapped *GetActiveMapped() const{ return pActiveMapped; }
	
	/** Active mapped is present. */
	bool HasActiveMapped() const;
	
	/** Set active mapped or nullptr. */
	void SetActiveMapped(seMapped::Ref mapped);
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** Retrieves the texture list read-only. */
	inline const seTextureList &GetTextureList() const{ return pTextureList; }
	/** Adds a new texture. */
	void AddTexture(seTexture::Ref texture);
	/** Removes a texture. */
	void RemoveTexture(seTexture::Ref texture);
	/** Removes all textures. */
	void RemoveAllTextures();
	/** Retrieves the active texture or NULL if none is active. */
	inline seTexture *GetActiveTexture() const{ return pActiveTexture; }
	/** Determines if there is an active texture or not. */
	bool HasActiveTexture() const;
	/** Sets the active texture or NULL if none is active. */
	void SetActiveTexture(seTexture::Ref texture);
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener(seSkinListener *listener);
	/** Removes a listener. */
	void RemoveListener(seSkinListener *listener);
	
	/** Notify all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notify all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** Notify all that a skin parameter changed. */
	void NotifySkinChanged();
	/** Notify all that the sky changed. */
	void NotifySkyChanged();
	/** Notify all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notify all that the view changed. */
	void NotifyViewChanged();
	/** Notify all that the camera changed. */
	void NotifyCameraChanged();
	
	
	
	/** Notify all mapped have been added or removed. */
	void NotifyMappedStructureChanged();
	
	/** Notify all mapped changed. */
	void NotifyMappedChanged(seMapped::Ref mapped);
	
	/** Notify all mapped name changed. */
	void NotifyMappedNameChanged(seMapped::Ref mapped);
	
	/** Active mapped changed. */
	void NotifyActiveMappedChanged();
	
	
	
	/** Notify all that textures have been added or removed. */
	void NotifyTextureStructureChanged();
	/** Notify all that a texture changed. */
	void NotifyTextureChanged(seTexture::Ref texture);
	/** Notify all that a texture name changed. */
	void NotifyTextureNameChanged(seTexture::Ref texture);
	/** Active texture changed. */
	void NotifyActiveTextureChanged();
	
	
	/** Notify all that properties have been added or removed. */
	void NotifyPropertyStructureChanged(seTexture::Ref texture);
	/** Notify all that a property changed. */
	void NotifyPropertyChanged(seTexture::Ref texture, seProperty *property);
	/** Active property changed. */
	void NotifyActivePropertyChanged(seTexture::Ref texture);
	
	/** \brief Property node structre changed. */
	void NotifyPropertyNodeStructureChanged(seTexture::Ref texture, seProperty *property);
	
	/** \brief Property node changed. */
	void NotifyPropertyNodeChanged(seTexture::Ref texture, seProperty *property, sePropertyNode *node);
	
	/** \brief Active property node changed. */
	void NotifyPropertyActiveNodeChanged(seTexture::Ref texture, seProperty *property);
	
	/** \brief Property node selection changed. */
	void NotifyPropertyNodeSelectionChanged(seTexture::Ref texture, seProperty *property);
	
	/** \brief Active property node group changed. */
	void NotifyPropertyActiveNodeGroupChanged(seTexture::Ref texture, seProperty *property);
	
	/** \brief Active property node layer changed. */
	void NotifyPropertyActiveNodeLayerChanged(seTexture::Ref texture, seProperty *property);
	
	/** \brief Dynamic skin renderables have been added or removed. */
	void NotifyDynamicSkinRenderableStructureChanged();
	
	/** \brief Dynamic skin renderable changed. */
	void NotifyDynamicSkinRenderableChanged(seDynamicSkinRenderable *renderable);
	
	/** \brief Dynamic skin renderable name changed. */
	void NotifyDynamicSkinRenderableNameChanged(seDynamicSkinRenderable *renderable);
	
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
