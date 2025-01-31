#include <memory>

#include <cstdio>
#include <iostream>
#include <joybuttons.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
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
      subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&Drivetrain::topic_callback, this, _1));
      motors[0].SetInverted(false);
      motors[2].SetInverted(false);
    }

  private:
    SparkMax motors[4] = {SparkMax("can0", 1), SparkMax("can0", 2), SparkMax("can0", 3), SparkMax("can0", 4)};    
    std::string locations[4] = {"Back Left", "Back Right", "Front Left", "Front Right"};
    void topic_callback(const sensor_msgs::msg::Joy &raw)
    {
	cout << "This is working!" << endl;
	SparkMax::Heartbeat();
	motors[0].SetDutyCycle((raw.axes[AXIS_LEFTY])*MOTOR_MAX);
	motors[3].SetDutyCycle((raw.axes[AXIS_LEFTY])*MOTOR_MAX);
	motors[1].SetDutyCycle((raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
	motors[2].SetDutyCycle((raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
      //for(int i = 0; i <=2; i+=2){
        //motors[i].SetDutyCycle((raw.axes[AXIS_LEFTY] + raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
	//motors[i].SetDutyCycle(0.1);
      //}
      //for(int i = 1; i <=3; i+=2){
        //motors[i].SetDutyCycle((raw.axes[AXIS_LEFTY] - raw.axes[AXIS_RIGHTY])*MOTOR_MAX);
	//motors[i].SetVelocity(-1);
      //}
    }
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Drivetrain>());
  rclcpp::shutdown();
  return 0;
}
