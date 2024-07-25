# PULSE OXIMETRY

# Table of Contents
- [PULSE OXIMETRY](#pulse-oximetry)
- [Table of Contents](#table-of-contents)
  - [Requirements](#requirements)
  - [Design](#design)
    - [Requirements Approach](#requirements-approach)
    - [Required Modules](#required-modules)
    - [Detail Features](#detail-features)
      - [GUI components](#gui-components)
    - [Block Diagram](#block-diagram)
    - [Firmware Architecture](#firmware-architecture)

<a id="#1-requirements"></a>

## Requirements

* Measure the Heartbeat within an accuracy 10 percent of actual value.
* Stream the real-time PPG Heart Rate signal in OLED screen.
* Record the current Heart Rate by clicking the button.
* Record the Heart Rate per period by setting interval in GUI application.
* Store upto .. Heart Rate records with format *hh:mm:ss - DD/MM/YY - heart rate result*.
* Detect the Hypertension and alert through buzzer.
* Set the alert threshold throught GUI application.
* Show the records on GUI.

<a id="#firmware-architecture"></a>

## Design

### Requirements Approach

### Required Modules

* STM32F411VET6 Discovery Kit
* SSD1306 I2C OLED
* DS107 RTC Module
* [Analog/Digital PPG Heart Rate Sensor](https://hshop.vn/products/cam-bien-nhip-tim-dfrobot-gravity-analog-digital-ppg-heart-rate-sensor)
* USB to TTL CP2102 

### Detail Features

#### GUI components


### Block Diagram

 <p align="center">
  <img src="./img/block_diagram.png" alt="Block Diagram" width="600" height="auto">
</p>

### Firmware Architecture

 <p align="center">
  <img src="./img/firmware_architecture.svg" alt="Firmware Architecture" width="600" height="auto">
</p>