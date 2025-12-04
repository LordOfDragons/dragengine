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

#ifndef _IGDELABEL_H_
#define _IGDELABEL_H_

#include "igdeWidget.h"
#include "resources/igdeIcon.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Label with text and icon.
 */
class DE_DLL_EXPORT igdeLabel : public igdeWidget{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeLabel> Ref;
	
	
	

	/** \brief Alignment. */
	enum eAlignment{
		/** \brief Center alignment. */
		eaCenter = 0,
		
		/** \brief Left alignment. */
		eaLeft = 0x1,
		
		/** \brief Right alignment. */
		eaRight = 0x2,
		
		/** \brief Middle alignment. */
		eaMiddle = 0,
		
		/** \brief Top alignment. */
		eaTop = 0x4,
		
		/** \brief Bottom alignment. */
		eaBottom = 0x8,
	};
	
	
	
private:
	decString pText;
	int pAlignment;
	decString pDescription;
	igdeIcon::Ref pIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, const char *description,
		int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, igdeIcon *icon,
		int alignment = eaCenter | eaMiddle );
	
	/** \brief Create label. */
	igdeLabel( igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, int alignment = eaCenter | eaMiddle );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeLabel();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Alignment. */
	inline int GetAlignment() const{ return pAlignment; }
	
	/** \brief Set alignment. */
	void SetAlignment( int alignment );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
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
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Alignment changed. */
	virtual void OnAlignmentChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Icon changed. */
	virtual void OnIconChanged();
	/*@}*/
};

#endif
