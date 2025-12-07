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

#ifndef _DEBNVALUEDATA_H_
#define _DEBNVALUEDATA_H_

#include <stdint.h>

#include "debnValue.h"

class deNetworkValueData;


/**
 * \brief Network Value Data Class.
 */
class debnValueData : public debnValue{
private:
	deNetworkValueData &pValueData;
	uint8_t *pLastValue;
	int pLastValueLen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value. */
	debnValueData(deNetworkValueData &valueData);
	
	/** \brief Clean up network value. */
	~debnValueData() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Last value is equal to network value. */
	bool LastValueEqualsNetworkValue() const;
	
	/** \brief Set last value from network value. */
	void SetLastValueFromNetworkValue();
	
	/**
	 * \brief Update value.
	 * \returns true if value needs to by synchronized otherwise false if not changed enough.
	 */
	bool UpdateValue(bool force) override;
	
	/** \brief Read value from message. */
	void ReadValue(decBaseFileReader &reader) override;
	
	/** \brief Write value to message. */
	void WriteValue(decBaseFileWriter &writer) override;
	/*@}*/
};

#endif
