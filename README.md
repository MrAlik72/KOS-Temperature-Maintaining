### MQTT and control alphabot with GPIO commands

The program is based on example Mosquitto MQTT Subscriber Example

The example shows an implementation of MQTT Subscriber. 
For details please refer to the [documentation](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.1).

### Prerequisites
Before you begin, ensure that you have met the following requirements:

    You have installed the latest version of KasperskyOS Community Edition
    You have Debian GNU/Linux "Buster" 10 x64

### Usage
To build the example and run the resulting image use the cross-build.sh script.
A static IP address 10.0.2.2 is set for Mosquito broker using env variable MQTT_BROKER_ADDRESS. 
You can change the broker's address in the file env/src/env.c according to your network configuration.


To start mosquitto brocker
./mosquitto -c ./mosquitto.conf.example -v


An example of mosquitto.conf.example 
::::::::::::::
listener 1883 10.0.2.2
persistence false
persistence_location /pathto/mqtt
persistence_file mosquitto.db
log_dest syslog
log_dest stdout
log_dest topic
log_type error
log_type warning
log_type notice
log_type information
connection_messages true
log_timestamp true
allow_anonymous true
password_file /pathto/pwfile


To send command:
./mosquitto_pub -h 10.0.2.2 -t test -m "box"
./mosquitto_pub -h 10.0.2.2 -t test -m "move"
./mosquitto_pub -h 10.0.2.2 -t test -m "stop"
./mosquitto_pub -h 10.0.2.2 -t test -m "move"
./mosquitto_pub -h 10.0.2.2 -t test -m "right"
./mosquitto_pub -h 10.0.2.2 -t test -m "left"
./mosquitto_pub -h 10.0.2.2 -t test -m "left"
./mosquitto_pub -h 10.0.2.2 -t test -m "move"


