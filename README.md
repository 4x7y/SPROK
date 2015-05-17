## SPROK

###### SLAM + PCL + ROS + OpenNI + Kinect

### Setup Environment

配置环境:

* System: Ubuntu 12.04 amd64
* Kinect: Kinect XBox 360
* ROS: ROS Hydro
* OpenNI: v1.5.7.10
* NiTE: v1.5.2.23
* SensorKinect: v5.1.2.1
* Java: Oracle JDK 1.8.0_45
* PCL: v1.7.2

<em>注意：在实体机中配置可以正常使用，但是在Virtual Box虚拟机中运行NiViewer测试程序会提示 Error: A timeout has occurred when waiting for new data!</em>

首先

	cd ~
	mkdir Kinect
	cd Kinect

之后下载


 * OpenNI SDK v1.5.7.10
 * NiTE v1.5.2.23

<a href="http://openni.ru/openni-sdk/openni-sdk-history-2/index.html">Previous OpenNI Version</a>:

	wget http://openni.ru/wp-content/uploads/2013/11/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10.tar.zip
	wget http://openni.ru/wp-content/uploads/2013/10/NITE-Bin-Linux-x64-v1.5.2.23.tar.zip

前往 <a href="https://github.com/avin2/SensorKinect/downloads">Avin2/SensorKinect</a> 下载 SensorKinect v5.1.2.1

解压

	unzip Sensor-Bin-Linux-x64-v5.1.2.1.tar.zip
	unzip OpenNI-Bin-Dev-Linux-x64-v1.5.7.10.tar.zip
	unzip NITE-Bin-Linux-x64-v1.5.2.23.tar.zip

	tar -xjvf Sensor-Bin-Linux-x64-v5.1.2.1.tar.bz2
	tar -xjvf OpenNI-Bin-Dev-Linux-x64-v1.5.7.10.tar.bz2
	tar -xjvf NITE-Bin-Linux-x64-v1.5.2.23.tar.bz2

安装

	cd OpenNI-Bin-Dev-Linux-x64-v1.5.7.10
	sudo ./install.sh

	cd ../Sensor-Bin-Linux-x64-v5.1.2.1
	sudo ./install.sh

	cd ../NITE-Bin-Dev-Linux-x64-v1.5.2.23
	sudo ./install.sh

	cd ../

还要安装

	sudo apt-get install libusb-1.0-0-dev
	sudo apt-get install freeglut3-dev

接入Kinect

	lsusb

In Ubuntu 12.04 the gspca kernel driver prevent libfreenect from claiming the Kinect device in user-mode. Either remove and blacklist the module

	sudo modprobe -r gspca_kinect 
	sudo modprobe -r gspca_main
	echo "blacklist gspca_kinect" |sudo tee -a /etc/modprobe.d/blacklist.conf

测试

	cd ./OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Samples/Bin/x64-Release
	./NiViewer

安装 ROS Hydro，参考<a href="http://wiki.ros.org/hydro/Installation">这里</a>

	sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu precise main" /etc/apt/sources.list.d/ros-latest.list'

	wget https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -O - | sudo apt-key add -

	sudo apt-get update
	sudo apt-get install ros-hydro-desktop-full

	sudo rosdep init
	rosdep update

	echo "source /opt/ros/hydro/setup.bash"  ~/.bashrc
	source ~/.bashrc

	sudo apt-get install python-rosinstall

安装 Openni_camera， 参考<a href="http://wiki.ros.org/openni_camera">这里</a>
	
	sudo apt-get install ros-hydro-openni-camera

安装 Openni_launch

	sudo apt-get install ros-hydro-openni-launch

运行 Openni_launch

	roslaunch openni_launch openni.launch

测试 ros 与 Kinect 是否连接

	rosrun rviz rivz
