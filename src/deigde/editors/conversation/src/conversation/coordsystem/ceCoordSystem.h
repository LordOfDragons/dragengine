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

#ifndef _CECOORDSYSTEM_H_
#define _CECOORDSYSTEM_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Conversation coordinate system.
 */
class ceCoordSystem : public deObject{
private:
	ceConversation *pConversation;
	
	decString pID;
	decString pAliasID;
	decVector pPosition;
	decVector pOrientation;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCoordSystem> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new coordinate system. */
	ceCoordSystem();
	/** \brief Cleans up the conversation actor. */
	virtual ~ceCoordSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** \brief Sets the parent conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Retrieves the id. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the id. */
	void SetID( const char *id );
	/** \brief Retrieves the alias id. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** \brief Sets the alias id. */
	void SetAliasID( const char *id );
	/** \brief Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition( const decVector &position );
	/** \brief Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Notifies all that the coordinate system changed. */
	void NotifyCoordSystemChanged();
	/*@}*/
};

#endif
