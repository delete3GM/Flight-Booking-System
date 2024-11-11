#include "CitySelectionDialog.h"
#include "QEvent.h"

CitySelectionDialog::CitySelectionDialog(const QVector<QString>& cities, QWidget* parent)
    : QDialog(parent), m_cityNames(cities) {
    setWindowTitle("选择城市");
    //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //setModal(false);
    m_layout = new QGridLayout(this);
    int rows = (cities.size() + 2) / 4; // 显示4列
    for (int i = 0; i < cities.size(); ++i) {
        QPushButton* cityButton = new QPushButton(cities[i], this); // 假设 City 类有一个 getName() 方法
        connect(cityButton, &QPushButton::clicked, this, &CitySelectionDialog::onCityButtonClicked);
        m_layout->addWidget(cityButton, i / 4, i % 4); // 放置在网格中
    }
}

void CitySelectionDialog::onCityButtonClicked() {
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        m_selectedCity = senderButton->text();
        emit citySelected(m_selectedCity);
        accept(); // 关闭对话框
    }
}

void CitySelectionDialog::mousePressEvent(QMouseEvent* event) {
    // 检查点击是否在对话框内部
    if (!rect().contains(event->pos())) {
        reject(); // 关闭对话框
    }
    QDialog::mousePressEvent(event); // 调用基类的事件处理
}
