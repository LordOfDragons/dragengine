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

#include "deECCommon.h"

#include <dragengine/common/math/decMath.h>


// Class deECCommon
/////////////////////

uint32_t deECCommon::UInt32(const deServiceObject &so){
	return UInt32(so.GetString());
}

deServiceObject::Ref deECCommon::UInt32(uint32_t value){
	return deServiceObject::NewString(UInt32ToString(value));
}

uint32_t deECCommon::UInt32(const decString &string){
	return (uint32_t)string.ToLongValid();
}

decString deECCommon::UInt32ToString(uint32_t value){
	decString string;
	string.AppendValue((unsigned int)value);
	return string;
}

EOS_EpicAccountId deECCommon::AccountID(const deServiceObject &so){
	return AccountID(so.GetString());
}

deServiceObject::Ref deECCommon::AccountID(const EOS_EpicAccountId &id){
	return deServiceObject::NewString(AccountIDToString(id));
}

EOS_EpicAccountId deECCommon::AccountID(const decString &string){
	return EOS_EpicAccountId_FromString(string);
}

decString deECCommon::AccountIDToString(const EOS_EpicAccountId &id){
	char buffer[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
	int32_t len = EOS_EPICACCOUNTID_MAX_LENGTH;
	const EOS_EResult res = EOS_EpicAccountId_ToString(id, buffer, &len);
	DEASSERT_TRUE(res == EOS_EResult::EOS_Success)
	return buffer;
}

decStringList deECCommon::StringList(const deServiceObject &so){
	const int count = so.GetChildCount();
	decStringList list;
	int i;
	
	for(i=0; i<count; i++){
		list.Add(so.GetChildAt(i)->GetString());
	}
	
	return list;
}

deServiceObject::Ref deECCommon::StringList(const decStringList &list){
	const deServiceObject::Ref so(deServiceObject::NewList());
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		so->AddStringChild(list.GetAt(i));
	}
	
	return so;
}
