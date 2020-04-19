#!/usr/bin/python
import fnmatch, os

with open('icons.h','w') as f:
	f.write("/* \n")
	f.write(" * Drag[en]gine GUI Launcher\n")
	f.write(" *\n")
	f.write(" * Copyright (C) 2010, Plüss Roland ( roland@rptd.ch )\n")
	f.write(" * \n")
	f.write(" * This program is free software; you can redistribute it and/or \n")
	f.write(" * modify it under the terms of the GNU General Public License \n")
	f.write(" * as published by the Free Software Foundation; either \n")
	f.write(" * version 2 of the License, or (at your option) any later \n")
	f.write(" * version.\n")
	f.write(" *\n")
	f.write(" * This program is distributed in the hope that it will be useful,\n")
	f.write(" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
	f.write(" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
	f.write(" * GNU General Public License for more details.\n")
	f.write(" * \n")
	f.write(" * You should have received a copy of the GNU General Public License\n")
	f.write(" * along with this program; if not, write to the Free Software\n")
	f.write(" * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n")
	f.write(" */\n")
	f.write("\n")
	f.write("// include only once\n")
	f.write("#ifndef _ICONS_H_\n")
	f.write("#define _ICONS_H_\n")
	f.write("\n")
	
	for root, dirs, files in os.walk('.'):
		del dirs[:]
		for s in fnmatch.filter(files, '*.png'):
			name = ''
			for n in s[0:-4]:
				if n.lower() in 'abcdefghijklmnopqrstuvwxyz0123456789_':
					name = name + n
				else:
					name = name + '_'
			f.write('const unsigned char icon_{}[]={{\n'.format(name))
			content = open(s,'rb').read()
			for i in range(0,len(content),16):
				f.write('  ')
				for b in content[i:i+16]:
					f.write('0x{:02x},'.format(b))
				f.write('\n')
			f.write('  };\n\n')
	
	f.write("// end of include only once\n")
	f.write("#endif\n")
