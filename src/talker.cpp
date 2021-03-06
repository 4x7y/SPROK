/*
 * Copyright (C) 2008, Morgan Quigley and Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Stanford University or Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
// %Tag(FULLTEXT)%
// %Tag(ROS_HEADER)%
#include "ros/ros.h"
// %EndTag(ROS_HEADER)%
// %Tag(MSG_HEADER)%
#include "std_msgs/String.h"
// %EndTag(MSG_HEADER)%

#include <sstream>


// PCL include file
#include <pcl/io/openni_grabber.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/console/parse.h>

#include <iostream>

using namespace std;
using namespace pcl;

PointCloud<PointXYZRGBA>::Ptr cloudptr(new PointCloud<PointXYZRGBA>); // A cloud that will store color info.
PointCloud<PointXYZ>::Ptr fallbackCloud(new PointCloud<PointXYZ>);    // A fallback cloud with just depth data.
boost::shared_ptr<visualization::CloudViewer> viewer;                 // Point cloud viewer object.
Grabber* openniGrabber;                                               // OpenNI grabber that takes data from the device.
unsigned int filesSaved = 0;                                          // For the numbering of the clouds saved to disk.
bool saveCloud(false), noColor(false);                                // Program control..

ros::Publisher chatter_pub;

void
printUsage(const char* programName)
{
        cout << "Usage: " << programName << " [options]"
                 << endl
                 << endl
                 << "Options:\n"
                 << endl
                 << "\t<none>     start capturing from an OpenNI device.\n"
                 << "\t-v FILE    visualize the given .pcd file.\n"
                 << "\t-h         shows this help.\n";
}

// This function is called every time the device has new data.
void
grabberCallback(const PointCloud<PointXYZRGBA>::ConstPtr& cloud)
{
        if (! viewer->wasStopped())
                viewer->showCloud(cloud);

        if (saveCloud)
        {
                stringstream stream;
                stream << "inputCloud" << filesSaved << ".pcd";
                string filename = stream.str();
                if (io::savePCDFileASCII(filename, *cloud) == 0)
                {
                        filesSaved++;
                        cout << "Saved " << filename << "." << endl;
                }
                else PCL_ERROR("Problem saving %s.\n", filename.c_str());

                saveCloud = false;
        

		//send ros message
		std_msgs::String msg;

   		std::stringstream ss;
    		ss << "hello world ";
    		msg.data = ss.str();
		
ROS_INFO("%s", msg.data.c_str());
// %EndTag(ROSCONSOL
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */


// %Tag(PUBLISH)%
   chatter_pub.publish(msg);
// %EndTag(PUBLISH)%

// %Tag(SPINONCE)%
    ros::spinOnce();
// %EndTag(SPINONCE)%


	}
}

// For detecting when SPACE is pressed.
void
keyboardEventOccurred(const visualization::KeyboardEvent& event,
                                          void* nothing)
{
        if (event.getKeySym() == "space" && event.keyDown())
        {
	        saveCloud = true;
	}
}

// Creates, initializes and returns a new viewer.
boost::shared_ptr<visualization::CloudViewer>
createViewer()
{
        boost::shared_ptr<visualization::CloudViewer> v
        (new visualization::CloudViewer("OpenNI viewer"));
        v->registerKeyboardCallback(keyboardEventOccurred);

        return (v);
}











/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
// %Tag(INIT)%
  ros::init(argc, argv, "talker");
// %EndTag(INIT)%

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
// %Tag(NODEHANDLE)%
  ros::NodeHandle n;
// %EndTag(NODEHANDLE)%

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
// %Tag(PUBLISHER)%
  chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
// %EndTag(PUBLISHER)%



 if (console::find_argument(argc, argv, "-h") >= 0)
        {
                printUsage(argv[0]);
                return -1;
        }

        bool justVisualize(false);
        string filename;
        if (console::find_argument(argc, argv, "-v") >= 0)
        {
                if (argc != 3)
             { 
                        printUsage(argv[0]);
                        return -1;
                }

                filename = argv[2];
                justVisualize = true;
        }
        else if (argc != 1)
        {
                printUsage(argv[0]);
                return -1;
        }

        // First mode, open and show a cloud from disk.
        if (justVisualize)
        {
                // Try with color information...
                try
                {
                        io::loadPCDFile<PointXYZRGBA>(filename.c_str(), *cloudptr);
                }
                catch (PCLException e1)
                {
                        try
                        {
                                // ...and if it fails, fall back to just depth.
                                io::loadPCDFile<PointXYZ>(filename.c_str(), *fallbackCloud);
                        }
                        catch (PCLException e2)
                        {
                                return -1;
                        }

                        noColor = true;
                }

                cout << "Loaded " << filename << "." << endl;
                if (noColor)
                        cout << "This cloud has no RGBA color information present." << endl;
                else cout << "This cloud has RGBA color information present." << endl;
        }
        // Second mode, start fetching and displaying frames from the device.
        else
        {
                openniGrabber = new OpenNIGrabber();
                if (openniGrabber == 0)
                        return -1;
                boost::function<void (const PointCloud<PointXYZRGBA>::ConstPtr&)> f =
                        boost::bind(&grabberCallback, _1);
                openniGrabber->registerCallback(f);
        }

        viewer = createViewer();

        if (justVisualize)
        {
                if (noColor)
                        viewer->showCloud(fallbackCloud);
                else viewer->showCloud(cloudptr);
        }
        else openniGrabber->start();

        // Main loop.
        while (! viewer->wasStopped())
                boost::this_thread::sleep(boost::posix_time::seconds(1));

        if (! justVisualize)
                openniGrabber->stop();



  return 0;
}
// %EndTag(FULLTEXT)%
