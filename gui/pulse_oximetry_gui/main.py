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
import pyqtgraph as pg
import numpy as np
from PySide6.QtWidgets import QApplication, QMainWindow, QStackedWidget, QWidget, QVBoxLayout
from PySide6.QtCore import Slot
from ui_dev import Ui_Dev_UI
from ui_form import Ui_User_UI

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.stacked_widget = QStackedWidget()
        self.setCentralWidget(self.stacked_widget)

        # Set up the QWidget from dev.ui
        self.dev_ui = QWidget()
        self.ui_dev = Ui_Dev_UI()
        self.ui_dev.setupUi(self.dev_ui)

        # Set up the QMainWindow from form.ui
        self.user_ui = QMainWindow()
        self.ui_user = Ui_User_UI()
        self.ui_user.setupUi(self.user_ui)

        self.stacked_widget.addWidget(self.dev_ui)
        self.stacked_widget.addWidget(self.user_ui)

        self.ui_dev.btn_switch_to_user_ui.clicked.connect(self.show_user_ui)
        self.ui_user.btn_switch_to_dev_ui.clicked.connect(self.show_dev_ui)

        # Connect the combobox to the slot to switch UI
        self.ui_dev.cbb_mode_time.currentIndexChanged.connect(self.on_combobox_changed)

        # Set the initial widget to user_ui
        self.stacked_widget.setCurrentWidget(self.user_ui)

        # Add the plot widget raw_ppg to the layout in dev.ui
        self.plot_graph = pg.PlotWidget()
        layout = QVBoxLayout(self.ui_dev.Raw_PPG)
        layout.addWidget(self.plot_graph)

        self.plot_graph.setBackground("w")
        self.plot_graph.setTitle("Raw PPG signal", color="black", size="10pt")

        styles = {"color": "black", "font-size": "15px"}
        self.plot_graph.setLabel("left", "Temperature (°C)", **styles)
        self.plot_graph.setLabel("bottom", "Time (min)", **styles)

        pen = pg.mkPen(color=(255, 0, 0)) # Red
        ppg = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        time = [30, 32, 34, 32, 33, 31, 29, 32, 35, 45]
        self.plot_graph.plot(ppg, time, pen=pen)

        # Add the plot widget filtered_ppg to the layout in dev.ui
        self.filtered_graph = pg.PlotWidget()
        layout = QVBoxLayout(self.ui_dev.Filtered_PPG)
        layout.addWidget(self.filtered_graph)

        self.filtered_graph.setBackground("w")
        self.filtered_graph.setTitle("Filtered PPG signal", color="black", size="10pt")

        self.filtered_graph.setLabel("left", "Temperature (°C)", **styles)
        self.filtered_graph.setLabel("bottom", "Time (min)", **styles)

        pen1 = pg.mkPen(color=(0, 255, 0)) # Red
        ppg1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        time1 = [30, 32, 34, 32, 33, 31, 29, 32, 35, 45]
        self.filtered_graph.plot(ppg1, time1, pen=pen1)

        # Add the heart rate plot to the layout in user.ui
        self.heart_rate_plot = pg.PlotWidget()
        layout_hr = QVBoxLayout(self.ui_user.Heart_rate_graph)
        self.ui_user.Heart_rate_graph.setLayout(layout_hr)
        layout_hr.addWidget(self.heart_rate_plot)

        self.heart_rate_plot.setBackground("w")
        self.heart_rate_plot.setTitle("Heart Rate Graph", color="black", size="10pt")

        self.heart_rate_plot.setLabel("left", "Heart Rate (bpm)", **styles)
        self.heart_rate_plot.setLabel("bottom", "Time (s)", **styles)

        pen_hr = pg.mkPen(color=(0, 0, 255)) # Blue
        heart_rate_time = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        heart_rate = [72, 75, 78, 76, 77, 79, 74, 73, 78, 80]
        self.heart_rate_plot.plot(heart_rate_time, heart_rate, pen=pen_hr)

    @Slot()
    def show_user_ui(self):
        self.stacked_widget.setCurrentWidget(self.user_ui)

    @Slot()
    def show_dev_ui(self):
        self.stacked_widget.setCurrentWidget(self.dev_ui)

    @Slot()
    def on_combobox_changed(self):
        if self.ui_dev.cbb_mode_time.currentText() == "Date time":
            self.stacked_widget.setCurrentWidget(self.user_ui)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_window = MainWindow()
    main_window.show()
    sys.exit(app.exec())
