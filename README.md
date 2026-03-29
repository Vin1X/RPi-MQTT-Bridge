# RPi-MQTT-Bridge
Post current system usage of a Raspberry Pi to a local webserver. More or less a online Task Manager

`sudo apt install -y nlohmann-json3-dev`
`sudo apt install -y mosquitto mosquitto-clients`
`sudo systemctl enable mosquitto`
`sudo systemctl start mosquitto`
`mosquitto_sub -h localhost -t "telemetry" -v`