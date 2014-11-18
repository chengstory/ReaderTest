WORK_DIR=$(shell pwd)

all:
	#build Simlator 3.x
	rm -rf ./proj.mac/build/
	xcodebuild -project ../cocos2d-x/build/cocos2d_libs.xcodeproj -target "cocos2dx Mac"
	xcodebuild -project ../cocos2d-x/build/cocos2d_libs.xcodeproj -target "cocosBuilder Mac"
	xcodebuild -project ../cocos2d-x/build/cocos2d_libs.xcodeproj -target "cocosSpine Mac"
	xcodebuild -project proj.mac/Simulator.xcodeproj -configuration Release
	cp ../CocoStudioMono/CocoStudio.EngineAdapter/Lib/libglfw.3.0.dylib ./proj.mac/build/Release/Simulator.app/Contents/MacOS/
	
clean:
	rm -rf proj.mac/build
	rm -rf ../cocos2d-x/build/build

open:
	open proj.mac/Simulator.xcodeproj

.PHONY: all clean open 
