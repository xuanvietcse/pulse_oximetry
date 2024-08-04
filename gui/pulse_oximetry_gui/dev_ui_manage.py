# This Python file uses the following encoding: utf-8

from PySide6 import QtWidgets
from PySide6.QtWidgets import QMessageBox, QVBoxLayout
from PySide6.QtCore import Slot, QDateTime
from ui_dev import Ui_Dev_UI
import pyqtgraph as pg

class Widget(QtWidgets.QWidget):
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

        # Set default value for line_err_noti
        self.ui_dev.line_err_noti.setText("No error")

        # Connect the btn_set_rtc to send_rtc_time method
        self.ui_dev.btn_set_rtc.clicked.connect(self.send_rtc_time)

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
        tim = [30, 32, 34, 32, 33, 31, 29, 32, 35, 45]
        self.raw_ppg_graph.plot(ppg, tim, pen=pen)

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
        self.main_window.set_window_title("User UI")

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

    @Slot()
    def send_rtc_time(self):
        try:
            if not self.main_window.serial_connection:
                raise Exception("Serial port not connected.")

            mode = self.ui_dev.cbb_mode_time.currentText()

            if mode == "Date time mode 12h":
                selected_date = self.ui_dev.calendar_widget.selectedDate()
                selected_time = self.ui_dev.te_mode_12.time()
                date_time_str = f"{selected_date.toString('yyyy-MM-dd')} {selected_time.toString('hh:mm:ss AP')}"
                dt = QDateTime.fromString(date_time_str, 'yyyy-MM-dd hh:mm:ss AP')
                if not (dt.isValid()):
                    raise ValueError("Invalid date or time.")

            elif mode == "Date time mode 24h":
                selected_date = self.ui_dev.calendar_widget.selectedDate()
                selected_time = self.ui_dev.te_mode_24.time()
                date_time_str = f"{selected_date.toString('yyyy-MM-dd')} {selected_time.toString('hh:mm:ss')}"
                dt = QDateTime.fromString(date_time_str, 'yyyy-MM-dd hh:mm:ss')
                if not (dt.isValid()):
                    raise ValueError("Invalid date or time.")

            elif mode == "Epoch time":
                epoch_time_str = self.ui_dev.line_epoch_time.text()
                try:
                    epoch_time = int(epoch_time_str)
                    epoch_hex = f'{epoch_time:08X}'
                    dt = QDateTime.fromSecsSinceEpoch(epoch_time)
                except ValueError:
                    QMessageBox.warning(self, "Error", "Invalid epoch time.")
                    return
            else:
                QMessageBox.warning(self, "Error", "Invalid mode selected.")
                return

            epoch_time = dt.toSecsSinceEpoch() if mode != "Epoch time" else epoch_time
            epoch_hex = f'{epoch_time:08X}'

            rtc_command = f'14{epoch_hex}FF04'
            rtc_command_bytes = bytes.fromhex(rtc_command)

            if self.main_window.serial_connection:
                self.main_window.serial_connection.send(rtc_command_bytes)
                QMessageBox.information(self, "Success", f"Sent: {rtc_command}")
            else:
                QMessageBox.warning(self, "Error", "Serial port is not connected.")

        except ValueError:
            QMessageBox.warning(self, "Error", "Invalid date or time.")

        except Exception:
            QMessageBox.warning(self, "Error", "Serial port not connected.")
