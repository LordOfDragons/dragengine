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

#ifndef _IGDETEXTFIELDLISTENER_H_
#define _IGDETEXTFIELDLISTENER_H_


#include <dragengine/deObject.h>


class igdeTextField;


/**
 * \brief IGDE UI TextField Listener.
 */
class DE_DLL_EXPORT igdeTextFieldListener : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTextFieldListener> Ref;
	
	
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeTextFieldListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextFieldListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief TextField text changed.
	 * 
	 * User entered text and accepted it.
	 */
	virtual void OnTextChanged(igdeTextField *textField);
	
	/**
	 * \brief TextField text is changing.
	 * 
	 * User is entering text but has not yet accepted the input. This event is send each
	 * time the user changes the text. Once accepted OnTextChanged() is called.
	 */
	virtual void OnTextChanging(igdeTextField *textField);
	
	/**
	 * \brief Enter key has been pressed.
	 * 
	 * Send after OnTextChanged() has been send.
	 */
	virtual void OnEnterKey(igdeTextField *textField);
	/*@}*/
};

#endif
