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
	debnState(deNetworkState &state);
	
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
	void LinkReadValues(decBaseFileReader &reader, debnStateLink &link);
	
	/** \brief Read all values from message. */
	void LinkReadAllValues(decBaseFileReader &reader, debnStateLink &link);
	
	/**
	 * \brief Read all values from message including types.
	 * 
	 * Verifies that the values in the state match in type and update their values.
	 * Returns true if the state matches and has been updated or false otherwise.
	 */
	bool LinkReadAndVerifyAllValues(decBaseFileReader &reader);
	
	/**
	 * \brief Write all values to message.
	 * \param[in] withTypes Include types.
	 */
	void LinkWriteValues(decBaseFileWriter &writer);
	
	/**
	 * \brief Write all values to message.
	 * \param[in] withTypes Include types.
	 */
	void LinkWriteValuesWithVerify(decBaseFileWriter &writer);
	
	/**
	 * \brief Write values to message if changed in link.
	 * \param[in] withTypes Include types. Value only if \em allValues is true.
	 * \param[in] allValues Ignore changed flags and send all values.
	 */
	void LinkWriteValues(decBaseFileWriter &writer, debnStateLink &link);
	
	/** \brief Invalid value in all state links. */
	void InvalidateValue(int index);
	
	/** \brief Invalid value in all state links. */
	void InvalidateValueExcept(int index, debnStateLink &link);
	
	/** \brief Parent world or NULL. */
	inline debnWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(debnWorld *world);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Value added. */
	virtual void ValueAdded(int index, deNetworkValue *value);
	
	/** \brief Value changed. */
	virtual void ValueChanged(int index, deNetworkValue *value);
	/*@}*/
};

#endif
