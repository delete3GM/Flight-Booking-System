#include "UserPreferences.h"

UserPreferences::UserPreferences(double priceWeight, double timeWeight , double layoverWeight)
    : priceWeight(priceWeight), timeWeight(timeWeight), layoverWeight(layoverWeight) {}

void UserPreferences::setPriceWeight(double weight) {
    priceWeight = weight;
}

void UserPreferences::setTimeWeight(double weight) {
    timeWeight = weight;
}

void UserPreferences::setLayoverWeight(double weight) {
    layoverWeight = weight;
}

double UserPreferences::getPriceWeight() const {
    return priceWeight;
}

double UserPreferences::getTimeWeight() const {
    return timeWeight;
}

double UserPreferences::getLayoverWeight() const {
    return layoverWeight;
}

UserPreferences UserPreferences::getUserPreferencesFromOrder() {
    // 这里应该是一些逻辑来根据用户的订单信息获取用户的偏好设置
    // 例如，从数据库或文件中读取用户的偏好
    // 这里只是一个示例，我们假设所有用户对价格、时间和转机等待时间的偏好都是相同的






    return UserPreferences(0.8, 0.15, 0.05);
}

