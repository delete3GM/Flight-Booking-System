#pragma once

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDateTime>

using namespace std;

//加载航班文件
QTextStream* LoadTextFile(const QString& filename);
//计算飞行时长
int Duration(QString a, QString b);

// 默认的航班数据路径
#define FLIGHT_FILE							"D:/CS/projects/Flight_Ticket_Management_System/data/flight/flight_data.txt"
// 订单数据路径
#define ORDER_PATH                           "D:/CS/projects/Flight_Ticket_Management_System/data/order/"

// 排序规则
typedef enum Sort_Type
{
    SORT_NORMAL,											// 默认排序
    SORT_BY_DURA,                                           // 按飞行时长排序
    SORT_BY_TIME,											// 按起飞时间排序
    SORT_BY_PRICE,											// 按票价排序
    SORT_BY_SEAT                                            // 按余座排序


} SORT_TYPE;



// 航班状态
typedef enum _tagFlightStatus
{
	STATUS_NORMAL,											// 航班状态正常
	STATUS_DELAY,											// 航班状态延误
	STATUS_CANCEL,											// 航班状态取消
	STATUS_COUNT

} FLIGHT_STATUS;




