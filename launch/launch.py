import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import (
    IncludeLaunchDescription,
    DeclareLaunchArgument,
    TimerAction,
    GroupAction,
)


def generate_launch_description():    
    controller_teleop_node = Node(
        package="teleop",
        executable="teleop_node",
    )
 #   foxglove_studio = ExecuteProcess(cmd=["foxglove-studio"])

    canable_start_process = ExecuteProcess(
        cmd=[
            "bash",
            "-c",
            "sudo /lunabot/lunabot/lunabot/src/drivetrain/canable_start.sh",
            "can0"
        ],
        output="screen",
    )

    drivetrain_node = Node(
        package="drivetrain",
        executable="drivetrain_node",
    )



    ld = LaunchDescription()

    ld.add_action(canable_start_process)
    ld.add_action(controller_teleop_node)
    ld.add_action(drivetrain_node)

    return ld
