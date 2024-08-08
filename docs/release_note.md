# Pulse Oximetry Release Note

## **v0.0.1**
### 1. Features
Initialization release.
#### 1.1. Firmware
- Estimate the heart rate using TERMA framework.
- Manage the button for click and hold event.
#### 1.2. Application
- Complete UI design.
- Support serial port connection.
- Plot the heart rate data and PPG signals.
- Set interval for collecting heart rate data from device.

### 2. Issues
#### 2.1. Firmware
- Peak detector algorithm is not stable and low accuracy.
#### 2.2. Application
- Can not detect more than one device.

### 3. In progress
#### 3.1. Firmware
- Handle the SSD1306 OLED display.
- Create the sound effect for buzzer.
#### 3.2. Application
- Auto-detect COM ports.
- Parse data for visualization.
- Set threshold (lower and higher) and initialization time for RTC.
- Stream real-time data on the graphs.


## **v0.1.0**
### 1. Features

#### 1.1. Firmware

- Display heart rate, graph, thresholds, notifications on SSD1306.
- RTC, UART, Buzzer working out-of-the-box.

#### 1.2. Application

- GUI interaction with MCU working well.

### 2. In progress

- Fix synchronization problem on sending and receiving records between and GUI and MCU.

### 3. Issues

- GUI and MCU not synchronized on sending and receiving records.

## Contributors
- Giang Phan Truong
- Khanh Nguyen Ngoc
- Viet Hoang Xuan