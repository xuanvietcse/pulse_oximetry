# SYSTEM PROTOCOL FOR INTERACTION BETWEEN GUI AND STM32

## Requirements of GUI

1. **Set time for RTC**

When users turn on device, they must be able to configure the time of RTC.

2. **Set interval**

Users can configure their preference interval for getting the PPG signal using for display on OLED.

3. **Set threshold**

Users can configure specific threshold for detecting dangers, e.g. 30 for too low heart rate, 200 for too high heart rate.

4. **Get data**

Users can get data from system (heart rate, logs, PPG signal before and after filtered).

5. **Clear records**

Users can clear all previous records which are unneeded to reduce memory.

6. **Warning**

Whenever heart rate overcomes the threshold, system must signal to users.

7. **DEV ONLY: Tune filters**

Developers can tune their filter on system based on GUI.

8. **Error notification**

If there is any error, system must notify to users.

**More over, every time users or developers send a packet to system, there will be a **Check UART** packet sent first to ensure there is no packet lost between communication process.**

