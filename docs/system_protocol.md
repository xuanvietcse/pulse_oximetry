# SYSTEM PROTOCOL FOR INTERACTION BETWEEN GUI AND STM32

## Requirements of GUI

1. **Set time for RTC**

When users turn on device, they must be able to configure the time of RTC.

2. **Get time from RTC**

GUI can read time from RTC for displaying the record of the heart rate.

3. **Set interval**

Users can configure their preference interval for getting the PPG signal using for display on OLED.

4. **Set threshold**

Users can configure specific threshold for detecting dangers, e.g. 30 for too low heart rate, 200 for too high heart rate.

5. **Get data**

Users can get data from system (heart rate, logs, PPG signal before and after filtered).

6. **Clear records**

Users can clear all previous records which are unneeded to reduce memory.

7. **Warning**

Whenever heart rate overcomes the threshold, system must signal to users.

8. **Error notification**

If there is any error, system must notify to users.

**More over, every time users or developers send a packet to system, there will be a **Check UART** packet sent first to ensure there is no packet lost between communication process.**

## Packet format

### 5 Fields
- Start of frame (SOF):
  - Length: 1 byte.
  - Value: 0x01.
  - Inform starting point of packet.
- Command Type (CMD):
  - Length: 1 byte.
  - Value:
    - 0x00: Check UART.
    - 0x01: Get data.
    - 0x02: Set Threshold.
    - 0x03: Set interval.
    - 0x04: Set/Get time.
    - 0x05: Clear records.
    - 0x06: Error notification.
- Data (DATA):
  - Length: 4 bytes.
  - Value: Based on CMD.
    - Check UART: 0xFFFFFFFF.
    - Get data: 0xFFFFFFFX. The last nibble X depends on what users want to get. 0x0: record (including heart rate + time), 0x1: PPG signal after filtered, 0x2: PPG signal before filtered.
    - Set threshold: Based on Nes, *et al.* (2013)[[1]](https://sci-hub.se/https://doi.org/10.1111/j.1600-0838.2012.01445.x), Heart rate max = 211 - (0.64 * age). So maximum heart rate we can get is 211. Therefore, we will need 1 byte to store the heart rate we want to set. 0xFFFFFFXX, XX is the hex value of heart rate to be set.
    - Set interval: Default unit is second (s). 0xXXXXXXXX, we should read datasheet before to ensure the interval we set is supported by MCU.
    - Set/Get time: 0xXXXXXXXX is the epoch time.
    - Clear records: 0xFFFFFFFF.
    - Error notification: 0xFFFFFFFF.
- Threshold:
  - Length: 1 byte.
  - Value: 0xXY. 
    - XY = FF, heart rate stays between upper and lower threshold. 
    - XY = F0, heart rate is lower than the lower heart rate. 
    - XY = 0F, heart rate is higher than the upper heart rate.
- End of frame (EOF):
  - Length: 1 byte.
  - Value: 0x04.
  - Inform ending point of packet.

**Example packet:**

![image](./img/example_packet.jpg)

## Reference

[1] https://sci-hub.se/https://doi.org/10.1111/j.1600-0838.2012.01445.x