#include <memory>
#include <cstdio>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "string"
#include "SparkMax.hpp"
#include "constants.h"
using std::placeholders::_1;
using namespace std;

class Drivetrain : public rclcpp::Node
{
 public:
    Drivetrain()
    : Node("Drivetrain")
    {
      subscription_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "drivetrain_cmd_vel", 10, std::bind(&Drivetrain::topic_callback, this, _1));
      sensorPub = this->create_publisher<sensor_msgs::msg::JointState>("drivetrain_sensor_output", 10);

      motors[0].SetInverted(false);
      motors[1].SetInverted(true);
      motors[2].SetInverted(true);
      motors[3].SetInverted(false);
    }

  private:
    SparkMax motors[4] = {SparkMax("can0", 1), SparkMax("can0", 2), SparkMax("can0", 3), SparkMax("can0", 4)};    
    std::string locations[4] = {"Back Left", "Back Right", "Front Left", "Front Right"};
    void topic_callback(const sensor_msgs::msg::JointState &drivetrain_states)
    {
      //set motor values
      SparkMax::Heartbeat();
      motors[0].SetVoltage(drivetrain_states.velocity[0]);
      motors[3].SetVoltage(drivetrain_states.velocity[3]);
      motors[1].SetVoltage(drivetrain_states.velocity[1]);
      motors[2].SetVoltage(drivetrain_states.velocity[2]);
      
      //publlish sensor data
      sensor_msgs::msg::JointState motor_states;
      motor_states.name.resize(4);
      motor_states.velocity.resize(4);
      motor_states.position.resize(4);
      motor_states.effort.resize(4);
      for(int i = 0; i < 4; i++){
      	motor_states.name[i] = locations[i];
	motor_states.velocity[i] = motors[i].GetVelocity();
	motor_states.position[i] = motors[i].GetPosition();
	motor_states.effort[i] = motors[i].GetVoltage();
      }
      sensorPub->publish(motor_states);
    }
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr sensorPub;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Drivetrain>());
  rclcpp::shutdown();
  return 0;
}
