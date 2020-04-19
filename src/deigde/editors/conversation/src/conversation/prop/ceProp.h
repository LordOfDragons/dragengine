/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _CEPROP_H_
#define _CEPROP_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class ceConversation;

class igdeWObject;



/**
 * \brief A prop for testing conversations with.
 */
class ceProp : public deObject{
private:
	ceConversation *pConversation;
	
	igdeWObject *pObjectWrapper;
	
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
	inline igdeWObject *GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
