/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYPATHWIDGET_H_
#define _IGDEMETAPROPERTYPATHWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyPath.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeEditPath.h"
#include "../../../gui/composed/igdeEditPathListener.h"

class deEngine;


/**
 * \brief Path meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyPathWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyPathWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyPath::Listener{
		igdeMetaPropertyPathWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyPathWidget &widget);
		
	protected:
		~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyPath *property, const igdeMetaContext::Ref &context) override;
		void OnBasePathChanged(igdeMetaPropertyPath *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyPath &pPropertyPath;
	PropertyListener::Ref pPropertyListener;
	igdeEditPath::Ref pEditPath;
	igdeEditPathListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyPathWidget(igdeMetaPropertyPath &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyPathWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property path. */
	inline igdeMetaPropertyPath &GetPropertyPath() const{ return pPropertyPath; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(Builder &builder, bool noLabel) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	/** \brief Update base path. */
	void UpdateBasePath();
	
	/** \brief Edit path widget or nullptr. */
	inline const igdeEditPath::Ref &GetEditPath() const{ return pEditPath; }
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	bool IsPropertyValid() const override;
	
	/** \brief Load file and return file content information. */
	decStringDictionary FileContentInformation() const;
	/*@}*/
	
	
protected:
	void OnContextChanged() override;
	
	void pFileInfo(igdeEnvironment &environment, const igdeMetaContext::Ref &context,
		deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoAnimation(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoAnimator(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoFont(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoImage(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoLanguagePack(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	void pFileInfoModel(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoNavigationSpace(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoOcclusionMesh(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoParticleEmitter(igdeEnvironment &environment,deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoRig(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	void pFileInfoSkin(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoSky(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoSynthesizer(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoSound(deEngine &engine, const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoVideo(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoSpeechAnimation(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoCamera(igdeEnvironment &environment, deEngine &engine,
		const decString &path, decStringDictionary &dict) const;
	
	void pFileInfoWorld(deEngine &engine, const decString &path, decStringDictionary &dict) const;
};

#endif
