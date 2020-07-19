#!/usr/bin/python3

import xml.etree.ElementTree as ET
import hashlib, sys, os

pathManifest = sys.argv[1]
dirManifest = os.path.split(pathManifest)[0]

tree = ET.parse(pathManifest)

library = tree.getroot().find('library')

libraryContent = open(os.path.join(dirManifest, library.find('file').text), 'rb').read()
library.find('size').text = str(len(libraryContent))
library.find('sha1').text = str(hashlib.sha1(libraryContent).hexdigest())

tree.write(pathManifest)
