# This Python file uses the following encoding: utf-8
# Important:
# You need to run the following command to generate the ui_dev.py file
#     pyside6-uic dev.ui -o ui_dev.py, or
#     pyside2-uic dev.ui -o ui_dev.py

# And similarly for the user UI:
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py.py

import sys
import serial
import serial.tools.list_ports
import pyqtgraph as pg
from PySide6.QtWidgets import QApplication, QMainWindow, QStackedWidget, QMessageBox, QVBoxLayout
from PySide6.QtCore import Slot, QTimer, QDateTime
from dev_ui_manage import Widget
from ui_form import Ui_User_UI
from serial_manage import serial_manage

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

        # Set the window title for the User UI window
        self.set_window_title("User UI")

        # Set default value for line_thre_noti
        self.ui_user.line_thre_noti.setText("Normal heart rate")

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
        self.heart_rate_graph.setLabel("bottom", "Time (h)", **styles)

        self.heart_rate_pen = pg.mkPen(color=(0, 0, 255))  # Blue
        self.heart_rate_graph.setXRange(0, 24) # X-Axis from 0h to 24h

        # Data lists for plotting heart rate
        self.heart_rate_time = []
        self.heart_rate_value = []

        # PlotDataItem for lines
        self.heart_rate_plot_lines = pg.PlotDataItem(pen=self.heart_rate_pen)
        self.heart_rate_graph.addItem(self.heart_rate_plot_lines)

        # ScatterPlotItem for scatter points
        self.heart_rate_scatter = pg.ScatterPlotItem(size=10, pen=None, brush=(0, 0, 255))
        self.heart_rate_graph.addItem(self.heart_rate_scatter)

        # Time data lists for plotting heart rate and displaying record
        self.dayofweek = []
        self.day = []
        self.month = []
        self.year = []
        self.hour = []
        self.minute = []
        self.second = []

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
        if self.serial_connection:
            self.disconnect_serial()

        else:
            self.connect_serial()

    @Slot()
    def connect_serial(self):
        port = self.ui_user.cbb_com.currentText()
        baudrate = self.ui_user.cbb_baudrate.currentText()
        if not port == "":
            try:
                self.serial_connection = serial_manage(port, baudrate)
                self.serial_connection.data_received.connect(self.process_serial_data)
                self.serial_connection.start()
                self.ui_user.btn_connect_com.setText("Disconnect")
                QMessageBox.information(self, "Connection", f"Connected to {port} at {baudrate} baudrate.")

            except Exception:
                self.update_available_ports()
                QMessageBox.warning(self, "Error", "Serial port not found.")
        else:
            QMessageBox.warning(self, "Error", "Serial port not found.")

    @Slot()
    def disconnect_serial(self):
        if self.serial_connection:
            self.serial_connection.stop()
            self.serial_connection = None
            self.ui_user.btn_connect_com.setText("Connect")
            QMessageBox.information(self, "Disconnection", "Serial port disconnected.")

    def set_window_title(self, title):
        self.setWindowTitle(title)

    @Slot()
    def show_dev_ui(self):
        self.stacked_widget.setCurrentWidget(self.dev_widget)
        # Set the title to "Dev UI"
        self.set_window_title("Dev UI")

    @Slot()
    def send_interval_code(self):
        # Send the interval code based on the value in line_set_interval.
        try:
            if not self.serial_connection:
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
            if self.serial_connection:
                # Send the byte command over serial
                self.serial_connection.send(interval_command_bytes)
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
            if not self.serial_connection:
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
            if self.serial_connection:
                # Send the byte command over serial
                self.serial_connection.send(threshold_command_bytes)
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
            if not self.serial_connection:
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x0), hex value, threshold (0xFF) and end code (0x04)
            check_com_hex_command = '10FFFFFFFFFF04'
            check_com_command_bytes = bytes.fromhex(check_com_hex_command)

            # Check if serial_connection has been established and is open
            if self.serial_connection:
                # Send the byte command over serial
                self.serial_connection.send(check_com_command_bytes)
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
            if not self.serial_connection:
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x1), hex value, threshold (0xFF) and end code (0x04)
            read_record_hex_command = '11FFFFFFF0FF04'
            read_record_command_bytes = bytes.fromhex(read_record_hex_command)

            # Check if serial_connection has been established and is open
            if self.serial_connection:
                # Send the byte command over serial
                self.serial_connection.send(read_record_command_bytes)
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
            if not self.serial_connection:
                raise Exception("Serial port not connected.")

            # Create the command string by concatenating the start code (0x1), cmd (0x5), hex value, threshold (0xFF) and end code (0x04)
            clear_record_hex_command = '15FFFFFFFFFF04'
            clear_record_command_bytes = bytes.fromhex(clear_record_hex_command)

            # Check if serial_connection has been established and is open
            if self.serial_connection:
                # Send the byte command over serial
                self.serial_connection.send(clear_record_command_bytes)
                # Show success message
                QMessageBox.information(self, "Success", f"Sent: {clear_record_hex_command}")
                # Clear the content of txt_record
                self.ui_user.txt_record.clear()
            else:
                # Show warning if serial port is not connected
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")

    @Slot(bytes)
    def process_serial_data(self, data):
        if self.serial_connection:
            try:
                if len(data) == 7:
                    packet = data.hex().upper()
                    if packet.startswith("1") and packet.endswith("04"):
                        cmd = packet[1:2]
                        data = packet[2:10]
                        threshold = packet[10:12]
                        if threshold in ["FF", "0F", "F0"]:
                            if threshold == "0F":
                                self.ui_user.line_thre_noti.setText("Heart rate too high")
                            elif threshold == "F0":
                                self.ui_user.line_thre_noti.setText("Heart rate too low")
                            elif threshold == "FF":
                                self.ui_user.line_thre_noti.setText("Normal heart rate")
                        else:
                            QMessageBox.warning(self, "Error", "Invalid threshold byte")

                        if cmd in ["1", "4", "6"]:
                            if cmd == "6":
                                if data == "FFFFFFFF":
                                    self.dev_widget.ui_dev.line_err_noti.setText("Error occurred")
                                else:
                                    QMessageBox.warning(self, "Error", "Invalid data")
                            elif cmd == "1":
                                data_type = data[7:8]
                                if data_type in ["0", "1", "2"]:
                                    if data_type == "0":
                                        #plot heart rate
                                        data_value = int(data[0:7], 16)

                                        time_in_hours = self.hour[-1] + self.minute[-1] / 60 + self.second[-1] / 3600
                                        self.heart_rate_time.append(time_in_hours)
                                        self.heart_rate_value.append(data_value)

                                        # Update the PlotDataItem
                                        self.heart_rate_plot_lines.setData(self.heart_rate_time, self.heart_rate_value)

                                        # Update the ScatterPlotItem
                                        self.heart_rate_scatter.setData(self.heart_rate_time, self.heart_rate_value)


                                    elif data_type == "1":
                                        #plot filtered ppg signal
                                        data_value = int(data[0:7], 16)

                                        time_in_hours = self.hour[-1] + self.minute[-1] / 60 + self.second[-1] / 3600
                                        self.dev_widget.filtered_ppg_time.append(time_in_hours)
                                        self.dev_widget.filtered_ppg_value.append(data_value)
                                        self.dev_widget.filtered_ppg_graph.plot(self.dev_widget.filtered_ppg_time, self.dev_widget.filtered_ppg_value, pen=self.dev_widget.filtered_ppg_pen, clear=True)


                                    elif data_type == "2":
                                        #plot raw ppg signal
                                        data_value = int(data[0:7], 16)

                                        time_in_hours = self.hour[-1] + self.minute[-1] / 60 + self.second[-1] / 3600
                                        self.dev_widget.raw_ppg_time.append(time_in_hours)
                                        self.dev_widget.raw_ppg_value.append(data_value)
                                        self.dev_widget.raw_ppg_graph.plot(self.dev_widget.raw_ppg_time, self.dev_widget.raw_ppg_value, pen=self.dev_widget.raw_ppg_pen, clear=True)

                                else:
                                    QMessageBox.warning(self, "Error", "Invalid data type")

                            elif cmd == "4":
                                epoch_value = int(data, 16)
                                dt = QDateTime.fromSecsSinceEpoch(epoch_value)

                                self.dayofweek.append(dt.date().dayOfWeek() - 1)  # QDate.dayOfWeek(): 1 (Monday) to 7 (Sunday)
                                self.day.append( dt.date().day())
                                self.month.append(dt.date().month())
                                self.year.append(dt.date().year())
                                self.hour.append(dt.time().hour())
                                self.minute.append(dt.time().minute())
                                self.second.append(dt.time().second())
                        else:
                            QMessageBox.warning(self, "Error", "Invalid command")
                    else:
                        QMessageBox.warning(self, "Error", "Invalid frame of data packet")
                else:
                    QMessageBox.warning(self, "Error", "Invalid data length received from serial port")

            except Exception as e:
                QMessageBox.warning(self, "Error", f"Failed to read serial data: {str(e)}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_window = MainWindow()
    main_window.show()
    sys.exit(app.exec())
