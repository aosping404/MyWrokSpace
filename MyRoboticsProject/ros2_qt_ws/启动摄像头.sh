#!/bin/bash
# 启动 USB 工业摄像头（高分辨率 MJPEG2RGB 格式）

echo "启动 USB 工业摄像头..."
echo "设备: /dev/video1"
echo "分辨率: 2592x1944"
echo "格式: mjpeg2rgb"
echo "帧率: 15 FPS"
echo "话题: /camera/image_raw"
echo ""

# 检查 usb_cam 是否可用
if ! ros2 pkg executables usb_cam 2>/dev/null | grep -q usb_cam_node_exe; then
    if [ -f "$HOME/usb_cam_ws/install/setup.bash" ]; then
        source $HOME/usb_cam_ws/install/setup.bash
    else
        echo "错误: usb_cam 未安装"
        exit 1
    fi
fi

ros2 run usb_cam usb_cam_node_exe --ros-args \
  -p video_device:="/dev/video1" \
  -p image_width:=2592 \
  -p image_height:=1944 \
  -p pixel_format:="mjpeg2rgb" \
  -p framerate:=15.0 \
  -r image_raw:=/camera/image_raw
