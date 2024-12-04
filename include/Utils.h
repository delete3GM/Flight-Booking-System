#pragma once

#include "qwebengineview.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDateTime>

using namespace std;


QTextStream* LoadFlightFile(const QString& filename);

int Duration(QString a, QString b);
QString formatMinutes(int totalMinutes);

QString hashPassword(const QString &password);
QString getPasswordById(const QString &id);

QImage GenerateQRCodeImage(const QString& text);
void showQRCode(QString text);

void clearJsonFile(const QString &filePath);

void configWebEngine(QWebEngineView * we);


#define FLIGHT_FILE							"../data/flight/flight_data_final.txt"
#define CITY_FILE                           "../data/flight/city_info.csv"
#define PYTHON_FILE                         "../data/flight/get_weather_info.exe"

#define USER_FILE                           "../data/user.txt"
#define ORDER_PATH                          "../data/order/"

#define MAX_DEPTH                           2
#define MAX_RECOMMENDATIONS                 6

#define NORMAL_MEAL_PRICE                   35
#define PLUS_MEAL_PRICE                     80

#define SMOOTHING_FACTOR                    0.2

extern QSet<QString> foreignCities;
extern QMap<QString, double> mealPrice;
extern QMap<QString, double> cabinPrice;


typedef enum Sort_Type {
    SORT_NORMAL,											// 默认排序
    SORT_BY_DURA,                                           // 按飞行时长排序
    SORT_BY_TIME,											// 按起飞时间排序
    SORT_BY_PRICE,											// 按票价排序
    SORT_BY_PERSON                                          // 智能个性化排序
} SORT_TYPE;

typedef enum Flight_Status {
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

