"%GSTREAMER_1_0_ROOT_MSVC_X86_64%\bin\gst-launch-1.0.exe" -v tcpclientsrc host=127.0.0.1 port=5000 ! videoparse width=640 height=480 framerate=60/1 format=8 ! autovideoconvert ! autovideosink
