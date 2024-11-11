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

void clearJsonFile(const QString &filePath);

#define USER_FILE                           "D:/CS/projects/Flight_Ticket_Management_System/data/user/user.txt"
#define FLIGHT_FILE							"D:/CS/projects/Flight_Ticket_Management_System/data/flight/flight_data_new.txt"
#define ORDER_PATH                          "D:/CS/projects/Flight_Ticket_Management_System/data/order/"
#define CITY_FILE                           "D:/CS/projects/Flight_Ticket_Management_System/data/flight/city_info.csv"
#define PYTHON_FILE                         "D:/CS/projects/Flight_Ticket_Management_System/data/flight/get_weather_info.exe"

#define MAX_DEPTH                           3
class Utils {
public:
    static QSet<QString> foreignCities; // 声明
};


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

struct CityInfo {
    double latitude;
    double longitude;
    QString weather;
    QString temperature;
    QString wind;
};

