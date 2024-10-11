#include "Utils.h"
#include "qcryptographichash.h"
#include <QDateTime>


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

int Duration(QString a, QString b) {
    QDateTime depTime = QDateTime::fromString(a, "yyyy-MM-dd HH:mm");
    QDateTime arrTime = QDateTime::fromString(b, "yyyy-MM-dd HH:mm");

    qint64 msecs = depTime.msecsTo(arrTime); // 计算时间差，以毫秒为单位
    int seconds = msecs / 1000; // 转换为秒
    return seconds / 60; // 返回分钟数
}

QString hashPassword(const QString &password) {
    QByteArray passwordData = password.toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha256); // 使用SHA-256哈希算法
    hash.addData(passwordData);
    QByteArray hashResult = hash.result();
    return QString(hashResult.toHex());
}

QString getPasswordById(const QString &id) {
    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << USER_FILE;
        return QString(); // 返回空字符串表示操作失败
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(' ');

        // 确保该行包含ID和密码两个字段
        if (fields.size() == 2) {
            QString fileId = fields[0];
            QString password = fields[1];

            // 如果找到匹配的ID，返回对应的密码
            if (fileId == id) {
                file.close();
                return password;
            }
        }
    }
    file.close();
    return QString();
}


