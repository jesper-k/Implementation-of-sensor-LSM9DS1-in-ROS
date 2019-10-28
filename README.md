# LSM9DS1 for ROS Guide
In order to receive serial data from the arduino, the rosserial library is used.
[https://github.com/ros-drivers/rosserial]

Upload the given code to your Arduino and run the following command to encode/decode the Arduino output

```sh
$ rosrun rosserial_python serial_node.py <serial_device_name> _baud:=9600
```

Running

```sh
$ rostopic list
```
will then show IMUdata in the topics list.

