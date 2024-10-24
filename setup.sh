#!/bin/bash
set -e

sudo apt update -y
rosdep update --rosdistro eloquent
rosdep install -i --from-paths src --rosdistro eloquent
