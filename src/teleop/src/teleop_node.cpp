#include <memory>

#include <cstdio>
#include <iostream>
#include <joybuttons.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "string"
#include "constants.h"
#include "std_msgs/msg/bool.hpp"
#include "interfaces/srv/set_teleop.hpp"


using std::placeholders::_1;
using namespace std;

class Teleop : public rclcpp::Node
{
 public:
    Teleop()
    : Node("Teleop")
    {
      subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&Teleop::topic_callback, this, _1));
      drivetrainPub = this->create_publisher<sensor_msgs::msg::JointState>("drivetrain_cmd_vel", 10);
      service = this->create_service<interfaces::srv::SetTeleop>("set_teleop", [this](const std::shared_ptr<interfaces::srv::SetTeleop::Request> request,
           std::shared_ptr<interfaces::srv::SetTeleop::Response> response) {
      this->set_teleop(request, response);
    });
    }

  private:
    void topic_callback(const sensor_msgs::msg::Joy &raw)
    {
        sensor_msgs::msg::JointState drivetrain_states;
	drivetrain_states.velocity.resize(4);
	drivetrain_states.velocity[0] = 0;
	drivetrain_states.velocity[2] = 0;
	drivetrain_states.velocity[1] = 0;
	drivetrain_states.velocity[3] = 0;
	if(raw.buttons[BUTTON_RBUMPER]){
		teleopEnabled = true;
	}
        if(teleopEnabled){
            cout << "This is working!" << endl;
            if (!raw.buttons[BUTTON_LBUMPER]) {
              drivetrain_states.velocity[0] = (raw.axes[AXIS_LEFTY])*MOTOR_MAX;
              drivetrain_states.velocity[2] = (raw.axes[AXIS_LEFTY])*MOTOR_MAX;
              drivetrain_states.velocity[1] = (raw.axes[AXIS_RIGHTY])*MOTOR_MAX;
              drivetrain_states.velocity[3] = (raw.axes[AXIS_RIGHTY])*MOTOR_MAX;
              cout << "Normal speed" << endl;
            }
            else
            {
              drivetrain_states.velocity[0] = (raw.axes[AXIS_LEFTY])*ARHAN_MODE * MOTOR_MAX;
              drivetrain_states.velocity[2] = (raw.axes[AXIS_LEFTY])*ARHAN_MODE * MOTOR_MAX;
              drivetrain_states.velocity[1] = (raw.axes[AXIS_RIGHTY])*ARHAN_MODE * MOTOR_MAX;
              drivetrain_states.velocity[3] = (raw.axes[AXIS_RIGHTY])*ARHAN_MODE * MOTOR_MAX;
              cout << "Arhan Mode engaged" << endl;
            }
        }
        drivetrainPub->publish(drivetrain_states);
    }
    void set_teleop(const std::shared_ptr<interfaces::srv::SetTeleop::Request> request, std::shared_ptr<interfaces::srv::SetTeleop::Response> response){
    	string enabled;
	teleopEnabled = request->teleop_enabled;
	if(teleopEnabled){
      		RCLCPP_INFO(get_logger(), "\033[1;35mMANUAL CONTROL:\033[0m \033[1;32mENABLED\033[0m");
		enabled = "enabled";
	}else{
      		RCLCPP_INFO(get_logger(), "\033[1;35mMANUAL CONTROL:\033[0m \033[1;32mDISABLED\033[0m");
		enabled = "disabled";
	}
	response->message = "Teleop: " + enabled;
	response->success = true;
    }
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr drivetrainPub;
    rclcpp::Service<interfaces::srv::SetTeleop>::SharedPtr service;
    bool teleopEnabled = false;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Teleop>());
  rclcpp::shutdown();
  return 0;
}
