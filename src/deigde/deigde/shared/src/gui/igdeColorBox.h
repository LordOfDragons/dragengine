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

#ifndef _IGDECOLORBOX_H_
#define _IGDECOLORBOX_H_

#include "igdeWidget.h"
#include "event/igdeAction.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class igdeColorBoxListener;


/**
 * \brief IGDE UI Box with color showing a color picker dialog if clicked.
 */
class DE_DLL_EXPORT igdeColorBox : public igdeWidget{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeColorBox> Ref;
	
	
	class DE_DLL_EXPORT cActionCopy : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionCopy> Ref;
		cActionCopy(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionCopyHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionCopyHex> Ref;
		cActionCopyHex(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionPaste : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionPaste> Ref;
		cActionPaste(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionPasteHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionPasteHex> Ref;
		cActionPasteHex(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditValues : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionEditValues> Ref;
		cActionEditValues(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditHex : public igdeAction{
		igdeColorBox &pColorBox;
	public:
		typedef deTObjectReference<cActionEditHex> Ref;
		cActionEditHex(igdeColorBox &colorBox);
		virtual void OnAction();
	};
	
	
	
private:
	decString pDescription;
	bool pEnabled;
	decColor pColor;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create color box. */
	igdeColorBox(igdeEnvironment &environment);
	
	/** \brief Create color box. */
	igdeColorBox(igdeEnvironment &environment, const char *description);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeColorBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief ColorBox is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if color box is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor(const decColor &color);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeColorBoxListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeColorBoxListener *listener);
	
	/** \brief Notify listeners color changed. */
	virtual void NotifyColorChanged();
	
	/** \brief Show context menu at position. */
	virtual void ShowContextMenu(const decPoint &position);
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Color changed. */
	virtual void OnColorChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	/*@}*/
};

#endif
