/*
    Copyright (c) 2010, Daniel Hewlett, Antons Rebguns
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Antons Rebguns <email> ""AS IS"" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Antons Rebguns <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
 * \file  gazebo_ros_diff_drive.cpp
 *
 * \brief A differential drive plugin for gazebo. Based on the ObjectMove plugin
 * developed for the erratic robot (see copyright notice above). The original
 * plugin can be found in the ROS package gazebo_erratic_plugins.
 *
 * \author  Piyush Khandelwal (piyushk@gmail.com)
 *
 * $ Id: 06/21/2013 11:23:40 AM piyushk $
 */


/*
 *
 * The support of acceleration limit was added by
 * \author   George Todoran <todorangrg@gmail.com>
 * \author   Markus Bader <markus.bader@tuwien.ac.at>
 * \date 22th of May 2014
 */

#include <algorithm>
#include <assert.h>

#include <gazebo_plugins/gazebo_ros_object_move.h>

#include <ignition/math/Angle.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Quaternion.hh>
#include <ignition/math/Vector3.hh>
#include <sdf/sdf.hh>

#include <ros/ros.h>

namespace gazebo
{

GazeboRosObjectMove::GazeboRosObjectMove() {}

// Destructor
GazeboRosObjectMove::~GazeboRosObjectMove() 
{
	FiniChild();
}

// Load the controller
void GazeboRosObjectMove::Load ( physics::ModelPtr _parent, sdf::ElementPtr _sdf )
{
geometry_msgs::Point pt_;
pt_.x = 0.856 ; pt_.y = 3 ; pt_.z = 0.0 ; points_map["mid_door"] = pt_;
pt_.x = 0.856 ; pt_.y = 4.4 ; pt_.z = 0.0 ; points_map["door"] = pt_; 
pt_.x = 0.856 ; pt_.y = 4.4 ; pt_.z = 5.0 ; points_map["high_door"] = pt_;
pt_.x = 0.856 ; pt_.y = 6 ; pt_.z = 0.0 ; points_map["out_door"] = pt_; 

pt_.x = 2.5146 ; pt_.y = 0.8749 ; pt_.z = 0.0 ; points_map["female"] = pt_; 
pt_.x = 3.25 ; pt_.y = 0.67646 ; pt_.z = 0.0 ; points_map["male"] = pt_;

pt_.x = 3.25 ; pt_.y = 0.33 ; pt_.z = 1.3 ; points_map["pick_male"] = pt_;
pt_.x = 2.5146 ; pt_.y = 0.65 ; pt_.z = 1.3 ; points_map["pick_female"] = pt_; 


pt_.x = 2.76 ; pt_.y = -0.467 ; pt_.z = 1 ; points_map["mid_place_pick"] = pt_; 

pt_.x = 2.25 ; pt_.y = -0.571 ; pt_.z = 1 ; points_map["mid_box_1"] = pt_;
pt_.x = 2.25 ; pt_.y = -0.571 ; pt_.z = 0.66235 ; points_map["box_1"] = pt_;

pt_.x = 2.75 ; pt_.y = -0.571 ; pt_.z = 1 ; points_map["mid_box_2"] = pt_;
pt_.x = 2.75 ; pt_.y = -0.571 ; pt_.z = 0.66235 ; points_map["box_2"] = pt_;

pt_.x = 3.293 ; pt_.y = -0.571 ; pt_.z = 1 ; points_map["mid_box_3"] = pt_; 
pt_.x = 3.293 ; pt_.y = -0.571 ; pt_.z = 0.66235 ; points_map["box_3"] = pt_; 


ROS_INFO("LOADEDD!");
    this->parent = _parent;
    gazebo_ros_ = GazeboRosPtr ( new GazeboRos ( _parent, _sdf, "ObjectMove" ) );
    gazebo_ros_->isInitialized();
    gazebo_ros_->getParameter<std::string> (object_name, "objectName", "" );
    if (!_sdf->HasElement("legacyMode"))
    {
      ROS_ERROR_NAMED("diff_drive", "GazeboRosObjectMove Plugin missing <legacyMode>, defaults to true\n"
	       "This setting assumes you have a old package, where the right and left wheel are changed to fix a former code issue\n"
	       "To get rid of this error just set <legacyMode> to false if you just created a new package.\n"
	       "To fix an old package you have to exchange left wheel by the right wheel.\n"
	       "If you do not want to fix this issue in an old package or your z axis points down instead of the ROS standard defined in REP 103\n"
	       "just set <legacyMode> to true.\n"
      );
    }

    // Initialize update rate stuff
    if ( this->update_rate_ > 0.0 ) this->update_period_ = 1.0 / this->update_rate_;
    else this->update_period_ = 0.0;
#if GAZEBO_MAJOR_VERSION >= 8
    last_update_time_ = parent->GetWorld()->SimTime();
#else
    last_update_time_ = parent->GetWorld()->GetSimTime();
#endif
   // start custom queue for object move 
    this->callback_queue_thread_ = boost::thread ( boost::bind ( &GazeboRosObjectMove::QueueThread, this ) );

    // listen to the update event (broadcast every simulation iteration)
    this->update_connection_ =  event::Events::ConnectWorldUpdateBegin ( boost::bind ( &GazeboRosObjectMove::UpdateChild, this ) );
    obj_position_client = gazebo_ros_->node()->serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
    event_pub = gazebo_ros_->node()->advertise<gazebo_event_controller::EventPublisher>("/experiments_event", 1);
    this->my_service = gazebo_ros_->node()->advertiseService("/event_service_"+object_name, &GazeboRosObjectMove::ServiceCallback, this);
    //my_service = nh.advertiseService("/event_service_"+object_name, &gazebo::GazeboRosObjectMove::ServiceCallback, this);
ROS_INFO("End loaded");
}

void GazeboRosObjectMove::Reset()
{
#if GAZEBO_MAJOR_VERSION >= 8
  last_update_time_ = parent->GetWorld()->SimTime();
#else
  last_update_time_ = parent->GetWorld()->GetSimTime();
#endif
}

// Update the controller
void GazeboRosObjectMove::UpdateChild()
{


    //if ( odom_source_ == ENCODER ) UpdateOdometryEncoder();
#if GAZEBO_MAJOR_VERSION >= 8
    common::Time current_time = parent->GetWorld()->SimTime();
#else
    common::Time current_time = parent->GetWorld()->GetSimTime();
#endif
    double seconds_since_last_update = ( current_time - last_update_time_ ).Double();

    if ( seconds_since_last_update > update_period_ ) {
        //if (this->publish_tf_) publishOdometry ( seconds_since_last_update );
        //if ( publishWheelTF_ ) publishWheelTF();
        //if ( publishWheelJointState_ ) publishWheelJointState();

        // Update robot in case new velocities have been requested
        //getWheelVelocities();


        last_update_time_+= common::Time ( update_period_ );
    }
}

void GazeboRosObjectMove::Animate(const std::vector<geometry_msgs::Point> &points)
{
    // name the animation "test",
    // make it last 10 seconds,
    // and set it on a repeat loop
    gazebo::common::PoseAnimationPtr anim(new gazebo::common::PoseAnimation("test", 10.0, false));
    gazebo::common::PoseKeyFrame *key;
    // set starting location of the box
    //key = anim->CreateKeyFrame(0);
    //key->Translation(ignition::math::Vector3d(0, 0, 0));
    //key->Rotation(ignition::math::Quaterniond(0, 0, 0));

    // set waypoint location after 2 seconds
    double time = 0.0; 
    for(auto point_ : points)
    {
        key = anim->CreateKeyFrame(time++);
        key->Translation(ignition::math::Vector3d(point_.x, point_.y, point_.z));
        key->Rotation(ignition::math::Quaterniond(0, 0, 0));
    }


    // set the animation
    this->parent->SetAnimation(anim);

}

// Finalize the controller
void GazeboRosObjectMove::FiniChild()
{
    alive_ = false;
    queue_.clear();
    queue_.disable();
    gazebo_ros_->node()->shutdown();
    callback_queue_thread_.join();
}




void GazeboRosObjectMove::QueueThread()
{
    static const double timeout = 0.01;

    while ( alive_ && gazebo_ros_->node()->ok() ) {
        queue_.callAvailable ( ros::WallDuration ( timeout ) );
    }
}

bool GazeboRosObjectMove::ServiceCallback(gazebo_event_controller::EventCaller::Request  &req,
        gazebo_event_controller::EventCaller::Response &res)
{
    std::vector<geometry_msgs::Point> points; 
    if(req.event == "pick")
    {
            points.push_back(points_map["mid_place_pick"]);
            points.push_back(points_map["pick_"+req.agent]);
            Animate(points);
            res.status = true;
            //return true; 
    }
    else if(req.event == "place")
    {
        points.push_back(points_map["pick_"+req.agent]);
        points.push_back(points_map["mid_place_pick"]);
        points.push_back(points_map["mid_"+req.place]);
        points.push_back(points_map[req.place]);
        Animate(points);
        res.status = true;
        //return true; 
    }
    else if(req.event == "enter")
    {
        points.push_back(points_map["out_door"]);
        points.push_back(points_map["high_door"]);
        points.push_back(points_map["door"]);
        points.push_back(points_map["mid_door"]);
        points.push_back(points_map[req.agent]);
        Animate(points);
        res.status = true;
        //return true; 
    }
    else if(req.event == "leave")
    {
        points.push_back(points_map[req.agent]);
        points.push_back(points_map["mid_door"]);
        points.push_back(points_map["door"]);
        points.push_back(points_map["high_door"]);
        points.push_back(points_map["out_door"]);
        Animate(points);
        res.status = true;
        //return true; 
    }
    res.status = true; 
    ROS_INFO("Getting service calls");
    gazebo_event_controller::EventPublisher event_msg; 
    event_msg.stamp = ros::Time::now();
    event_msg.object = req.object;  
    event_msg.agent = req.agent; 
    event_msg.event = req.event; 
    event_msg.place = req.place;
    if(res.status)
    {
        event_pub.publish(event_msg);
        return true;
    }
    else
        return false;

    
}





GZ_REGISTER_MODEL_PLUGIN ( GazeboRosObjectMove )
}
