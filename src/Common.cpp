#include "Common.h"
#include <QDateTime>


QTextStream* LoadTextFile(const QString& filename)
{
    QFile* flight_file = new QFile(filename);

    if (!flight_file->open(QIODevice::ReadOnly)){
        qInfo() << "open file failed!";
        delete flight_file;
        return nullptr;
    }

    QTextStream* stream = new QTextStream(flight_file);
    return stream;
}


int Duration(QString a, QString b)
{
    QDateTime depTime = QDateTime::fromString(a, "yyyy-MM-dd HH:mm");
    QDateTime arrTime = QDateTime::fromString(b, "yyyy-MM-dd HH:mm");
    qint64 msecs = depTime.msecsTo(arrTime); // 计算时间差，以毫秒为单位
    int seconds = msecs / 1000; // 转换为秒
    return seconds / 60; // 返回分钟数
}
