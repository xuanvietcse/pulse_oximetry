# PULSE OXIMETRY

# Table of Contents
- [PULSE OXIMETRY](#pulse-oximetry)
- [Table of Contents](#table-of-contents)
  - [Requirements](#requirements)
  - [Design](#design)
    - [Requirements Approach](#requirements-approach)
    - [Required Modules](#required-modules)
    - [Detail Features](#detail-features)
      - [Stream the real-time Heart Rate with 0.92" OLED](#stream-the-real-time-heart-rate-with-092-oled)
      - [Visualize the Heart Rate record on GUI application](#visualize-the-heart-rate-record-on-gui-application)
      - [Record the Heart Rate manually or periodic](#record-the-heart-rate-manually-or-periodic)
      - [Notificate to user when the Heart Rate is lower or upper then threshold](#notificate-to-user-when-the-heart-rate-is-lower-or-upper-then-threshold)
    - [Block Diagram](#block-diagram)
    - [Firmware Architecture](#firmware-architecture)
    - [Contributors](#contributors)

<a id="#1-requirements"></a>

## Requirements

* Measure the Heartbeat within an accuracy 10 percent of actual value.
* Stream the real-time PPG Heart Rate signal in OLED screen.
* Record the current Heart Rate by clicking the button.
* Record the Heart Rate per period by setting interval in GUI application.
* Store upto 1,000 Heart Rate records with format *hh:mm:ss - DD/MM/YY - heart rate result*.
* Detect the Hypertension and alert through buzzer.
* Set the alert threshold throught GUI application.
* Show the records on GUI.

<a id="#firmware-architecture"></a>

## Design

### Requirements Approach

1. **Measure the Heartbeat within an accuracy 10 percent of actual value.**

- We apply the FFT to transform and analysis the PPG signal in frequency domain. After that, we create the histogram chart and determine the peak of signal each period. We count the peaks in a short time, for example, in 5 second. Then, we multiply that result by 12 to calculate the Heart Rate in bpm (beats per minute) unit.

2. **Stream the real-time PPG Heart Rate signal in OLED screen.**
- The problem is real-time display. Heart Rate PPG signal have the frequency in range $0.5 - 5Hz$ (with Heart Rate from 30 to 300 bpm). So that, the refesh rate of display should be $5Hz$ minimum - 200ms per frame. We also consider the bandwidth of I2C interface (in this case is $400kHz$) and the frame data. We only refresh the area of display that have a real-time graph. 

3. **Record the current Heart Rate by clicking the button.**

- By the time user click the button, device will get the current Heart Rate value and store it in EEPROM of RTC module with timestamp.

4. **Record the Heart Rate per period that set in GUI application.**

- In some case, we need to monitor Heart Rate for a long time. User can set the period to record the Heart Rate, and show the graph in GUI. How to store records can be mentioned below.

5. **Store upto 1,000 Heart Rate records with format *hh:mm:ss - DD/MM/YY - heart rate result*.**

- The Module RTC have EEPROM 24C32 32Kb, when use above format, each record get max. 32bit of space. So that, we can use this memory to store upto 1,0000 records. When strorage is full, stop recording and notificate user.

6. **Detect the Hypertension and alert through buzzer.**
- Device can do that if user enter the upper threshold and lower threshold of the Heart Rate.

7. **Set the alert threshold throught GUI application.**
- User set the upper and lower Heart Rate by this way. GUI will transmit that value to device and it will monitor and compare the Heart Rate.

8. **Show the records on GUI.**
- We use Qt Designer for Python to design the GUI. After receive the records to device, GUI will copy those data to .txt or .csv file and display records on a graph.


### Required Modules

* STM32F411VET6 Discovery Kit
* SSD1306 I2C OLED
* DS1307 RTC Module
* [Analog/Digital PPG Heart Rate Sensor](https://hshop.vn/products/cam-bien-nhip-tim-dfrobot-gravity-analog-digital-ppg-heart-rate-sensor)
* USB to TTL CP2102 

### Detail Features

#### Stream the real-time Heart Rate with 0.92" OLED
 <p align="center">
  <img src="./img/oled_ui.svg" alt="OLED UI" width="512" height="auto">
</p>

#### Visualize the Heart Rate record on GUI application
 <p align="center">
  <img src="./img/app_ui.svg" alt="APP UI" width="256" height="auto">
</p>

#### Record the Heart Rate manually or periodic
- Click the button to record the Heart Rate immediately.
- Set interval of each record. Minimum: 10 seconds.

#### Notificate to user when the Heart Rate is lower or upper then threshold
- The thresholds can be set on GUI application.
- Change the color of the heart icon at the up-right corner.
- Turn on the buzzer with sound "beep beep".

### Block Diagram

 <p align="center">
  <img src="./img/block_diagram.svg" alt="Block Diagram" width="600" height="auto">
</p>

### Firmware Architecture

 <p align="center">
  <img src="./img/firmware_architecture.svg" alt="Firmware Architecture" width="600" height="auto">
</p>

### Contributors
1. Viet Hoang Xuan
2. Giang Phan Truong
3. Khanh Nguyen Ngoc