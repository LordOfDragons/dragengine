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

#ifndef _IGDECONTAINERBOX_H_
#define _IGDECONTAINERBOX_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in Box layout.
 * 
 * Widgets are placed next to each other along the X or Y axis. Widget sizes along the axis
 * are equally sized relative to their desired size to fill all the available space.
 * The widget sizes along the other axis is set to fill the available space.
 */
class DE_DLL_EXPORT igdeContainerBox : public igdeContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeContainerBox> Ref;
	
	
	/** \brief Layout axis for subclasses. */
	enum eAxis{
		/** \brief Layout along the X axis. */
		eaX,
		
		/** \brief Layout along the Y axis. */
		eaY,
	};
	
	
	
private:
	eAxis pAxis;
	int pSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerBox(igdeEnvironment &environment, eAxis axis, int spacing = 0);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Layout axis. */
	inline eAxis GetAxis() const{return pAxis;}
	
	/** \brief Spacing. */
	inline int GetSpacing() const{return pSpacing;}
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
	/*@}*/
};

#endif
