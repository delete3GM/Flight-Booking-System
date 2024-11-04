#pragma once

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDateTime>

using namespace std;


QTextStream* LoadFlightFile(const QString& filename);

int Duration(QString a, QString b);

QString hashPassword(const QString &password);
QString getPasswordById(const QString &id);

QImage GenerateQRCodeImage(const QString& text);
void showQRCode(QString text);

#define USER_FILE                           "D:/CS/projects/Flight_Ticket_Management_System/data/user/user.txt"
#define FLIGHT_FILE							"D:/CS/projects/Flight_Ticket_Management_System/data/flight/flight_data.txt"
#define ORDER_PATH                          "D:/CS/projects/Flight_Ticket_Management_System/data/order/"

#define MAX_DEPTH                            3

typedef enum Sort_Type {
    SORT_NORMAL,											// 默认排序
    SORT_BY_DURA,                                           // 按飞行时长排序
    SORT_BY_TIME,											// 按起飞时间排序
    SORT_BY_PRICE,											// 按票价排序
    SORT_BY_SEAT                                            // 按余座排序
} SORT_TYPE;

typedef enum _tagFlightStatus {
	STATUS_NORMAL,											// 航班状态正常
	STATUS_DELAY,											// 航班状态延误
	STATUS_CANCEL,											// 航班状态取消
	STATUS_COUNT

} FLIGHT_STATUS;

