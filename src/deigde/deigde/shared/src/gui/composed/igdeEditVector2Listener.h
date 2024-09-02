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

#ifndef _IGDEEDITVECTOR2LISTENER_H_
#define _IGDEEDITVECTOR2LISTENER_H_


#include <dragengine/deObject.h>


class igdeEditVector2;


/**
 * \brief IGDE UI EditVector2 Listener.
 */
class DE_DLL_EXPORT igdeEditVector2Listener : public deObject{
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeEditVector2Listener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       vectorer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditVector2Listener();
	/*@}*/
	
	
	
public:
	/** \name Events */
	/*@{*/
	/**
	 * \brief EditVector2 vector changed.
	 * 
	 * User entered vector and accepted it.
	 */
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 );
	/*@}*/
};

#endif
