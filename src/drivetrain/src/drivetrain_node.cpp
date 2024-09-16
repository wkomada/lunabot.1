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
      "drivetrain velocity", 10, std::bind(&Drivetrain::topic_callback, this, _1));
      motors[0].SetInverted(false);
      motors[2].SetInverted(false);
    }

  private:
    SparkMax motors[4] = {SparkMax("can0", 1), SparkMax("can0", 2), SparkMax("can0", 3), SparkMax("can0", 4)};    
    std::string locations[4] = {"Back Left", "Back Right", "Front Left", "Front Right"};
    void topic_callback(const sensor_msgs::msg::JointState &drivetrain_states)
    {
      cout << "This is working!" << endl;
      SparkMax::Heartbeat();
      motors[0].SetVelocity(drivetrain_states.velocity[0]);
      motors[3].SetVelocity(drivetrain_states.velocity[3]);
      motors[1].SetVelocity(drivetrain_states.velocity[1]);
      motors[2].SetVelocity(drivetrain_states.velocity[2]);
      //for(int i = 0; i <=2; i+=2){
        //motors[i].SetDutyCycle((raw.axes[AXIS_LEFTY] + raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
	//motors[i].SetDutyCycle(0.1);
      //}
      //for(int i = 1; i <=3; i+=2){
        //motors[i].SetDutyCycle((raw.axes[AXIS_LEFTY] - raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
	//motors[i].SetVelocity(-1);
      //}
    }
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Drivetrain>());
  rclcpp::shutdown();
  return 0;
}
