#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import subprocess
import shutil
import sys
import burger
	
#
# Build the burgerlib headers
#

def main(workingDir):

	#
	# Was Burgerlib already installed?
	#
	
	sdks = burger.getsdksfolder()

	#
	# Update the changelist header
	#
	
	burger.makeversionheader(workingDir,os.path.join(workingDir,'source','version.h'))

	#
	# Folders for all the target operating systems supported
	#
	
	destinations = [
		'windows',
		'dos',
		'mac',
		'macosx',
		'linux',
		'beos',
		'ps2',
		'ps3',
		'ps4',
		'gamecube',
		'wii',
		'dsi',
		'xbox',
		'xbox360',
		'xboxone',
		'ios',
		'android',
		'shield',
		'ouya']
		
	
	#
	# Ensure the destination directories exist
	#
	
	for dest in destinations:
		burger.createfolderifneeded(os.path.join(sdks,dest,'burgerlib'))

	
	#
	# Ensure the output folder exists
	#
	
	outputfolder = os.path.join(workingDir,'bin')
	burger.createfolderifneeded(outputfolder)

	#
	# Create the super header
	#

	
	burgerhfile = os.path.join(outputfolder,'burger.h')
	cmd = 'makeheader "' + os.path.join(workingDir,'source','templateburgerbase.h') + '" "' + burgerhfile + '"'
	error = subprocess.call(cmd,cwd=workingDir,shell=True)
	if error==0:
	
		# Was there a change in the output?
	
		windowsburgerlib = os.path.join(sdks,'windows','burgerlib')
		testfile = os.path.join(windowsburgerlib,'burger.h')
		if os.path.isfile(testfile)!=True or \
			burger.comparefiles(burgerhfile,testfile)!=True:
			
			# Copy only if changed
	
			for dest in destinations:
				destfolder = os.path.join(sdks,dest,'burgerlib')
				error = burger.copyfileandcheckoutifneeded(burgerhfile,os.path.join(destfolder,'burger.h'))
				if error!=0:
					break


	#
	# Was there a change in special headers?
	#
	
	if error==0:
		specialheaders = [
			'brstartup.h',
			'brgl.h',
			'brglext.h',
			'brglut.h',
			'brglxext.h'
			]
	
		for item in specialheaders:
			brstartuphfile = os.path.join(workingDir,'source',item)
			testfile = os.path.join(sdks,'windows','burgerlib',item)
			if os.path.isfile(testfile)!=True or \
				burger.comparefiles(brstartuphfile,testfile)==False:
				for dest in destinations:
					error = burger.copyfileandcheckoutifneeded(brstartuphfile,os.path.join(sdks,dest,'burgerlib',item))
					if error!=0:
						break
	
	#
	# Did any of the Mac Carbon/Classic resource files change?
	#
	
	if error==0:
		macosfolder = os.path.join(workingDir,'source','mac')
		destfolder = os.path.join(sdks,'mac','burgerlib')
		filedata = os.listdir(macosfolder)
		for file in filedata:
			tfile = file.lower()
			if tfile.endswith('.r'):
				destfile = os.path.join(destfolder,file)
				sourcefile = os.path.join(macosfolder,file)
				if os.path.isfile(destfile)!=True or \
					burger.comparefiles(sourcefile,destfile)==False:
					error = burger.copyfileandcheckoutifneeded(sourcefile,destfile)
					if error!=0:
						break
		
	return error
		
# 
# If called as a function and not a class,
# call my main
#

if __name__ == "__main__":
	sys.exit(main(os.path.dirname(os.path.abspath(__file__))))

