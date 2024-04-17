#!/bin/bash

# Define the path to the service unit file
SERVICE_FILE="/etc/systemd/system/wifi-hotspot.service"

# Define the path to the Wi-Fi script
WIFI_SCRIPT="/home/toor/Code/wifi.sh"

# Create the service unit file
cat <<EOF | sudo tee "$SERVICE_FILE" > /dev/null
[Unit]
Description=Wi-Fi Hotspot Startup Script
Requires=NetworkManager.service
After=NetworkManager.service

[Service]
ExecStartPre=/bin/sleep 10
ExecStart=/bin/bash -x $WIFI_SCRIPT > /var/log/wifi-hotspot.log 2>&1

[Install]
WantedBy=multi-user.target
EOF

# Reload systemd to apply the changes
sudo systemctl daemon-reload

# Enable and start the service
sudo systemctl enable wifi-hotspot.service
sudo systemctl start wifi-hotspot.service

echo "Wi-Fi hotspot service created and started successfully."

