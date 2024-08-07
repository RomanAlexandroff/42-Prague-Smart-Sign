
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
- [Calculating Default Constants for the Battery Charge Function](#calculatingdefaultconstantsforthebatterychargefunction)
- [Conclusion](#conclusion)


## Introduction

Welcome to the README for the 42 Prague Smart Sign — a self-sufficient information display designed to be installed on a cluster door. Its purpose is to notify students when the cluster is reserved for an exam and prevent them from accidental entering. The device automatically retrieves exam dates from Intra and displays appropriate warnings and information on its e-paper screen. In the spare time it simply displays the cluster number.

For more information please refer to the technical documentation in this repository.


## Usage

Once the 42 Prague Smart Sign is installed and powered on, it will automatically retrieve exam dates from Intra and update the display accordingly. The device operates in the following states:
1. **Normal state**: The device displays only the number of the cluster.
2. **Pre-exam warning**: On the day of an exam, the device adds a small warning note informing about an upcoming exam later in the day. This way students who want to settle in the room may plan their work with the cluster availability limitations in mind.
3. **1-hour countdown**: 1 hour before the exam a countdown starts. The device displays a big sign with the minutes left until the exam, telling students that it is time to vacate the room.
4. **Exam in progress**: During the exam, the device displays a big bright-red warning sign indicating the ongoing exam. The drastically changed colour pallet attracts attention, increasing the sign's efficiency.
5. **Post-exam state**: After the exam ends, the device returns to its normal state, displaying the number of the cluster.

The device operates fully automatically and does not require any input from the school personnel.


## Features

- Automatic check of exam dates and time on Intra;
- Automatic battery charge monitoring;
- Deep Sleep mode: the microcontroller wakes up only when it has a task to do, drastically saving the battery charge;
- Over The Air firmware update: develop and upload new features from anywhere at school.


## Components

The following components are used in the 42 Prague Smart Sign:
1. Seeed Studio XIAO ESP32C3 Wi-Fi module: provides connectivity to the Internet for retrieving exam dates, updates the display, takes care of the battery with its battery management and charging circuit — all of that while rocking modern USB-C port for charging and easy software updating.
2. Good Display GDEY075Z08 7.5" 800x480 ePaper black/red/white SPI display: a high-resolution display that allows for clear and easy-to-read information.
3. Good Display DESPI-C02 universal SPI e-Paper adapter: transforms display's FPC interface into microcontroller's SPI.
4. 4000mAh Li-ion battery with overcharge and undercharge protection: powers the device and ensures continuous operation.


## Maintenance

The 42 Prague Smart Sign is designed with low-maintenance in mind. The battery capacity of 4000mAh ensures long periods of operation before requiring a recharge, typically once every few months. To recharge the device, it needs to be connected to a power source using any USB-C 5V power adapter. During the recharge, the Sign remains completely operational.


## License

The 42 Prague Smart Sign project is licensed under the [MIT License](https://opensource.org/licenses/MIT). You are free to use, modify, and distribute the code according to the terms and conditions of the license.

Please note that while the project strives to provide accurate information, it is provided "as is" without any warranty. Use the project at your ownrisk.


## Regards

This project is a group effort of various 42 students with support from the 42 Prague Bocal team. Here they are:
- raleksan, 42 Prague — firmware and electronic hardware developer. Made the idea reality,
- gsura, 42 Prague — idea starter and motivation supporter. Pitched the idea to Bocal so well they ended up asking for two devices,
- cgray, 42 Prague — hardware development. Modeled beautiful and functional inner frame for the electronics,
- phelebra, 42 Prague — 3D printing. Turned 3D model into high-quality pieces of hardware.
- arosado-, 42 Lisboa — 42 API expert. Tought how to get the data from the 42 servers,
- jrathelo, 42 Nice — memory wizard. Helped resolve the microcontroller's RAM overflow.

The project is based on Jean-Marc Zingg's [GxEPD2](https://github.com/ZinggJM/GxEPD2) library for e-paper displays.


## Contributing

Contributions to the 42 Prague Smart Sign project are welcome! If you have any ideas, bug fixes, or improvements, feel free to open an issue or submit a pull request on the GitHub repository.

When contributing, please adhere to the existing code style and follow the established guidelines. Clearly describe your changes and provide any necessary documentation or tests.


## Conclusion

Thank you for your interest in the 42 Prague Smart Sign project! We hope this README provides you with the necessary information to understand the project's purpose, features, installation process, usage, and maintenance. If you did not find the information you need, please, refer to the technical documentation in this repository. If you have any further questions or need assistance, please don't hesitate to reach out. Happy coding!