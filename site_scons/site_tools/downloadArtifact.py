import urllib.request
import shutil
import hashlib
import os
from SCons.Builder import Builder
from SCons.Action import Action

# Provides support to download artifact.
#
# Adds builder DownloadArtifact()

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	def builderDownloadArtifact(env, target, source):
		url = "{}/{}".format(env['baseUrl'] or env['url_extern_artifacts'], env['url'])
		filename = target[0].abspath
		
		# download file and checksum
		with urllib.request.urlopen(url) as r, open(filename, 'wb') as f:
			shutil.copyfileobj(r, f)
		
		with urllib.request.urlopen(url + '.sha256sum') as r, open(filename + '.sha256sum', 'wb') as f:
			shutil.copyfileobj(r, f)
		
		# verify checksum. file contains only the checksum but to be on the safe side use only
		# the first token in case sha256sum output has been stored directly (checksum filename")
		with open(filename + '.sha256sum', 'rb') as f:
			content = f.read().decode('utf-8', errors='ignore').strip()
		if not content:
			raise Exception('Empty checksum file for "{}"'.format(filename))
		expected = content.split()[0].strip()
		
		h = hashlib.sha256()
		with open(filename, 'rb') as f:
			for chunk in iter(lambda: f.read(8192), b''):
				h.update(chunk)
		actual = h.hexdigest()
		
		if actual.lower() != expected.lower():
			# invalid file or checksum. remove both files to force redownload the next time
			try:
				os.remove(filename)
			except Exception:
				pass
			try:
				os.remove(filename + '.sha256sum')
			except Exception:
				pass
			raise Exception('Checksum mismatch for "{}": expected {}, got {}'.format(filename, expected, actual))
	
	def downloadArtifactHelper(filenameArtifact, urlPath, alias, fakeSource='SConscript', baseUrl=None):
		if type(filenameArtifact) not in [list, tuple]:
			filenameArtifact = [filenameArtifact]
		
		nodes = []
		buildNodes = []
		
		for x in filenameArtifact:
			node = env.File(x).srcnode()
			if node.exists():
				nodes.append(x)
			else:
				nodes.append(node)
				buildNodes.append(env.DownloadArtifact(node, fakeSource,
					url="{}/{}".format(urlPath, x), baseUrl=baseUrl))
		
		env.Alias(alias, buildNodes)
		return nodes
	
	env.Append(BUILDERS={'DownloadArtifact': Builder(action=Action(
		builderDownloadArtifact, '$DOWNLOADARTIFACTCOMSTR'))})
	env['DOWNLOADARTIFACTCOM'] = 'Download Artifact "$TARGET"'
	env['DOWNLOADARTIFACTCOMSTR'] = env['DOWNLOADARTIFACTCOM']
	
	env.DownloadArtifactHelper = downloadArtifactHelper

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
