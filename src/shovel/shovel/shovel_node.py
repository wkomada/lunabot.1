import RPi.GPIO as GPIO
import rclpy
from rclpy.node import Node
from . import constants
from std_msgs.msg import String


class shovel_node(Node):

    def __init__(self):
        GPIO.setmode(GPIO.BOARD)

        # ARM LEFT
        GPIO.setup(constants.ARM_PWM_LEFT, GPIO.OUT, initial=GPIO.HIGH)
        self.arm_pwm_left = GPIO.PWM(constants.ARM_PWM_LEFT, 50)
        self.arm_pwm_left.start(0)

        # ARM RIGHT
        GPIO.setup(constants.ARM_PWM_RIGHT, GPIO.OUT, initial=GPIO.HIGH)
        self.arm_pwm_right = GPIO.PWM(constants.ARM_PWM_RIGHT, 50)
        self.arm_pwm_right.start(0)

        GPIO.setup(constants.SCOOP_PWM, GPIO.OUT, initial=GPIO.HIGH)
        self.SCOOP_PWM = GPIO.PWM(constants.SCOOP_PWM, 50)
        self.SCOOP_PWM.start(0)

        GPIO.setup(constants.BUCKET_PWM, GPIO.OUT, initial=GPIO.LOW)
        # self.bucket_pwm = GPIO.PWM(constants.BUCKET_PWM, 50)
        # self.bucket_pwm.start(0)

        GPIO.setup(constants.SCOOP_GPIO, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(constants.ARM_GPIO_LEFT, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(constants.ARM_GPIO_RIGHT, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(constants.BUCKET_GPIO, GPIO.OUT, initial=GPIO.HIGH)
        
        GPIO.output(constants.BUCKET_PWM, GPIO.LOW)



        super().__init__('shovel_node')

        self.arm_subscription = self.create_subscription(
            String,
            '/shovel/arm_cmd',
            self.arm_callback,
            10)
        self.scoop_subscription = self.create_subscription(
            String,
            '/shovel/scoop_cmd',
            self.scoop_callback,
            10)
        self.bucket_subscription = self.create_subscription(
            String,
            '/shovel/bucket_cmd',
            self.bucket_callback,
            10)

        self.bucket_subscription  # prevent unused variable warning
        self.arm_subscription  # prevent unused variable warning

    def arm_callback(self, msg):
        print(msg.data == 'f')
        if(msg.data == constants.ARM_FORWARD):
            self.arm_pwm_left.ChangeDutyCycle(constants.ARM_DUTY_CYCLE)
            self.arm_pwm_right.ChangeDutyCycle(constants.ARM_DUTY_CYCLE)
            GPIO.output(constants.ARM_GPIO_RIGHT, GPIO.HIGH)
            GPIO.output(constants.ARM_GPIO_LEFT, GPIO.HIGH)
        elif(msg.data == constants.ARM_BACKWARD):
            self.arm_pwm_left.ChangeDutyCycle(constants.ARM_DUTY_CYCLE)
            self.arm_pwm_right.ChangeDutyCycle(constants.ARM_DUTY_CYCLE)
            GPIO.output(constants.ARM_GPIO_RIGHT, GPIO.LOW)
            GPIO.output(constants.ARM_GPIO_LEFT, GPIO.LOW)
        else:
            self.arm_pwm_left.ChangeDutyCycle(0)
            self.arm_pwm_right.ChangeDutyCycle(0)

    def bucket_callback(self, msg):
        if(msg.data == constants.BUCKET_FORWARD):
            GPIO.output(constants.BUCKET_GPIO, GPIO.LOW)
            GPIO.output(constants.BUCKET_PWM, GPIO.HIGH)
        elif(msg.data == constants.BUCKET_BACKWARD):
            GPIO.output(constants.BUCKET_GPIO, GPIO.HIGH)
            GPIO.output(constants.BUCKET_PWM, GPIO.HIGH)
        else:
            GPIO.output(constants.BUCKET_PWM, GPIO.LOW)

    def scoop_callback(self, msg):
        if(msg.data == constants.SCOOP_FORWARD):
            self.SCOOP_PWM.ChangeDutyCycle(constants.SCOOP_DUTY_CYCLE)
            GPIO.output(constants.SCOOP_GPIO, GPIO.HIGH)
        elif(msg.data == constants.SCOOP_BACKWARD):
            self.SCOOP_PWM.ChangeDutyCycle(constants.SCOOP_DUTY_CYCLE)
            GPIO.output(constants.SCOOP_GPIO, GPIO.LOW)
        else:
            self.SCOOP_PWM.ChangeDutyCycle(0)

def main(args=None):
    rclpy.init(args=args)

    shovelNode = shovel_node()

    rclpy.spin(shovelNode)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    GPIO.cleanup()
    shovelNode.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
