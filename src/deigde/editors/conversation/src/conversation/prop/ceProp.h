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

#ifndef _CEPROP_H_
#define _CEPROP_H_

#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class ceConversation;



/**
 * \brief A prop for testing conversations with.
 */
class ceProp : public deObject{
private:
	ceConversation *pConversation;
	
	igdeWObject::Ref pObjectWrapper;
	
	decString pName;
	decString pObjectClass;
	decVector pPosition;
	decVector pOrientation;
	bool pVisible;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new prop. */
	ceProp();
	/** \brief Cleans up the prop. */
	virtual ~ceProp();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** \brief Sets the parent conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the name. */
	void SetName( const char *name );
	/** \brief Retrieves the object class. */
	inline const decString &GetObjectClass() const{ return pObjectClass; }
	/** \brief Sets the object class. */
	void SetObjectClass( const char *objectClass );
	/** \brief Retrieves the position of the prop. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position of the prop. */
	void SetPosition( const decVector &position );
	/** \brief Retrieves the orientation of the prop. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation of the prop. */
	void SetOrientation( const decVector &orientation );
	/** \brief Determines if the prop is visible. */
	inline bool GetVisible() const{ return pVisible; }
	/** \brief Sets if the prop is visible. */
	void SetVisible( bool visible );
	
	/** \brief Retrieves the object wrapper. */
	inline const igdeWObject::Ref &GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
