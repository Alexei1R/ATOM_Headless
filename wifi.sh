#!/bin/bash

# Define the SSID and password for the Wi-Fi hotspot
SSID="AtomCar"
PASSWORD="android12"

# Activate the Wi-Fi hotspot
sudo nmcli device wifi hotspot ssid "$SSID" password "$PASSWORD"

