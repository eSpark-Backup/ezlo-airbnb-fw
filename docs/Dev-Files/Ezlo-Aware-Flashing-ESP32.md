### Software Dependencies
1. ESP32 Flash Tools (Download latest version from link attached)
https://www.espressif.com/en/support/download/other-tools
2. Termite or Putty for Serial Logs (optional)

### Instructions
1. Connect USB Type C to the USB connector on the Ezlo Aware.
2. Go to Device Manager and observe the COM Port on which Ezlo Aware is connected.
3. Open ESP32 flash tools and upload binaries as per the instructions in info.json file. Select the right COM Port as observed in Step 2. Set baudrate to 921600. Click start to start the upload.
4. Once finished, on-board the Ezlo aware using MiOS iOS application.

### NOTES:
1. Please ensure to remove any offline controllers and devices before on boarding via the mobile app.
2. Please restart the app if the devices don't appear.
3. The particle sensor received in new batch has wires reversed (the old batch had correct wires) so it is necessary to flip the wires before it will start reporting data.