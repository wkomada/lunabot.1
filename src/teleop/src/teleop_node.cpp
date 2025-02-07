#include <memory>

#include <cstdio>
#include <iostream>
#include <joybuttons.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "string"
#include "constants.h"
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
      drivetrainPub = this->create_publisher<sensor_msgs::msg::JointState>("drivetrain_velocity", 10);

      RCLCPP_INFO(get_logger(), "\033[1;35mMANUAL CONTROL:\033[0m \033[1;32mENABLED\033[0m");
    }

  private:
    void topic_callback(const sensor_msgs::msg::Joy &raw)
    {
        sensor_msgs::msg::JointState drivetrain_states;
	drivetrain_states.velocity.resize(4);
	drivetrain_states.velocity[0] = 0;
	drivetrain_states.velocity[1] = 0;
	drivetrain_states.velocity[2] = 0;
	drivetrain_states.velocity[3] = 0;

        if(raw.buttons[BUTTON_LBUMPER]){
            cout << "This is working!" << endl;
            drivetrain_states.velocity[0] = (raw.axes[AXIS_LEFTY])*MOTOR_MAX;
            drivetrain_states.velocity[3] = (raw.axes[AXIS_LEFTY])*MOTOR_MAX;
            drivetrain_states.velocity[1] = (raw.axes[AXIS_RIGHTY])*MOTOR_MAX;
            drivetrain_states.velocity[2] = (raw.axes[AXIS_RIGHTY])*MOTOR_MAX;
        }
        drivetrainPub->publish(drivetrain_states);
    }
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr drivetrainPub;


};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Teleop>());
  rclcpp::shutdown();
  return 0;
}
