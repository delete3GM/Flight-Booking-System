#ifndef ORDERDETAILSDIALOG_H
#define ORDERDETAILSDIALOG_H


#include "Order.h"
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>

class OrderDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit OrderDetailsDialog(const Order& order, QWidget* parent = nullptr);
    void setupUI();

private:
    Order order;
    QVBoxLayout* layout;
    QLabel* orderDetailsLabel;
};
#endif // ORDERDETAILSDIALOG_H
