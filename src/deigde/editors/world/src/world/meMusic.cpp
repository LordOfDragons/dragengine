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

#include "meMusic.h"
#include "meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/sound/deSoundManager.h>

#include <deigde/environment/igdeEnvironment.h>


// Definitions
////////////////

#define LOGSOURCE "World Editor"


// Class meMusic
//////////////////

// Constructor, destructor
////////////////////////////

meMusic::meMusic(meWorld &world) :
pWorld(world),
pSpeaker(world.GetEngine()->GetSpeakerManager()->CreateSpeaker())
{
	world.GetMicrophone()->AddSpeaker(pSpeaker);
}

meMusic::~meMusic(){
	pCleanUp();
}



// Management
///////////////

void meMusic::SetPath(const char *path){
	if(pPath == path){
		return;
	}
	
	pSpeaker->SetSound(nullptr);
	
	pPath = path;
	
	if(!pPath.IsEmpty()){
		try{
			pSpeaker->SetSound(pWorld.GetEngine()->GetSoundManager()->LoadSound(path, "/", false));
			
		}catch(const deException &e){
			pWorld.GetEnvironment()->GetLogger()->LogException(LOGSOURCE, e);
		}
	}
	
	pWorld.NotifyMusicChanged();
}

float meMusic::GetVolume() const{
	return pSpeaker->GetVolume();
}

void meMusic::SetVolume(float volume){
	if(fabsf(volume - pSpeaker->GetVolume()) < 0.001f){
		return;
	}
	
	pSpeaker->SetVolume(volume);
	pWorld.NotifyMusicChanged();
}

void meMusic::Play(){
	pSpeaker->Play();
}

void meMusic::Pause(){
	pSpeaker->Pause();
}

void meMusic::Stop(){
	pSpeaker->Stop();
}


// Private Functions
//////////////////////

void meMusic::pCleanUp(){
	if(pSpeaker){
		pWorld.GetMicrophone()->RemoveSpeaker(pSpeaker);
	}
}
