# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'dev.ui'
##
## Created by: Qt User Interface Compiler version 6.7.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QDateEdit, QGroupBox,
    QLabel, QLineEdit, QPushButton, QSizePolicy,
    QTimeEdit, QWidget)

class Ui_Dev_UI(object):
    def setupUi(self, Dev_UI):
        if not Dev_UI.objectName():
            Dev_UI.setObjectName(u"Dev_UI")
        Dev_UI.resize(1535, 814)
        self.Filtered_PPG = QGroupBox(Dev_UI)
        self.Filtered_PPG.setObjectName(u"Filtered_PPG")
        self.Filtered_PPG.setGeometry(QRect(680, 390, 821, 331))
        self.btn_switch_to_user_ui = QPushButton(Dev_UI)
        self.btn_switch_to_user_ui.setObjectName(u"btn_switch_to_user_ui")
        self.btn_switch_to_user_ui.setGeometry(QRect(720, 740, 141, 41))
        self.Raw_PPG = QGroupBox(Dev_UI)
        self.Raw_PPG.setObjectName(u"Raw_PPG")
        self.Raw_PPG.setGeometry(QRect(680, 20, 821, 351))
        self.Set_RTC = QGroupBox(Dev_UI)
        self.Set_RTC.setObjectName(u"Set_RTC")
        self.Set_RTC.setGeometry(QRect(30, 20, 631, 601))
        self.btn_set_rtc = QPushButton(self.Set_RTC)
        self.btn_set_rtc.setObjectName(u"btn_set_rtc")
        self.btn_set_rtc.setGeometry(QRect(270, 560, 93, 29))
        self.cbb_mode_time = QComboBox(self.Set_RTC)
        self.cbb_mode_time.addItem("")
        self.cbb_mode_time.addItem("")
        self.cbb_mode_time.setObjectName(u"cbb_mode_time")
        self.cbb_mode_time.setGeometry(QRect(170, 30, 111, 26))
        self.lbl_epoch_time = QLabel(self.Set_RTC)
        self.lbl_epoch_time.setObjectName(u"lbl_epoch_time")
        self.lbl_epoch_time.setGeometry(QRect(20, 520, 141, 21))
        self.lbl_mode_time = QLabel(self.Set_RTC)
        self.lbl_mode_time.setObjectName(u"lbl_mode_time")
        self.lbl_mode_time.setGeometry(QRect(20, 30, 81, 20))
        self.line_epoch_time = QLineEdit(self.Set_RTC)
        self.line_epoch_time.setObjectName(u"line_epoch_time")
        self.line_epoch_time.setGeometry(QRect(170, 520, 251, 26))
        self.lbl_date_time = QLabel(self.Set_RTC)
        self.lbl_date_time.setObjectName(u"lbl_date_time")
        self.lbl_date_time.setGeometry(QRect(20, 80, 131, 20))
        self.lbl_day = QLabel(self.Set_RTC)
        self.lbl_day.setObjectName(u"lbl_day")
        self.lbl_day.setGeometry(QRect(170, 80, 51, 20))
        self.lbl_date = QLabel(self.Set_RTC)
        self.lbl_date.setObjectName(u"lbl_date")
        self.lbl_date.setGeometry(QRect(360, 80, 51, 20))
        self.lbl_month = QLabel(self.Set_RTC)
        self.lbl_month.setObjectName(u"lbl_month")
        self.lbl_month.setGeometry(QRect(170, 130, 51, 20))
        self.lbl_year = QLabel(self.Set_RTC)
        self.lbl_year.setObjectName(u"lbl_year")
        self.lbl_year.setGeometry(QRect(360, 130, 41, 20))
        self.lbl_hour = QLabel(self.Set_RTC)
        self.lbl_hour.setObjectName(u"lbl_hour")
        self.lbl_hour.setGeometry(QRect(170, 180, 51, 20))
        self.lbl_min = QLabel(self.Set_RTC)
        self.lbl_min.setObjectName(u"lbl_min")
        self.lbl_min.setGeometry(QRect(360, 180, 51, 20))
        self.lbl_sec = QLabel(self.Set_RTC)
        self.lbl_sec.setObjectName(u"lbl_sec")
        self.lbl_sec.setGeometry(QRect(270, 220, 51, 20))
        self.timeEdit = QTimeEdit(self.Set_RTC)
        self.timeEdit.setObjectName(u"timeEdit")
        self.timeEdit.setGeometry(QRect(110, 350, 118, 26))
        self.dateEdit = QDateEdit(self.Set_RTC)
        self.dateEdit.setObjectName(u"dateEdit")
        self.dateEdit.setGeometry(QRect(110, 410, 110, 26))
        self.Notification = QGroupBox(Dev_UI)
        self.Notification.setObjectName(u"Notification")
        self.Notification.setGeometry(QRect(30, 630, 631, 91))
        self.lbl_notification = QLabel(self.Notification)
        self.lbl_notification.setObjectName(u"lbl_notification")
        self.lbl_notification.setGeometry(QRect(28, 40, 81, 20))
        self.line_notification = QLineEdit(self.Notification)
        self.line_notification.setObjectName(u"line_notification")
        self.line_notification.setGeometry(QRect(110, 40, 491, 26))

        self.retranslateUi(Dev_UI)

        QMetaObject.connectSlotsByName(Dev_UI)
    # setupUi

    def retranslateUi(self, Dev_UI):
        Dev_UI.setWindowTitle(QCoreApplication.translate("Dev_UI", u"Form", None))
        self.Filtered_PPG.setTitle(QCoreApplication.translate("Dev_UI", u"Filtered PPG signal", None))
        self.btn_switch_to_user_ui.setText(QCoreApplication.translate("Dev_UI", u"Switch to User UI", None))
        self.Raw_PPG.setTitle(QCoreApplication.translate("Dev_UI", u"Raw PPG signal", None))
        self.Set_RTC.setTitle(QCoreApplication.translate("Dev_UI", u"Set RTC", None))
        self.btn_set_rtc.setText(QCoreApplication.translate("Dev_UI", u"Set time", None))
        self.cbb_mode_time.setItemText(0, QCoreApplication.translate("Dev_UI", u"Date time", None))
        self.cbb_mode_time.setItemText(1, QCoreApplication.translate("Dev_UI", u"Epoch time", None))

        self.lbl_epoch_time.setText(QCoreApplication.translate("Dev_UI", u"Epoch time (GMT0):", None))
        self.lbl_mode_time.setText(QCoreApplication.translate("Dev_UI", u"Mode time:", None))
        self.lbl_date_time.setText(QCoreApplication.translate("Dev_UI", u"Date time (GMT7):", None))
        self.lbl_day.setText(QCoreApplication.translate("Dev_UI", u"Day:", None))
        self.lbl_date.setText(QCoreApplication.translate("Dev_UI", u"Date:", None))
        self.lbl_month.setText(QCoreApplication.translate("Dev_UI", u"Month:", None))
        self.lbl_year.setText(QCoreApplication.translate("Dev_UI", u"Year:", None))
        self.lbl_hour.setText(QCoreApplication.translate("Dev_UI", u"Hour:", None))
        self.lbl_min.setText(QCoreApplication.translate("Dev_UI", u"Min:", None))
        self.lbl_sec.setText(QCoreApplication.translate("Dev_UI", u"Sec:", None))
        self.timeEdit.setDisplayFormat(QCoreApplication.translate("Dev_UI", u"hh:mm:ss", None))
        self.Notification.setTitle(QCoreApplication.translate("Dev_UI", u"Notification", None))
        self.lbl_notification.setText(QCoreApplication.translate("Dev_UI", u"Status:", None))
    # retranslateUi

