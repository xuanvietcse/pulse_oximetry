# This Python file uses the following encoding: utf-8

from PySide6 import QtCore
import serial
import threading
from PySide6.QtCore import Signal

class serial_manage(QtCore.QObject):
    data_received = Signal(bytes)

    def __init__(self, port, baudrate):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.running = False
        self.thread = threading.Thread(target=self.run)
        self.serial_connection = None

    def start(self):
        if not self.running:
            self.running = True
            self.thread.start()

    def stop(self):
        self.running = False
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()

    def run(self):
        self.serial_connection = serial.Serial(self.port, self.baudrate, timeout=1)
        while self.running:
            if self.serial_connection.in_waiting > 0:
                data = self.serial_connection.readall()
                self.data_received.emit(data)

    def send(self, command_bytes):
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.write(command_bytes)
