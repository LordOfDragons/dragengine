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

#ifndef _deECCommon_H_
#define _deECCommon_H_

#include <stdint.h>
#include <eos_common.h>

#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/service/deServiceObject.h>


/**
 * Convert common data.
 */
class deECCommon{
private:
	deECCommon() = default;
	
public:
	/** Convert uint32_t. */
	static uint32_t UInt32(const deServiceObject &so);
	static deServiceObject::Ref UInt32(uint32_t value);
	
	static uint32_t UInt32(const decString &string);
	static decString UInt32ToString(uint32_t value);
	
	/** Convert AccountID. */
	static EOS_EpicAccountId AccountID(const deServiceObject &so);
	static deServiceObject::Ref AccountID(const EOS_EpicAccountId &id);
	
	static EOS_EpicAccountId AccountID(const decString &string);
	static decString AccountIDToString(const EOS_EpicAccountId &id);
	
	/** Convert string list. */
	static decStringList StringList(const deServiceObject &so);
	static deServiceObject::Ref StringList(const decStringList &list);
};

#endif
