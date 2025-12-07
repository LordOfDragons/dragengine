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

#include "deModioModConfig.h"

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Class deModioModConfig
///////////////////////////

// Constructor, destructor
////////////////////////////

deModioModConfig::deModioModConfig(){
}

deModioModConfig::deModioModConfig(decBaseFileReader &reader){
	const int version = reader.ReadByte();
	switch(version){
	case 0:
		id = reader.ReadString8();
		path = reader.ReadString16();
		releaseVersion = reader.ReadString8();
		break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "Unsupported version");
	}
}

deModioModConfig::~deModioModConfig(){
}



// Management
///////////////

void deModioModConfig::WriteToFile(decBaseFileWriter &writer){
	writer.WriteByte(0);
	writer.WriteString8(id);
	writer.WriteString16(path);
	writer.WriteString8(releaseVersion);
}
