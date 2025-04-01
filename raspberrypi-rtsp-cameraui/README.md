# Honebridge Camera UI integration 

## Hardware Setup and Camera Test

It is possible to do all these steps remotely over an SSH connection. On Windows Command Prompt, use the following command (assuming the SSH server is running on your SBC):

```sh
ssh (user-name)@192.168.1.133
```

_(This is just an example; replace it with your SBC's actual IP and username.)_

### Update Packages and Dependencies

First, ensure all your packages and dependencies are up to date:

```sh
sudo apt update && sudo apt upgrade
```

### Enable Camera on Raspberry Pi (Raspberry Pi OS - Bullseye)

If you are using Raspberry Pi OS (Bullseye), enable the camera peripheral by running:

```sh
sudo raspi-config
```

Navigate to:
- **Interface Options** → **Legacy Camera** → **Enable**

### Verify Camera Detection

Connect your camera to the SBC **while it is powered off**. Then, check if the camera is detected:

```sh
vcgencmd get_camera
```

If everything is set up correctly, you should see:

```sh
supported=1 detected=1
```

Otherwise, you may need to troubleshoot.

### Capture a Test Image

To test the camera by taking a photo:

```sh
raspistill -o test.jpg
```

The captured image will be stored at:

```sh
/home/(user-name)/test.jpg
```

To transfer `test.jpg` to your host machine on Windows, use the `scp` command:

```sh
scp user-name@192.168.1.112:/home/user-name/test.jpg "C:\Users\user-name\Downloads\"
```

---

## RTSP Server Setup

### Clone and Compile RTSP Server

```sh
git clone https://github.com/mpromonet/v4l2rtspserver.git
cd v4l2rtspserver
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

**Explanation:**
1. `cmake ..` - Generates necessary build files.
2. `make -j$(nproc)` - Compiles the project using multiple CPU cores.
3. `sudo make install` - Installs the compiled files.

### Load Camera Driver

To allow the RTSP server to use the camera:

```sh
sudo modprobe bcm2835-v4l2
```

Since this must be done after every reboot, make it persistent:

```sh
echo "bcm2835-v4l2" | sudo tee -a /etc/modules
```

**Breakdown:**
- `echo "bcm2835-v4l2"` - Outputs the text.
- `|` - Pipes it to the next command.
- `sudo tee -a /etc/modules` - Appends the text to `/etc/modules`.

### Start RTSP Streaming

```sh
v4l2rtspserver -P 8555 -m -W 1280 -H 720 -F 30 /dev/video0
```

**Parameters:**
- `-m` → Multicast streaming.
- `-P 8555` → Streaming port.
- `-W 1280` → Width.
- `-H 720` → Height.
- `-F 30` → 30 FPS.
- `/dev/video0` → Video device.

If everything works, the stream will be available at:

```
rtsp://192.168.1.109:8555/multicast
```

_(Replace with your Raspberry Pi's actual IP.)_

### Viewing RTSP Stream in VLC

1. Open **VLC Media Player**.
2. Go to **Media** → **Open Network Stream**.
3. Enter the RTSP URL.
4. Click **Play**.

### Auto-Start RTSP Server on Boot

To automatically start the RTSP server after each boot:

1. Copy the service file:

    ```sh
    sudo cp rtsp-stream.service /etc/systemd/system/
    ```

2. Enable and start the service:

    ```sh
    sudo systemctl enable rtsp-stream.service
    sudo systemctl start rtsp-stream.service
    ```

---

## Homebridge Camera UI Integration

_(Add details here if needed.)_
