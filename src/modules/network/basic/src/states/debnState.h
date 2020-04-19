/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNSTATE_H_
#define _DEBNSTATE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/network/deBaseNetworkState.h>

class deNetworkState;
class deNetworkBasic;
class deNetworkMessage;
class debnWorld;
class debnValue;
class debnStateLink;
class debnConnection;
class debnStateLinkList;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief State.
 */
class debnState : public deBaseNetworkState{
private:
	deNetworkState &pState;
	
	debnValue **pValues;
	int pValueCount;
	int pValueSize;
	
	debnStateLinkList *pLinks;
	
	debnWorld *pParentWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create state. */
	debnState( deNetworkState &state );
	
	/** \brief Clean up state. */
	virtual ~debnState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Retrieves the state. */
	inline deNetworkState &GetState() const{ return pState; }
	
	/** Retrieves the list of state links. */
	inline debnStateLinkList *GetLinks() const{ return pLinks; }
	
	/** Update state. */
	void Update();
	
	/** Retrieves the number of values. */
	inline int GetValueCount() const{ return pValueCount; }
	
	/** \brief Read values from message. */
	void LinkReadValues( decBaseFileReader &reader, debnStateLink &link );
	
	/** \brief Read all values from message. */
	void LinkReadAllValues( decBaseFileReader &reader, debnStateLink &link );
	
	/**
	 * \brief Read all values from message including types.
	 * 
	 * Verifies that the values in the state match in type and update their values.
	 * Returns true if the state matches and has been updated or false otherwise.
	 */
	bool LinkReadAndVerifyAllValues( decBaseFileReader &reader );
	
	/**
	 * \brief Write all values to message.
	 * \param[in] withTypes Include types.
	 */
	void LinkWriteValues( decBaseFileWriter &writer );
	
	/**
	 * \brief Write all values to message.
	 * \param[in] withTypes Include types.
	 */
	void LinkWriteValuesWithVerify( decBaseFileWriter &writer );
	
	/**
	 * \brief Write values to message if changed in link.
	 * \param[in] withTypes Include types. Value only if \em allValues is true.
	 * \param[in] allValues Ignore changed flags and send all values.
	 */
	void LinkWriteValues( decBaseFileWriter &writer, debnStateLink &link );
	
	/** \brief Invalid value in all state links. */
	void InvalidateValue( int index );
	
	/** \brief Invalid value in all state links. */
	void InvalidateValueExcept( int index, debnStateLink &link );
	
	/** \brief Parent world or NULL. */
	inline debnWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( debnWorld *world );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Value added. */
	virtual void ValueAdded( int index, deNetworkValue *value );
	
	/** \brief Value changed. */
	virtual void ValueChanged( int index, deNetworkValue *value );
	/*@}*/
};

#endif
