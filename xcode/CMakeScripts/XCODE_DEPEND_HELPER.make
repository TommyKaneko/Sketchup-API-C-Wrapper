# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.GoogleTest.Debug:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libGoogleTest.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libGoogleTest.a


PostBuild.SketchUpAPICpp.Debug:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libSketchUpAPICpp.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libSketchUpAPICpp.a


PostBuild.SketchUpAPITests.Debug:
PostBuild.GoogleTest.Debug: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/SketchUpAPITests
PostBuild.SketchUpAPICpp.Debug: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/SketchUpAPITests
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/SketchUpAPITests:\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libGoogleTest.a\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libSketchUpAPICpp.a
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/SketchUpAPITests


PostBuild.GoogleTest.Release:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libGoogleTest.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libGoogleTest.a


PostBuild.SketchUpAPICpp.Release:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libSketchUpAPICpp.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libSketchUpAPICpp.a


PostBuild.SketchUpAPITests.Release:
PostBuild.GoogleTest.Release: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/SketchUpAPITests
PostBuild.SketchUpAPICpp.Release: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/SketchUpAPITests
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/SketchUpAPITests:\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libGoogleTest.a\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libSketchUpAPICpp.a
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/SketchUpAPITests


PostBuild.GoogleTest.MinSizeRel:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libGoogleTest.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libGoogleTest.a


PostBuild.SketchUpAPICpp.MinSizeRel:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libSketchUpAPICpp.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libSketchUpAPICpp.a


PostBuild.SketchUpAPITests.MinSizeRel:
PostBuild.GoogleTest.MinSizeRel: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/SketchUpAPITests
PostBuild.SketchUpAPICpp.MinSizeRel: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/SketchUpAPITests
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/SketchUpAPITests:\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libGoogleTest.a\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libSketchUpAPICpp.a
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/SketchUpAPITests


PostBuild.GoogleTest.RelWithDebInfo:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libGoogleTest.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libGoogleTest.a


PostBuild.SketchUpAPICpp.RelWithDebInfo:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libSketchUpAPICpp.a:
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libSketchUpAPICpp.a


PostBuild.SketchUpAPITests.RelWithDebInfo:
PostBuild.GoogleTest.RelWithDebInfo: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/SketchUpAPITests
PostBuild.SketchUpAPICpp.RelWithDebInfo: /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/SketchUpAPITests
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/SketchUpAPITests:\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libGoogleTest.a\
	/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libSketchUpAPICpp.a
	/bin/rm -f /Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/SketchUpAPITests




# For each target create a dummy ruleso the target does not have to exist
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libGoogleTest.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Debug/libSketchUpAPICpp.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libGoogleTest.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/MinSizeRel/libSketchUpAPICpp.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libGoogleTest.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/RelWithDebInfo/libSketchUpAPICpp.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libGoogleTest.a:
/Users/tomkaneko/Dropbox/Sketchup/Extensions/Sketchup-API-C-Wrapper/xcode/Release/libSketchUpAPICpp.a:
