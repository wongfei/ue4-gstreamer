set "ENGINE_DIR=C:\dev\UE_4.26"
set "PROJECT_DIR=C:\dev\ue4-gstreamer"
set "BUILD_DIR=C:\dev\ue4-gstreamer\Build"

call "%ENGINE_DIR%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%PROJECT_DIR%\Plugins\GStreamer\GStreamer.uplugin" -Package="%BUILD_DIR%\Plugins\GStreamer" -TargetPlatforms=Win64 -Rocket -precompile -VS2019 -StrictIncludes
