/* 
 * Drag[en]gine Console Input Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// includes
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <curses.h>

#include <termios.h>

#include <sys/time.h>

#include <poll.h>

#include "deConsoleInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSConsole.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Export definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *ConsoleInputCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *ConsoleInputCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deConsoleInput( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Function Emulation
///////////////////////

int readNextChar(){
	struct pollfd fds;
	
	fds.fd = 0;
	fds.events = POLLIN;
	
	if( poll( &fds, 1, 0 ) == 1 ){
		if( fds.revents & POLLIN ){
			return fgetc( stdin );
			
		}else{
			return 0;
		}
		
	}else{
		return 0;
	}
}

/*
int kbhit(){ // from: http://www.c-plusplus.de/forum/viewtopic-var-p-is-285478.html
	struct termios term, oterm;
	int fd = 0;
	int c = 0;
	tcgetattr(fd, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag = term.c_lflag & (!ICANON);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 1;
	tcsetattr(fd, TCSANOW, &term);
	c = getchar();
	tcsetattr(fd, TCSANOW, &oterm);
	if (c != -1)
	ungetc(c, stdin);
	return ((c != -1) ? 1 : 0);
}

int getch(){ // from: http://www.c-plusplus.de/forum/viewtopic-var-p-is-285478.html
	static int ch = -1, fd = 0;
	struct termios neu, alt;
	fd = fileno(stdin);
	tcgetattr(fd, &alt);
	neu = alt;
	neu.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(fd, TCSANOW, &neu);
	ch = getchar();
	tcsetattr(fd, TCSANOW, &alt);
	return ch;
}
*/



// Class deConsoleInput
/////////////////////////

// Constructor, destructor
////////////////////////////

deConsoleInput::deConsoleInput( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ),
pKeyCodeMap( NULL ){
}

deConsoleInput::~deConsoleInput(){
}



// Management
///////////////

bool deConsoleInput::Init(){
	pInitKeyCodeMap();
	
	/*initscr();
	cbreak();
	noecho();
	keypad( stdscr, TRUE );
	nodelay( stdscr, TRUE );
	
	getmaxyx( stdscr, pScreenHeight, pScreenWidth );*/
	
	tcgetattr( 0, &pOldTermSettings );
	
	termios newTermSettings = pOldTermSettings;
	newTermSettings.c_lflag &= ( ~ICANON );
	newTermSettings.c_lflag &= ( ~ECHO ); // no echo of characters
	newTermSettings.c_lflag &= ( ~ISIG ); // no auto handling of ctrl-c
	newTermSettings.c_cc[ VTIME ] = 1; // timeout in tenths of a second
	newTermSettings.c_cc[ VMIN ] = 0; // minimum number of characters
	tcsetattr( 0, TCSANOW, &newTermSettings );
	
	return true;
}

void deConsoleInput::CleanUp(){
	//endwin();
	tcsetattr( 0, TCSANOW, &pOldTermSettings );
	
	if( pKeyCodeMap ){
		delete [] pKeyCodeMap;
		pKeyCodeMap = NULL;
	}
}



// Devices
////////////

int deConsoleInput::GetDeviceCount(){
	return 0;
}

deInputDevice *deConsoleInput::GetDeviceAt( int index ){
	DETHROW( deeInvalidParam );
}

int deConsoleInput::IndexOfDeviceWithID( const char *id ){
	return -1;
}

int deConsoleInput::IndexOfButtonWithID( int device, const char *id ){
	return -1;
}

int deConsoleInput::IndexOfAxisWithID( int device, const char *id ){
	return -1;
}

int deConsoleInput::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

bool deConsoleInput::GetButtonPressed( int device, int button ){
	DETHROW( deeInvalidParam );
}

float deConsoleInput::GetAxisValue( int device, int axis ){
	DETHROW( deeInvalidParam );
}

float deConsoleInput::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deConsoleInput::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

int deConsoleInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
	if( device != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<256; i++ ){
		if( pKeyCodeMap[ i ] == keyCode ){
			return i;
		}
	}
	
	return -1;
}

int deConsoleInput::ButtonMatchingKeyChar( int device, int character ){
	if( device != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<256; i++ ){
		if( pKeyCodeMap[ i ] == character ){
			return i;
		}
	}
	
	return -1;
}



// Events
///////////

void deConsoleInput::ProcessEvents(){
	pProcessEventQueue();
}



// Private functions
//////////////////////

void deConsoleInput::pInitKeyCodeMap(){
	int i;
	
	// table for key codes with high byte 0x00
	pKeyCodeMap = new deInputEvent::eKeyCodes[ 256 ];
	for( i=0; i<256; i++ ){
		pKeyCodeMap[ i ] = deInputEvent::ekcUndefined;
	}
	
	pKeyCodeMap[ ( int )' ' ] = deInputEvent::ekcSpace;
	pKeyCodeMap[ ( int )'\t' ] = deInputEvent::ekcTab;
	pKeyCodeMap[ ( int )'\n' ] = deInputEvent::ekcReturn;
	
	for( i='0'; i<='9'; i++ ){
		pKeyCodeMap[ i ] = ( deInputEvent::eKeyCodes )( deInputEvent::ekc0 + ( i - '0' ) );
	}
	
	for( i='a'; i<='z'; i++ ){
		pKeyCodeMap[ i ] = ( deInputEvent::eKeyCodes )( deInputEvent::ekcA + ( i - 'a' ) );
	}
	
	for( i='A'; i<='Z'; i++ ){
		pKeyCodeMap[ i ] = ( deInputEvent::eKeyCodes )( deInputEvent::ekcA + ( i - 'A' ) );
	}
}

void deConsoleInput::pAddKeyPress( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deConsoleInput::pAddKeyRelease( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deConsoleInput::pProcessEventQueue(){
decTimer debugTimer;
	timeval eventTime;
	int character;
	
	character = readNextChar();
	while( character ){
		gettimeofday( &eventTime, NULL );
		
		const deInputEvent::eKeyCodes keyCode = pKeyCodeMap[ character ];
		
		if( keyCode == deInputEvent::ekcQ ){
			GetGameEngine()->Quit();
			
		}else if( keyCode != deInputEvent::ekcUndefined ){
			pAddKeyPress( 0, character, character, keyCode, eventTime );
			pAddKeyRelease( 0, character, character, keyCode, eventTime );
		}
		
		character = readNextChar();
	}
}
