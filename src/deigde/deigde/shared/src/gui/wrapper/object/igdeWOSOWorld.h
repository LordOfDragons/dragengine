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

#include "igdeWOSubObject.h"
#include "../../../utils/igdeBaseXML.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
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
	
	
private:
	class LoadedObjectTexture : public deObject{
	public:
		typedef deTObjectReference<LoadedObjectTexture> Ref;
		
		const decString name;
		decString pathSkin;
		deSkin::Ref skin;
		int texture;
		deDynamicSkin::Ref dynamicSkin;
		decTexMatrix2 tcTransform;
		
		LoadedObjectTexture(const decString &name);
		
	protected:
		~LoadedObjectTexture() override = default;
	};
	
	class LoadedObject : public deObject{
	public:
		typedef deTObjectReference<LoadedObject> Ref;
		
	private:
		igdeWObject *pWrapper;
		
	public:
		decDMatrix originalMatrix;
		decObjectOrderedSet textures;
		
		LoadedObject(igdeEnvironment &environment);
		inline igdeWObject &Wrapper(){ return *pWrapper; }
		
	protected:
		~LoadedObject() override;
	};
	
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
		
		void pReadObject(const decXmlElementTag &root, LoadedObject &object);
		
		void pReadObjectTexture(const decXmlElementTag &root,
			LoadedObject &object, LoadedObjectTexture &texture);
		
		void pReadObjectTextureTransform(const decXmlElementTag &root,
			LoadedObject &object, LoadedObjectTexture &texture);
	};
	
	
	const igdeGDCWorld &pGDWorld;
	decString pPathWorld;
	decDVector pPosition;
	decQuaternion pOrientation;
	decObjectOrderedSet pObjects;
	
	
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
	
	/** \brief Visit. */
	void Visit(igdeWOSOVisitor &visitor) override;
	
	/** \brief Add object wrapper. */
	void AddObject(const LoadedObject::Ref &object);
	/*@}*/
	
	
protected:
	void AttachToCollider() override;
	void DetachFromCollider() override;
	
	
private:
	void pUpdateWorld();
	void pDestroyWorld();
	void pLoadWorld(const decString &path);
};

#endif
