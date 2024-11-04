FROM althack/ros2:humble-dev

WORKDIR /lunabot

RUN sudo apt update \
        && sudo apt install -y ros-humble-turtlesim \
        && sudo apt install -y ros-humble-rqt*

RUN sudo mkdir /run/user/1000 && sudo chmod 0700 /run/user/1000 && sudo chown 1000 /run/user/1000

RUN sudo apt install -y iputils-ping
# RUN sudo apt install -y ros-humble-teleop-twist-joy
RUN sudo apt install -y ros-humble-joy

# Set up auto-source of workspace for ros user
ARG WORKSPACE
RUN echo "if [ -f ${WORKSPACE}/install/setup.bash ]; then source ${WORKSPACE}/install/setup.bash; fi" >> /home/ros/.bashrc
RUN echo "ROS_DOMAIN_ID=1" >> /home/ros/.bashrc