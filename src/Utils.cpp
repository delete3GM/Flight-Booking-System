#include "Utils.h"
#include "qboxlayout.h"
#include "qcryptographichash.h"
#include "qdialog.h"
#include "qimage.h"
#include "QRCodeGenerator/qrcodegen.hpp"
#include "qlabel.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qwebenginesettings.h"
#include "qwebengineview.h"
#include <QDateTime>

QSet<QString> foreignCities = {"伦敦","纽约","莫斯科","悉尼","东京","巴黎"};

QMap<QString, double> mealPrice = {
    {"无餐食", 0.0},
    {"标准餐", 35.0},
    {"豪华餐", 80.0}
};

QMap<QString, double> cabinPrice = {
    {"经济舱", 1.0},
    {"商务舱", 1.8},
    {"头等舱", 2.4}
};

QTextStream* LoadFlightFile(const QString& filename) {
    QFile* flight_file = new QFile(filename);

    if (!flight_file->open(QIODevice::ReadOnly)) {
        qInfo() << "open file failed!";
        delete flight_file;
        return nullptr;
    }
    qInfo() << "航班文件：" << filename;
    QTextStream* stream = new QTextStream(flight_file);
    return stream;
}

int Duration(QString dep, QString arr) {
    QDateTime depTime = QDateTime::fromString(dep, "yyyy-MM-dd HH:mm");
    QDateTime arrTime = QDateTime::fromString(arr, "yyyy-MM-dd HH:mm");

    qint64 msecs = depTime.msecsTo(arrTime);
    int seconds = msecs / 1000;
    return seconds / 60; // 分钟
}

QString formatMinutes(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return QString("%1h %2min").arg(hours).arg(minutes);
}

QString hashPassword(const QString &password) {
    QByteArray passwordData = password.toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(passwordData);
    QByteArray hashResult = hash.result();
    return QString(hashResult.toHex());
}

QString getPasswordById(const QString &id) {
    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << USER_FILE;
        return QString();
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(' ');

        if (fields.size() == 2) {
            QString fileId = fields[0];
            QString password = fields[1];

            if (fileId == id) {
                file.close();
                return password;
            }
        }
    }
    file.close();
    return QString();
}

QImage GenerateQRCodeImage(const QString& text) {
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.toStdString().c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
    int size = qr.getSize();
    int moduleSize = 10; // 每个模块的像素大小
    QImage image(size * moduleSize, size * moduleSize, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setBrush(Qt::black);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * moduleSize, y * moduleSize, moduleSize, moduleSize);
            }
        }
    }
    painter.end();
    return image;
}

void showQRCode(QString text) {
    QImage image = GenerateQRCodeImage(text);
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("QR Code");
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QLabel* label = new QLabel(dialog);
    label->setPixmap(QPixmap::fromImage(image));
    layout->addWidget(label);
    dialog->setLayout(layout);
    QTimer::singleShot(6000, dialog, &QDialog::accept);
    dialog->exec();
}

void clearJsonFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
    } else {
        qWarning("Could not open file for writing: %s", qPrintable(file.errorString()));
    }
}

void configWebEngine(QWebEngineView * we) {
    we->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);
    we->settings()->setAttribute(
        QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    we->settings()->setAttribute(
        QWebEngineSettings::LocalStorageEnabled, true);
    we->settings()->setAttribute(
        QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    we->settings()->setAttribute(
        QWebEngineSettings::WebGLEnabled, true);
    we->settings()->setAttribute(
        QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    we->settings()->setAttribute(
        QWebEngineSettings::AllowRunningInsecureContent, true);
    we->settings()->setAttribute(
        QWebEngineSettings::ErrorPageEnabled, true);
    we->settings()->setAttribute(
        QWebEngineSettings::PluginsEnabled, false);
    we->settings()->setAttribute(
        QWebEngineSettings::AllowGeolocationOnInsecureOrigins, false);
}



