## Hardware Setup and Camera Test

It is possible to do all these steps like me over SSH connection remotely 
on windows command prompt use the following command. of SSH server should be running on your SBC 
ssh (user-name)@192.168.1.133 	//This is just an example use your own SBC IP and user name
First of all make all your packages and dependencies are up to date 

sudo apt update 

After that

sudo apt upgrade 

If you are on raspberry pi on Rasbian Bullseye OS you need to enable camera peripheral using

sudo raspi-config 

to open raspberry pi software configuration tool 

Interface Options -> Legacy Camera Enable

After connecting your camera to your SBC while it is not powered. Use this command

vcgencmd get_camera

to make sure that your camera is detected by the raspberry pi so if everything is ok you are supposed to get thus message 

supported=1 detected=1

otherwise you need to start troubleshooting

It is possible to test the camera by taking a photo using the following command 
raspistill -o test.jpg

The SBC will take a picture using the camera and store the captured frame at the following directory 
/home/(user-name)

It is possible to view test.jpg by transfering test.jpg file to your host machine. to do it on windows open powershell and execute scp command the follwoing way 

scp user-name@192.168.1.112:/home/user-name/test.jpg "C:\Users\user-name\Downloads\" 	// or any directory 

## RTSP Server Setup

git clone https://github.com/mpromonet/v4l2rtspserver.git		//Download rtsp sever project source code
cd v4l2rtspserver												//Enter downloaded file directory 
mkdir build														//create build directory for compilation convinent 
cd build														//Enter created directory
cmake ..														//Is used in the CMake build system to generate the necessary build files for compiling a project
make -j$(nproc)													//This compiles the project using multiple CPU cores for faster execution.
sudo make install												//Copies the compiled program and related files (like libraries, config files) to system directories 

Note that  CMake command does the follwoing 
1- searches for a CMakeLists.txt file in the parent directory (..).
2- creates build system files (e.g., Makefile for GNU Make) based on the project's configuration.
3- checks dependencies, sets compiler flags, and prepares the environment for make.

Now we need to load the camera driver and we can do that be using modprobe command. This is necessary to allow our RTSP server use the camera for streaming  

sudo modprobe bcm2835-v4l2

This step has to be done everytime the raspberry pi boots up and to make camera driver load operation perminant we nee to insert it to /etc/modules directory and we can do that using the following command combination.

echo "bcm2835-v4l2" | sudo tee -a /etc/modules

let us break down this command:


1- echo "bcm2835-v4l2" -> Prints the text bcm2835-v4l2 to standard output.

2- | (Pipe) -> Passes the output of echo to the next command.

3- sudo tee -a /etc/modules ->

		* tee writes the input to the file and displays it on the terminal.

        * -a (append mode) ensures it adds bcm2835-v4l2 to /etc/modules without overwriting existing content.

        * sudo ensures you have the necessary permissions.


After completing this step we are able to start our RTSP server so we can start video streaming

v4l2rtspserver -P 8555 -m -W 1280 -H 720 -F 30 /dev/video0

0- -m -> stream as multicast so several devices can listen to the same streaming

1- -p 8555 -> specify the server streaming port

2- -W 1280 -> Set width to 1280 pixels

3- -H 720 -> Set height to 720 pixels

4- -F 30 -> Set framerate to 30 FPS

5- /dev/video0 → The default video device

Note that if -m is not passed the streaming will be unicast and one signle device will be able to listen to the streaming 

If this command runs smoothly, congradulation ! video streaming has started and you can watch the video streaming over the local network  using the follwoing URL

rtsp://192.168.1.109:8555/multicast			// Exampe ip check out your raspberry pi IP from your router.

It is a good practice to have a reserved IP for your raspberry pi so you can always access your streaming while your URL does not change  

Use VLC program to watch your video streaming. Open VLC then Media -> Open Network Stream then go to net work and paste your URL and then hit Play.

Notice that v4l2rtspserver cammand must be executed after every raspberry device boot so if everything is working properly we can add a new system service refer to rtsp-stream.service file and place it in /etc/systemd/system/ directory using 

sudo cp rtsp-stream.service /etc/systemd/system/

Or use scp command if your are copyithe service file from your host machine. 
Now use these commands to get your service running 

sudo systemctl enable rtsp-stream.service
sudo systemctl start rtsp-stream.service
## Honebridge Camera UI integration 