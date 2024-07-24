# PULSE OXIMETRY

# Table of Contents
- [PULSE OXIMETRY](#pulse-oximetry)
- [Table of Contents](#table-of-contents)
  - [Requirements](#requirements)
  - [Design](#design)
    - [Required Modules](#required-modules)
    - [Requirements Approach](#requirements-approach)
    - [Block Diagram](#block-diagram)
    - [Firmware Architecture](#firmware-architecture)

<a id="#1-requirements"></a>

## Requirements

* Stream the real-time PPG Heart Rate signal to GUI application.
* Measure the SpO2 and Heartbeat within an accuracy 10 percent of actual value.
* Detect the Hypertension and alert thoughout buzzer.

<a id="#firmware-architecture"></a>

## Design

### Required Modules

* STM32F411VET6 Discovery Kit
* DS1307
* SSD1306
* [Analog/Digital PPG Heart Rate Sensor](https://hshop.vn/products/cam-bien-nhip-tim-dfrobot-gravity-analog-digital-ppg-heart-rate-sensor)
* USB to TTL CP2102 


### Requirements Approach


### Block Diagram


### Firmware Architecture

 <p align="center">
  <img src="./img/firmware_architecture.svg" alt="Firmware Architecture" width="600" height="auto">
</p>