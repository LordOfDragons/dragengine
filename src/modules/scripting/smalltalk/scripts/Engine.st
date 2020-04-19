" Smalltalk Class DEEngine
|
| Copyright (C) 2009, Plüss Roland ( roland@rptd.ch )
|
| This program is free software; you can redistribute it and/or 
| modify it under the terms of the GNU General Public License 
| as published by the Free Software Foundation; either 
| version 2 of the License, or (at your option) any later 
| version.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
| 
| You should have received a copy of the GNU General Public License
| along with this program; if not, write to the Free Software
| Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."

Object subclass: DEEngine [
	<comment: 'I am the game engine. I am a singleton.'>
	<shape: #byte>
	
	DEEngine class [
		| scripting |
		
		new [
			self error: #shouldNotImplement
		]
		
		elapsedTime [
			<category: 'engine information'>
			<cCall: 'DEEngine.getElapsedTime' returning: #double args: #(#self)>
		]
		
		fpsRate [
			<category: 'engine information'>
			<cCall: 'DEEngine.getFPSRate' returning: #int args: #(#self)>
		]
		
		arguments [
			<category: 'engine information'>
			<cCall: 'DEEngine.getArguments' returning: #smalltalk args: #(#self)>
		]
		
		game [
			<category: 'engine information'>
			<cCall: 'DEEngine.getGame' returning: #smalltalk args: #(#self)>
		]
		
		quit [
			<category: 'engine handling'>
			<cCall: 'DEEngine.quit' returning: #void args: #(#self)>
		]
		
		resetTimers [
			<category: 'engine handling'>
			<cCall: 'DEEngine.resetTimers' returning: #void args: #(#self)>
		]
	]
]
