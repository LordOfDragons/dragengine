/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEWOSOWORLD_H_
#define _IGDEWOSOWORLD_H_

#include "../igdeWObject.h"
#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../utils/igdeBaseXML.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>

class igdeGDCWorld;
class decXmlElementTag;


/**
 * \brief Object wrapper sub world.
 */
class DE_DLL_EXPORT igdeWOSOWorld : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOWorld> Ref;
	
	
	/** \brief Child object texture. */
	class ChildObjectTexture : public deObject{
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<ChildObjectTexture> Ref;
		
		/** \brief Texture name. */
		const decString name;
		
		/** \brief Path to skin to use or empty string. */
		decString pathSkin;
		
		/** \brief Skin to use or null if not used or not loaded. */
		deSkin::Ref skin;
		
		/** \brief Index of texture in skin to use. */
		int texture;
		
		/** \brief Dynamic skin to use or nullptr. */
		deDynamicSkin::Ref dynamicSkin;
		
		/** \brief Texture transformation to use. */
		decTexMatrix2 tcTransform;
		
		/** \brief Has texture transformation to use. */
		bool hasTCTransform;
		
		/** \brief Create child object texture for named texture. */
		ChildObjectTexture(const decString &name);
		
	protected:
		~ChildObjectTexture() override = default;
	};
	
	
	/** \brief Child object. */
	class ChildObject : public deObject{
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<ChildObject> Ref;
		
	private:
		igdeWObject pWrapper;
		decObjectOrderedSet pTextures;
		
	public:
		/** \brief Original object matrix. */
		decDMatrix originalMatrix;
		
		/** \brief Create child object. */
		ChildObject(igdeEnvironment &environment);
		
		/** \brief Object wrapper. */
		inline igdeWObject &GetWrapper(){ return pWrapper; }
		
		/** \brief Count of textures. */
		int GetTextureCount() const;
		
		/** \brief Texture at index. */
		ChildObjectTexture &GetTextureAt(int index) const;
		
		/** \brief Named texture or nullptr. */
		ChildObjectTexture *GetNamedTexture(const char *name) const;
		
		/** \brief Add texture. */
		void AddTexture(const ChildObjectTexture::Ref &texture);
		
	protected:
		~ChildObject() override = default;
	};
	
	
private:
	class LoadXmlWorld : public igdeBaseXML{
	private:
		igdeWOSOWorld &pOwner;
		igdeEnvironment &pEnvironment;
		
	public:
		decObjectOrderedSet objects;
		
		LoadXmlWorld(igdeWOSOWorld &owner);
		void LoadWorld(const decString &path);
		
	private:
		void pReadWorld(const decXmlElementTag &root);
		
		void pReadObject(const decXmlElementTag &root, ChildObject &object);
		
		void pReadObjectTexture(const decXmlElementTag &root,
			ChildObject &object, ChildObjectTexture &texture);
		
		void pReadObjectTextureTransform(const decXmlElementTag &root,
			ChildObject &object, ChildObjectTexture &texture);
	};
	
	
	class LoadObjectResources : public igdeResourceLoaderListener{
	private:
		class Texture : public deObject{
		public:
			const ChildObject::Ref object;
			const ChildObjectTexture::Ref texture;
			Texture(ChildObject &object, ChildObjectTexture &texture);
		};
		
		igdeWOSOWorld *pOwner;
		decStringSet pSkins;
		decObjectOrderedSet pTextures;
		
	public:
		typedef deTObjectReference<LoadObjectResources> Ref;
		LoadObjectResources(igdeWOSOWorld &owner);
		void Drop();
		void LoadTexture(ChildObject &object, ChildObjectTexture &texture);
		void LoadingFinished(const igdeResourceLoaderTask &task, deFileResource *resource) override;
		void LoadingFailed(const igdeResourceLoaderTask &task) override;
		
	protected:
		~LoadObjectResources() override = default;
		void pUpdateTextures(const char *path, deSkin *skin);
	};
	
	
	class ChildAsyncFinished : public igdeWObject::cAsyncLoadFinished{
	private:
		igdeWOSOWorld &pOwner;
		
	public:
		ChildAsyncFinished(igdeWOSOWorld &owner);
		void LoadFinished(igdeWObject &wrapper, bool succeeded) override;
	};
	
	
	const igdeGDCWorld &pGDWorld;
	decString pPathWorld;
	decDVector pPosition;
	decQuaternion pOrientation;
	decObjectOrderedSet pChildObjects;
	ChildAsyncFinished pChildAsyncFinished;
	const LoadObjectResources::Ref pLoadObjectResources;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub world. */
	igdeWOSOWorld(igdeWObject &wrapper, const igdeGDCWorld &gdcWorld, const decString &prefix);
	
	/** \brief Clean up object wrapper sub world. */
	~igdeWOSOWorld() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update parameters. */
	void UpdateParameters() override;
	
	/** \brief All sub components finished loading. */
	void OnAllSubObjectsFinishedLoading() override;
	
	/** \brief Update visibility. */
	void UpdateVisibility() override;
	
	/** \brief Geometry changed. */
	void UpdateGeometry() override;
	
	/** \brief Collision filter changed. */
	void UpdateCollisionFilter() override;
	
	/** \brief Outline skin changed. */
	void OutlineSkinChanged() override;
	
	/** \brief Collider user pointer changed. */
	void ColliderUserPointerChanged() override;
	
	/** \brief Visit. */
	void Visit(igdeWOSOVisitor &visitor) override;
	
	
	/** \brief Count of child objects. */
	int GetChildObjectCount() const;
	
	/** \brief Child object at index. */
	ChildObject &GetChildObjectAt(int index) const;
	
	/** \brief Add child object. */
	void AddChildObject(const ChildObject::Ref &object);
	
	/** \brief Remove all child objects. */
	void RemoveAllChildObjects();
	
	/** \brief Child object async loading finished. */
	void ChildObjectFinishedAsyncLoad(ChildObject &object);
	
	/** \brief Load texture skin. */
	void LoadTextureSkin(ChildObject &object, ChildObjectTexture &texture);
	
	/** \brief Update child component texture. */
	void UpdateChildComponentTexture(ChildObject &object, ChildObjectTexture &texture);
	/*@}*/
	
	
protected:
	void AttachToCollider() override;
	void DetachFromCollider() override;
	
	
private:
	void pUpdateWorld();
	void pDestroyWorld();
	void pLoadWorld(const decString &path);
	void pUpdateChildComponentTextures(ChildObject &object);
};

#endif
