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

#ifndef _IGDEWINDOW_H_
#define _IGDEWINDOW_H_

#include <stddef.h>

#include "igdeContainer.h"
#include "resources/igdeIcon.h"

#include <dragengine/common/string/decString.h>

class igdeWidget;

/**
 * \brief IGDE UI Window.
 */
class DE_DLL_EXPORT igdeWindow : public igdeContainer{
private:
	decString pTitle;
	igdeIcon::Ref pIcon;
	bool pCanResize;
	decPoint pPosition, pSize;
	bool pEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	igdeWindow(igdeEnvironment &environment, const char *title,
		igdeIcon *icon = nullptr, bool canResize = true);
	
	
	
protected:
	/** \brief Clean up window. */
	virtual ~igdeWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle(const char *title);
	
	/** \brief Icon or nullptr. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or nullptr. */
	void SetIcon(igdeIcon *icon);
	
	/** \brief Window can be resized by user. */
	inline bool GetCanResize() const{ return pCanResize; }
	
	/** \brief Window size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set window size. */
	void SetSize(const decPoint &size);
	
	/** \brief Window is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if window is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Window position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set window position. */
	void SetPosition(const decPoint &position);
	
	/** \brief Raise and activate window. */
	virtual void RaiseAndActivate();
	
	
	
	/**
	 * \brief Add child.
	 * 
	 * Windows can contain only one widget. This is usually a container.
	 */
	virtual void AddChild(igdeWidget *child);
	
	
	
	/**
	 * \brief Request to close window due to clicking on close button.
	 * 
	 * Returns \em true to close window or \em false to keep it open. Default implementation
	 * returns \em true.
	 */
	virtual bool CloseWindow();
	
	/** \brief Close window bypassing CloseWindow(). */
	virtual void Close();
	
	/** \brief Parent window. */
	virtual igdeWindow *GetParentWindow();
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
	/**
	 * \brief Window title changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnTitleChanged();
	
	/**
	 * \brief Window icon changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnIconChanged();
	
	/**
	 * \brief Window size changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnSizeChanged();
	
	/**
	 * \brief Window position changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnPositionChanged();
	
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Raise and activate window. */
	virtual void OnRaiseAndActivate();
	/*@}*/
};

#endif
