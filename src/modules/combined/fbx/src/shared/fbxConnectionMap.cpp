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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "fbxConnectionMap.h"
#include "fbxConnection.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>


// Class fbxConnectionMap
///////////////////////////

// Constructor, destructor
////////////////////////////

fbxConnectionMap::fbxConnectionMap(int expectedCount)
{
	if(expectedCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	const int count = decMath::max((int)((float)expectedCount * 0.75f), 1);
	pBucketsSource.AddRange(count, {});
	pBucketsTarget.AddRange(count, {});
}

fbxConnectionMap::~fbxConnectionMap(){
}



// Management
///////////////

void fbxConnectionMap::Add(fbxConnection *connection){
	if(!connection){
		DETHROW(deeInvalidParam);
	}
	
	if(connection->GetSource() != 0){
		pBucketsSource[connection->GetSource() % pBucketsSource.GetCount()].connections.Add(connection);
	}
	if(connection->GetTarget() != 0){
		pBucketsTarget[connection->GetTarget() % pBucketsTarget.GetCount()].connections.Add(connection);
	}
}

void fbxConnectionMap::Get(int64_t id, decTList<fbxConnection*> &list) const{
	const decTList<fbxConnection*> &consSource = pBucketsSource[id % pBucketsSource.GetCount()].connections;
	const decTList<fbxConnection*> &consTarget = pBucketsTarget[id % pBucketsTarget.GetCount()].connections;
	
	consSource.Visit([&](fbxConnection *connection){
		if(connection->GetSource() == id){
			list.Add(connection);
		}
	});
	
	consTarget.Visit([&](fbxConnection *connection){
		if(connection->GetTarget() == id){
			list.Add(connection);
		}
	});
}
