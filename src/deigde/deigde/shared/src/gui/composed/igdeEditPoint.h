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

#ifndef _IGDEEDITPOINT_H_
#define _IGDEEDITPOINT_H_

#include "../igdeTextField.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditPointListener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Point.
 * 
 * Composed widget to edit decPoint.
 */
class DE_DLL_EXPORT igdeEditPoint : public igdeContainerBoxAlternate{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditPoint> Ref;
	
	
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListener : public igdeTextFieldListener{
	protected:
		igdeEditPoint &pEditPoint;
		igdeTextField::WeakRef pTextX;
		igdeTextField::WeakRef pTextY;
		
	public:
		typedef deTObjectReference<cListener> Ref;
		
		cListener(igdeEditPoint &editPoint, igdeTextField *textX, igdeTextField *textY);
		
		virtual ~cListener();
		virtual void OnTextChanged(igdeTextField *textField);
		virtual void OnTextChanging(igdeTextField *textField);
	};
	
	
	
private:
	bool pEnabled;
	decPoint pPoint;
	int pColumns;
	bool pEditable;
	decString pDescription;
	
	igdeTextField::Ref pTextX;
	igdeTextField::Ref pTextY;
	bool pPreventUpdate;
	
	decTObjectOrderedSet<igdeEditPointListener> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit point. */
	igdeEditPoint(igdeUIHelper &helper, int columns, const char *description = "");
	
	igdeEditPoint(igdeUIHelper &helper, int columns, bool editable,
		const char *description = "");
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditPoint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Widget is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if widget is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Visible columns in edit fields. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Widget is editable. */
	inline bool GetEditable() const{ return pEditable; }
	
	/** \brief Set if widget is editable. */
	void SetEditable(bool editable);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Point. */
	inline const decPoint &GetPoint() const{ return pPoint; }
	
	/** \brief Set point. */
	void SetPoint(const decPoint &point);
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeEditPointListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeEditPointListener *listener);
	
	/** \brief Notify listeners point changed. */
	virtual void NotifyPointChanged();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief Point changed. */
	virtual void OnPointChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	/*@}*/
	
	
	
private:
	void pCreateContent(igdeUIHelper &helper);
};

#endif
