#ifndef SEARCHABLECOMBOBOX_H
#define SEARCHABLECOMBOBOX_H

#include <QComboBox>
#include <QLineEdit>
#include <QStringListModel>

class SearchableComboBox : public QComboBox {
    Q_OBJECT

public:
    explicit SearchableComboBox(QWidget *parent = nullptr);

    void addItem(const QString &text);
    void addItems(const QStringList &texts);
    void setEditText(const QString &text);
    QString editText() const;

signals:
    void editTextChanged(const QString &text);
    void currentIndexChanged(int index);

public slots:
    void onTextChanged(const QString &arg1);

private:
    QLineEdit *lineEdit;
    QCompleter *completer;
    QStringListModel *model; // 用于存储下拉框的项
    QStringList originalItems; // 原始项列表

    void showPopup() override;
    void hidePopup() override;
};

#endif // SEARCHABLECOMBOBOX_H
