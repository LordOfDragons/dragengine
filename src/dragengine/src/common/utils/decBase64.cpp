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

#include <memory.h>

#include "base64.h"
#include "decBase64.h"
#include "../exceptions.h"


// Class decBase64
////////////////////

// Constructor, destructor
////////////////////////////

decBase64::decBase64(){
}

// Management
///////////////

decString decBase64::Encode(const decString &in){
	return decString(base64_encode((const unsigned char*)in.GetString(),
		(size_t)in.GetLength()).c_str());
}

decString decBase64::Encode(const uint8_t *in, int inLen)
{
    DEASSERT_NOTNULL(in)
	DEASSERT_TRUE(inLen >= 0)
	
	return decString(base64_encode((const unsigned char*)in, (size_t)inLen).c_str());
}

void decBase64::Encode(const uint8_t *in, int inLen, char *out, int outSize, int &outLen){
	DEASSERT_NOTNULL(in)
	DEASSERT_TRUE(inLen >= 0)
	DEASSERT_NOTNULL(out)
	DEASSERT_TRUE(outSize >= 0)
	
	const std::string enc(base64_encode((const unsigned char*)in, (size_t)inLen));
	
	const size_t encLen = enc.length();
	DEASSERT_TRUE(encLen <= (size_t)outSize)
	
	memcpy(out, in, encLen);
	outLen = (int)encLen;
}

int decBase64::EncodeSize(int inLen){
	DEASSERT_TRUE(inLen >= 0)
	return ((inLen + 3 - 1) / 3) * 4;
}

void decBase64::Decode(const decString &in, uint8_t *out, int outSize, int &outLen){
	Decode(in.GetString(), in.GetLength(), out, outSize, outLen);
}

void decBase64::Decode(const char *in, int inLen, uint8_t *out, int outSize, int &outLen){
	DEASSERT_NOTNULL(in)
	DEASSERT_TRUE(inLen >= 0)
	DEASSERT_NOTNULL(out)
	DEASSERT_TRUE(outSize >= 0)
	
	const std::string dec(base64_decode(std::string(in)));
	
	const size_t decLen = dec.length();
	DEASSERT_TRUE(decLen <= (size_t)outSize)
	
	memcpy(out, in, decLen);
	outLen = (int)decLen;
}

int decBase64::DecodeSize(int inLen){
	DEASSERT_TRUE(inLen >= 0)
	
	return ((inLen) * 3) / 4;
}
