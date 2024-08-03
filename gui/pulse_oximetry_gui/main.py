import sys
import serial
import serial.tools.list_ports
import pyqtgraph as pg
import numpy as np
from PySide6.QtWidgets import QApplication, QMainWindow, QStackedWidget, QWidget, QMessageBox, QVBoxLayout
from PySide6.QtCore import Slot, QTimer
from ui_dev import Ui_Dev_UI
from ui_form import Ui_User_UI

class Widget(QWidget):
    def __init__(self, main_window):
        super().__init__()
        self.main_window = main_window
        self.setup_ui()

    def setup_ui(self):
        self.ui_dev = Ui_Dev_UI()
        self.ui_dev.setupUi(self)

        # Connect the button to switch UI
        self.ui_dev.btn_switch_to_user_ui.clicked.connect(self.show_user_ui)

        # Connect the combobox to mode time changed method
        self.ui_dev.cbb_mode_time.currentIndexChanged.connect(self.on_cbb_mode_time_changed)

        # Set default value for cbb_mode_time to "None"
        self.ui_dev.cbb_mode_time.setCurrentText("None")
        self.on_cbb_mode_time_changed()  # Update UI elements according to default value

        # Add the plot widget raw_ppg to the layout in dev.ui
        self.raw_ppg_graph = pg.PlotWidget()
        layout = QVBoxLayout(self.ui_dev.Raw_PPG)
        layout.addWidget(self.raw_ppg_graph)

        self.raw_ppg_graph.setBackground("w")
        self.raw_ppg_graph.setTitle("Raw PPG signal", color="black", size="10pt")

        styles = {"color": "black", "font-size": "12px"}
        self.raw_ppg_graph.setLabel("left", "PPG", **styles)
        self.raw_ppg_graph.setLabel("bottom", "Time (s)", **styles)

        pen = pg.mkPen(color=(255, 0, 0))  # Red
        ppg = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        time = [30, 32, 34, 32, 33, 31, 29, 32, 35, 45]
        self.raw_ppg_graph.plot(ppg, time, pen=pen)

        # Add the plot widget filtered_ppg to the layout in dev.ui
        self.filtered_ppg_graph = pg.PlotWidget()
        layout = QVBoxLayout(self.ui_dev.Filtered_PPG)
        layout.addWidget(self.filtered_ppg_graph)

        self.filtered_ppg_graph.setBackground("w")
        self.filtered_ppg_graph.setTitle("Filtered PPG signal", color="black", size="10pt")

        self.filtered_ppg_graph.setLabel("left", "PPG", **styles)
        self.filtered_ppg_graph.setLabel("bottom", "Time (s)", **styles)

        pen1 = pg.mkPen(color=(0, 255, 0))  # Green
        ppg1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        time1 = [30, 32, 34, 32, 33, 31, 29, 32, 35, 45]
        self.filtered_ppg_graph.plot(ppg1, time1, pen=pen1)

    @Slot()
    def show_user_ui(self):
        self.main_window.stacked_widget.setCurrentWidget(self.main_window.user_ui)

    @Slot()
    @Slot()
    def on_cbb_mode_time_changed(self):
        current_mode = self.ui_dev.cbb_mode_time.currentText()
        self.ui_dev.lbl_mode_time.setVisible(True)
        self.ui_dev.cbb_mode_time.setVisible(True)

        modes_visibility = {
            "Date time mode 12h": (True, True, False, False, True, True, False, False, True, True),
            "Date time mode 24h": (True, True, True, True, False, False, False, False, True, True),
            "Epoch time": (False, False, False, False, False, False, True, True, False, True),
            "None": (False, False, False, False, False, False, False, False, False, False)
        }

        visibility = modes_visibility.get(current_mode, (True, True, False, False, False, False, False, False, False, False))

        (lbl_date_time, lbl_set_ddmy, lbl_mode_24, te_mode_24, lbl_mode_12,
        te_mode_12, lbl_epoch_time, line_epoch_time, calendar_widget, btn_set_rtc) = visibility

        self.ui_dev.lbl_date_time.setVisible(lbl_date_time)
        self.ui_dev.lbl_set_ddmy.setVisible(lbl_set_ddmy)
        self.ui_dev.lbl_mode_24.setVisible(lbl_mode_24)
        self.ui_dev.te_mode_24.setVisible(te_mode_24)
        self.ui_dev.lbl_mode_12.setVisible(lbl_mode_12)
        self.ui_dev.te_mode_12.setVisible(te_mode_12)
        self.ui_dev.lbl_epoch_time.setVisible(lbl_epoch_time)
        self.ui_dev.line_epoch_time.setVisible(line_epoch_time)
        self.ui_dev.calendar_widget.setVisible(calendar_widget)
        self.ui_dev.btn_set_rtc.setVisible(btn_set_rtc)

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.stacked_widget = QStackedWidget()
        self.setCentralWidget(self.stacked_widget)

        # Set up the QWidget from dev.ui
        self.dev_widget = Widget(self)

        # Set up the QMainWindow from form.ui
        self.user_ui = QMainWindow()
        self.ui_user = Ui_User_UI()
        self.ui_user.setupUi(self.user_ui)

        # Add the dev_widget (dev.ui) to the stacked widget
        self.stacked_widget.addWidget(self.dev_widget)

        # Add the user_ui (form.ui) to the stacked widget
        self.stacked_widget.addWidget(self.user_ui)

        # Connect the button to switch UI
        self.ui_user.btn_switch_to_dev_ui.clicked.connect(self.show_dev_ui)

        # Connect btn_set_interval to send_interval_code method
        self.ui_user.btn_set_interval.clicked.connect(self.send_interval_code)

        # Connect btn_set_threshold to send_threshold_code method
        self.ui_user.btn_set_threshold.clicked.connect(self.send_threshold_code)

        # Connect btn_read_record to send_read_record_code method
        self.ui_user.btn_read_record.clicked.connect(self.send_read_record_code)

        # Connect btn_clear_record to send_clear_record_code method
        self.ui_user.btn_clear_record.clicked.connect(self.send_clear_record_code)

        # Connect btn_check_com to send_check_com_code method
        self.ui_user.btn_check_com.clicked.connect(self.send_check_com_code)

        # Set the initial widget to user_ui
        self.stacked_widget.setCurrentWidget(self.user_ui)

        # Add the heart rate plot to the layout in user.ui
        self.heart_rate_graph = pg.PlotWidget()
        layout_hr = QVBoxLayout(self.ui_user.Heart_rate_graph)
        self.ui_user.Heart_rate_graph.setLayout(layout_hr)
        layout_hr.addWidget(self.heart_rate_graph)

        self.heart_rate_graph.setBackground("w")
        self.heart_rate_graph.setTitle("Heart Rate Graph", color="black", size="10pt")

        styles = {"color": "black", "font-size": "13px"}
        self.heart_rate_graph.setLabel("left", "Heart Rate (bpm)", **styles)
        self.heart_rate_graph.setLabel("bottom", "Time (s)", **styles)

        pen_hr = pg.mkPen(color=(0, 0, 255))  # Blue
        heart_rate_time = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        heart_rate = [72, 75, 78, 76, 77, 79, 74, 73, 78, 80]
        self.heart_rate_graph.plot(heart_rate_time, heart_rate, pen=pen_hr)

        # Set default value for cbb_baudrate
        self.ui_user.cbb_baudrate.setCurrentText("115200")

        # Initialize serial communication
        self.serial_connection = None
        self.update_available_ports()
        self.ui_user.btn_connect_com.clicked.connect(self.toggle_serial_connection)

        # Initialize and start the timer to check for COM port updates
        self.port_check_timer = QTimer(self)
        self.port_check_timer.timeout.connect(self.update_available_ports)
        self.port_check_timer.start(1000)  # Check every 1000 ms (1 second)

    def update_available_ports(self):
        # Update available COM ports
        available_ports = [port.device for port in serial.tools.list_ports.comports()]
        current_ports = [self.ui_user.cbb_com.itemText(i) for i in range(self.ui_user.cbb_com.count())]

        # Update the combobox only if there is a change in available ports
        if set(available_ports) != set(current_ports):
            self.ui_user.cbb_com.clear()
            self.ui_user.cbb_com.addItems(available_ports)

    @Slot()
    def toggle_serial_connection(self):
        if self.serial_connection and self.serial_connection.is_open:
            self.disconnect_serial()
        else:
            self.connect_serial()

    @Slot()
    def connect_serial(self):
        port = self.ui_user.cbb_com.currentText()
        baudrate = self.ui_user.cbb_baudrate.currentText()
        try:
            self.serial_connection = serial.Serial(port, baudrate, timeout=1)
            self.ui_user.btn_connect_com.setText("Disconnect")  # Update button text
            QMessageBox.information(self, "Connection", f"Connected to {port} at {baudrate} baudrate.")

        except Exception:
            self.update_available_ports()
            QMessageBox.warning(self, "Error", "Serial port not found.")

    @Slot()
    def disconnect_serial(self):
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
            self.ui_user.btn_connect_com.setText("Connect")  # Update button text
            QMessageBox.information(self, "Disconnection", "Serial port disconnected.")

    @Slot()
    def show_dev_ui(self):
        self.stacked_widget.setCurrentWidget(self.dev_widget)

    @Slot()
    def send_interval_code(self):
        # Send the interval code based on the value in line_set_interval.
        try:
            if not (self.serial_connection and self.serial_connection.is_open):
                raise Exception("Serial port not connected.")

            # Read the value from line_set_interval and convert it to an integer
            interval_value = int(self.ui_user.line_set_interval.text())

            # Check if the interval value is positive
            if interval_value <= 0:
                raise ValueError("Interval value must be a positive integer.")

            # Convert the integer value to an 8-character hex string
            interval_hex = f'{interval_value:08X}'

            # Create the command string by concatenating the start code (0x1), cmd (0x3), hex value, threshold (0xFF) and end code (0x04)
            interval_command = f'13{interval_hex}FF04'

            # Convert the command string to bytes for sending over serial
            interval_command_bytes = bytes.fromhex(interval_command)

            # Check if serial_connection has been established and is open
            if self.serial_connection and self.serial_connection.is_open:
                # Send the byte command over serial
                self.serial_connection.write(interval_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {interval_command}")
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except ValueError:
            # Show warning if the input value is not a valid positive integer
            QMessageBox.warning(self, "Error", "Invalid interval value. Please enter a valid positive integer.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

    @Slot()
    def send_threshold_code(self):
        # Send the threshold code based on the value in line_high_level and line_low_level.
        try:
            if not (self.serial_connection and self.serial_connection.is_open):
                raise Exception("Serial port not connected.")

            # Read the value from line_high_level and convert it to an integer
            threshold_high_value = int(self.ui_user.line_high_level.text())

            # Read the value from line_high_level and convert it to an integer
            threshold_low_value = int(self.ui_user.line_low_level.text())

            # Check if the threshold value is positive
            if (threshold_high_value <= 0) or (threshold_low_value <= 0):
                raise ValueError("Threshold value must be a positive integer.")

            # Convert the integer values to an 2-character hex string
            threshold_high_hex = f'{threshold_high_value:02X}'
            threshold_low_hex = f'{threshold_low_value:02X}'

            # Create the command string by concatenating the start code (0x1), cmd (0x2), hex value, threshold (0xFF) and end code (0x04)
            threshold_command = f'12FFFF{threshold_high_hex}{threshold_low_hex}FF04'

            # Convert the command string to bytes for sending over serial
            threshold_command_bytes = bytes.fromhex(threshold_command)

            # Check if serial_connection has been established and is open
            if self.serial_connection and self.serial_connection.is_open:
                # Send the byte command over serial
                self.serial_connection.write(threshold_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {threshold_command}")
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except ValueError:
            # Show warning if the input value is not a valid positive integer
            QMessageBox.warning(self, "Error", "Invalid threshold value. Please enter a valid positive integer.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

    @Slot()
    def send_check_com_code(self):
        try:
            if not (self.serial_connection and self.serial_connection.is_open):
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x0), hex value, threshold (0xFF) and end code (0x04)
            check_com_hex_command = '10FFFFFFFFFF04'
            check_com_command_bytes = bytes.fromhex(check_com_hex_command)
            self.serial_connection.write(check_com_command_bytes)

            # Check if serial_connection has been established and is open
            if self.serial_connection and self.serial_connection.is_open:
                # Send the byte command over serial
                self.serial_connection.write(check_com_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {check_com_hex_command}")
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

    @Slot()
    def send_read_record_code(self):
        try:
            if not (self.serial_connection and self.serial_connection.is_open):
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x1), hex value, threshold (0xFF) and end code (0x04)
            read_record_hex_command = '11FFFFFFF1FF04'
            read_record_command_bytes = bytes.fromhex(read_record_hex_command)
            self.serial_connection.write(read_record_command_bytes)

            # Check if serial_connection has been established and is open
            if self.serial_connection and self.serial_connection.is_open:
                # Send the byte command over serial
                self.serial_connection.write(read_record_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {read_record_hex_command}")
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

    @Slot()
    def send_clear_record_code(self):
        try:
            if not (self.serial_connection and self.serial_connection.is_open):
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x5), hex value, threshold (0xFF) and end code (0x04)
            clear_record_hex_command = '15FFFFFFFFFF04'
            clear_record_command_bytes = bytes.fromhex(clear_record_hex_command)
            self.serial_connection.write(clear_record_command_bytes)

            # Check if serial_connection has been established and is open
            if self.serial_connection and self.serial_connection.is_open:
                # Send the byte command over serial
                self.serial_connection.write(clear_record_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {clear_record_hex_command}")
                # Clear the content of txt_record
                self.ui_user.txt_record.clear()
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_window = MainWindow()
    main_window.show()
    sys.exit(app.exec())
