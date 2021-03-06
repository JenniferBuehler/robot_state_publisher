/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Wim Meeussen */

#ifndef JOINT_STATE_LISTENER_H
#define JOINT_STATE_LISTENER_H

#include <urdf/model.h>
#include <kdl/tree.hpp>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include "robot_state_publisher/robot_state_publisher.h"

using namespace std;
using namespace ros;
using namespace KDL;

typedef boost::shared_ptr<sensor_msgs::JointState const> JointStateConstPtr;
typedef std::map<std::string, boost::shared_ptr<urdf::JointMimic> > MimicMap;

namespace robot_state_publisher{

class JointStateListener{
public:
  /** Constructor
   * \param tree The kinematic model of a robot, represented by a KDL Tree
   */
  JointStateListener(const KDL::Tree& tree, const MimicMap& m, const urdf::Model& model = urdf::Model());
  
  /** Constructor which additionally takes a RobotStatePublisher object as parameter.
   * \param tree The kinematic model of a robot, represented by a KDL Tree
   * \param state_publisher_prototype a template object of the RobotStatePublisher class to use. 
   * This will be used to locally create a RobotStatePublisher object of the same type using
   * RobotStatePublisher::create(const KDL::Tree&,const urdf::Model&). 
   */
  JointStateListener(robot_state_publisher::RobotStatePublisher& state_publisher_prototype, 
           const KDL::Tree& tree, const MimicMap& m, const urdf::Model& model = urdf::Model());

  /// Destructor
  virtual ~JointStateListener();

protected:

  /**
   * Callback function for publishing joint states. To publish, the local RobotStatePublisher object
   * should be used, which can be obtained with getRobotStatePublisher().
   * The behaviour of this methods may be changed by subclasses.
   */
  virtual void handleJointState(const JointStateConstPtr& state);
  
  /**
   * Callback function for publishing the fixed transforms using the state_publisher_. 
   * Behaviour can be changed by subclasses.
   */
  virtual void handleFixedJoint(const ros::TimerEvent& e);
  
  /**
   * Returns the RobotStatePublisher which should be used to publish updates.
   */
  robot_state_publisher::RobotStatePublisher& getRobotStatePublisher();

  /**
   * Returns the mimic map which was initialised in the constructor.
   */
  MimicMap& getMimicMap();

private:
  void callbackJointState(const JointStateConstPtr& state);
  void callbackFixedJoint(const ros::TimerEvent& e);

  void initFromParams();

  std::string tf_prefix_;
  Duration publish_interval_;
  Subscriber joint_state_sub_;
  ros::Timer timer_;
  ros::Time last_callback_time_;
  std::map<std::string, ros::Time> last_publish_time_;
  MimicMap mimic_;
  bool use_tf_static_;
  
  robot_state_publisher::RobotStatePublisher state_publisher_;

};
}


#endif
