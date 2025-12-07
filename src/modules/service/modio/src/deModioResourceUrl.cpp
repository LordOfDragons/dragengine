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

#include "deModioResourceUrl.h"
#include "convert/deMCCommon.h"
#include <dragengine/common/exceptions.h>


// Class deModioResourceUrl
/////////////////////////////

// Constructor, destructor
////////////////////////////

deModioResourceUrl::deModioResourceUrl(const decString &nurl) :
url(nurl)
{
	const int index = nurl.FindString("://");
	DEASSERT_TRUE(index != -1);
	
	type = nurl.GetLeft(index);
	components = nurl.GetMiddle(index + 3).Split('/');
}


// Management
///////////////

const decString &deModioResourceUrl::getComponentAt(int index, const char *paramName) const{
	if(index < 0 || index >= components.GetCount()){
		DETHROW_INFO(deeInvalidParam, paramName);
	}
	return components.GetAt(index);
}

decString deModioResourceUrl::FormatUrl(const char *part1, const char *part2,
const char *part3, const char *part4){
	decString url;
	url.Format("res://%s/%s/%s/%s", part1, part2, part3, part4);
	return url;
}

decString deModioResourceUrl::FormatUrl(const char *part1, std::int64_t id,
const char *part2, const char *part3){
	return FormatUrl(part1, deMCCommon::IDToString(id), part2, part3);
}

decString deModioResourceUrl::FormatUrl(const char *part1, std::int64_t id,
const char *part2, int index, const char *part3){
	decString url;
	url.Format("res://%s/%s/%s/%d/%s", part1, deMCCommon::IDToString(id).GetString(), part2, index, part3);
	return url;
}
