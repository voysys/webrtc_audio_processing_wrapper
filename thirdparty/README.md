# How to update webrtc-audio-processing on Windows

- Run build.bat using "x64 Native Tools Command Prompt for VS"
- Remove thirdparty/include/windows/webrtc-audio-processing
- Copy: artifacts/webrtc/include/webrtc-audio-processing/* to thirdparty/include/windows/webrtc-audio-processing/webrtc
- Copy: artifacts/absl/include/absl/ to thirdparty/include/windows/webrtc-audio-processing/absl
- Copy: artifacts/lib/webrtc-audio-processing.a to thirdparty/lib/windows/webrtc-audio-processing.a
