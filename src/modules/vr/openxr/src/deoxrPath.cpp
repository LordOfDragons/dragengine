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

#include <stdlib.h>
#include <string.h>

#include "deoxrPath.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"


// class deoxrPath
////////////////////

deoxrPath::deoxrPath() :
pPath(XR_NULL_PATH){
}

deoxrPath::deoxrPath(const deoxrInstance &instance, XrPath path) :
pPath(path)
{
	if(path == XR_NULL_PATH){
		return;
	}
	
	uint32_t bufferCountOutput;
	
	OXR_CHECK(instance.xrPathToString(instance.GetInstance(),
		path, 0, &bufferCountOutput, nullptr));
	
	pName.Set(' ', bufferCountOutput);
	
	OXR_CHECK(instance.xrPathToString(instance.GetInstance(),
		path, bufferCountOutput, &bufferCountOutput, (char*)pName.GetString()));
}

deoxrPath::deoxrPath(const deoxrInstance &instance, const char *name) :
pPath(XR_NULL_PATH),
pName(name)
{
	OXR_CHECK(instance.xrStringToPath(instance.GetInstance(), name, &pPath));
}

deoxrPath::deoxrPath(const deoxrPath &path) :
pPath(path.pPath),
pName(path.pName){
}

deoxrPath::~deoxrPath(){
}



// Management
///////////////

void deoxrPath::Empty(){
	pPath = XR_NULL_PATH;
	pName.Empty();
}



// Operators
//////////////

deoxrPath &deoxrPath::operator=(const deoxrPath &path){
	pPath = path.pPath;
	pName = path.pName;
	return *this;
}
