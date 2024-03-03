
# 42 Prague Smart Sign

## Table of Contents

- [Introduction](#introduction)
- [Usage](#usage)
- [Features](#features)
- [Components](#components)
- [Maintenance](#maintenance)
- [License](#license)
- [Regards](#regards)
- [Contributing](#contributing)
- [Example of the credentials.h File](#example)
- [Calculating Default Constants for the Battery Charge Function](#calculating)
- [Conclusion](#conclusion)


## Introduction

Welcome to the README for the 42 Prague Smart Sign — a self-sufficient information display designed to be installed on a cluster door. Its purpose is to notify students when the cluster is reserved for an exam and prevent them from accidental entering. The device automatically retrieves exam dates from Intra and displays appropriate warnings and information on its e-paper screen. In the spare time it simply displays the cluster number.


## Usage

Once the 42 Prague Smart Sign is installed and powered on, it will automatically retrieve exam dates from Intra and update the display accordingly. The device operates in the following states:
1. **Normal state**: The device displays only the number of the cluster.
2. **Pre-exam warning**: On the day of an exam, the device adds a small warning sign indicating an upcoming exam later in the day.
3. **1-hour countdown**: 1 hour before the exam starts a countdown. The device displays a big warning sign with the minutes left until the exam.
4. **Exam in progress**: During the exam, the device displays a big red warning sign with the exact exam end time.
4. **Last hour of exam**: The exact time changes into a countdown.
5. **Post-exam state**: After the exam ends, the device returns to its normal state, displaying the number of the cluster.
The device operates fully automatically and does not require any input from school personnel.


## Features

- Automatic check of exam dates and time on Intra;
- Automatic battery charge monitoring;
- Deep Sleep mode: the microcontroller wakes up only when it has a task to do, drastically saving the battery charge;
- Touch pad: control the device with an array of touch buttons with custom features;
- Over The Air firmware update: develop and upload new features from anywhere at school.


## Components

The following components are used in the 42 Prague Smart Sign:
1. ESP32–S2 Wi-Fi module: Provides connectivity to the internet for retrieving exam dates and updating the display.
2. 7.5'' e-paper black/red/white SPI display: A high-resolution display that allows for clear and easy-to-read information.
3. 4000mAh Li-ion battery: Powers the device and ensures continuous operation.
4. Battery management and charging circuit: Manages the battery charging process and protects it from overcharging or discharging.


## Maintenance

The 42 Prague Smart Sign is designed with low-maintenance in mind. The battery capacity of 4000mAh ensures long periods of operation before requiring a recharge, typically once every few months. To recharge the device, it needs to be connected to a power source using any microUSB 5V power adapter. During the recharge, the Sign remains completely operational.


## License

The 42 Prague Smart Sign project is licensed under the [MIT License](https://opensource.org/licenses/MIT). You are free to use, modify, and distribute the code according to the terms and conditions of the license.

Please note that while the project strives to provide accurate information, it is provided "as is" without any warranty. Use the project at your ownrisk.


## Regards

This project is a group effort of various 42 students with support from the 42 Prague Bocal team. Here they are:
- raleksan — firmware and electronic hardware developer. Made the idea reality,
- gsura — motivation supporter and graphics designer. Pitched the idea to Bocal so well they ended up asking for two devices,
- Colin — hardware development and 3D printing. Created beautiful and functional incasement for the electronics,
- arosado- — 42 API expert. Tought how to get the data from the 42 servers.

The project is based on Jean-Marc Zingg's [GxEPD2](https://github.com/ZinggJM/GxEPD2) library for e-paper displays.

The OTA updating functionality added using the native Arduino OTA library.


## Contributing

Contributions to the 42 Prague Smart Sign project are welcome! If you have any ideas, bug fixes, or improvements, feel free to open an issue or submit a pull request on the GitHub repository.

When contributing, please adhere to the existing code style and follow the established guidelines. Clearly describe your changes and provide any necessary documentation or tests.


## Example of the credentials.h File

```  
#ifndef CREDENTIALS_H
# define CREDENTIALS_H

# define SECRET          "x-xxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
# define UID             "x-xxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
# define CAMPUS_ID       "56"                // 42 Prague
# define CLUSTER_ID      "xx"                // your cluster number

# define TIME_ZONE       2                   // according to the GMT standart

# define WIFI_SSID       "wifi_name"
# define WIFI_PASSWORD   "wifi_password"

#endif
```
 
## Calculating Default Constants for the Battery Charge Function

You will need to check the following little utility:
https://github.com/RomanAlexandroff/ESP-Battery-Charge-Utility
      
It was designed to be a universal solution for battery charge level detection in ESP-based projects and has all the instructions inside of it.

You may skip this step and use the default constants provided in the header file. The battery measuremet will autocalibrate itself after a few battery charge-discharge cycles. But expect to see inaccurate readings for a while.


## Conclusion

Thank you for your interest in the 42 Prague Smart Sign project! We hope this README provides you with the necessary information to understand the project's purpose, features, installation process, usage, and maintenance. If you have any further questions or need assistance, please don't hesitate to reach out. Happy coding!