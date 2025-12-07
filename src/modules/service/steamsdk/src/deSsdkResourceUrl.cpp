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

#include "deSsdkResourceUrl.h"
#include "convert/deSCCommon.h"
#include <dragengine/common/exceptions.h>


// Class deSsdkResourceUrl
////////////////////////////

// Constructor, destructor
////////////////////////////

deSsdkResourceUrl::deSsdkResourceUrl(const decString &nurl) :
url(nurl)
{
	const int index = nurl.FindString("://");
	DEASSERT_TRUE(index != -1);
	
	type = nurl.GetLeft(index);
	components = nurl.GetMiddle(index + 3).Split('/');
}


// Management
///////////////

const decString &deSsdkResourceUrl::getComponentAt(int index, const char *paramName) const{
	if(index < 0 || index >= components.GetCount()){
		DETHROW_INFO(deeInvalidParam, paramName);
	}
	return components.GetAt(index);
}

decString deSsdkResourceUrl::FormatUrl(const char *part1, const char *part2,
const char *part3, const char *part4){
	decString url;
	url.Format("res://%s/%s/%s/%s", part1, part2, part3, part4);
	return url;
}

decString deSsdkResourceUrl::FormatUrl(const char *part1, uint32 id,
const char *part2, const char *part3){
	return FormatUrl(part1, deSCCommon::UInt32ToString(id), part2, part3);
}
