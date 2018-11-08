# UE4 GStreamer plugin

[![Demo video](img/demo1.jpg)](https://www.youtube.com/watch?v=eCLCdaSIxBA)

## Getting Started

Tested with UE4 4.20 and GStreamer 1.14.4

Download and install [GStreamer binaries and dev files](https://gstreamer.freedesktop.org/download/)

Edit `Plugins\GStreamer\Source\GStreamer\GStreamer.Build.cs`

Modify `const string GStreamerRoot = @"d:\dev\gstreamer1.14.4\1.0\x86_64"` to path where GStreamer installed

Launch project and hit play

## Example pipelines

Test pattern:
`videotestsrc pattern=snow ! video/x-raw,width=(int)512,height=(int)512,format=(string)RGBA,framerate=(fraction)30/1 ! appsink name=sink`

Play a local file:
`filesrc location=d:/video/doom.mp4 ! decodebin ! videoconvert ! video/x-raw,format=(string)RGBA ! videoconvert ! appsink name=sink`

Play URI:
`uridecodebin uri=http://images.nvidia.com/geforce-com/international/videos/doom/doom-nvidia-geforce-gtx-geforce-dot-com-vulkan-graphics-api-gameplay-video.mp4 ! videoconvert ! video/x-raw,format=(string)RGBA ! videoconvert ! appsink name=sink`
