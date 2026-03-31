# RPi-MQTT-Bridge
Post current system usage of a Raspberry Pi to a local webserver. More or less a online Task Manager

`sudo apt install -y nlohmann-json3-dev`
`sudo apt install -y mosquitto mosquitto-clients`
`sudo systemctl enable mosquitto`
`sudo systemctl start mosquitto`
`mosquitto_sub -h localhost -t "telemetry" -v`

# Launching the App
### Via python
- needs init.d to auto start app

### Permanent webserver
- only has to be installed once and handles most stuff

# Mosquitto MQTT
### Config file
``` ```

# Firewall
`sudo ufw allow 9001` This is the port we are streaming the MQTT


# Dasboard
Uses svelte kit, tailwindcss and sveltekit icons