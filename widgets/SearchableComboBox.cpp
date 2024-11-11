#include "SearchableComboBox.h"
#include <QCompleter>
#include <QAbstractItemView>

SearchableComboBox::SearchableComboBox(QWidget *parent) : QComboBox(parent) {
    this->setEditable(true); // 允许编辑
    lineEdit = new QLineEdit(this); // 获取 QComboBox 的 QLineEdit
    this->setLineEdit(lineEdit); // 将 QLineEdit 设置为 QComboBox 的编辑器

    // 初始化模型
    model = new QStringListModel(this);
    completer = new QCompleter(model, this);
    completer->setCompletionMode(QCompleter::PopupCompletion); // 设置补全模式为弹出窗口
    completer->setCaseSensitivity(Qt::CaseInsensitive); // 设置为不区分大小写
    completer->setFilterMode(Qt::MatchContains); // 设置为包含匹配模式，实现模糊匹配
    lineEdit->setCompleter(completer); // 将 QCompleter 设置给 QLineEdit

    connect(lineEdit, &QLineEdit::textChanged, this, &SearchableComboBox::onTextChanged);
    connect(this, &QComboBox::activated, this, &SearchableComboBox::currentIndexChanged);
}

void SearchableComboBox::addItem(const QString &text) {
    QComboBox::addItem(text);
    originalItems.append(text); // 保存原始项
    model->setStringList(originalItems); // 更新模型
}

void SearchableComboBox::addItems(const QStringList &texts) {
    QComboBox::addItems(texts);
    originalItems.append(texts); // 保存原始项
    model->setStringList(originalItems); // 更新模型
}

void SearchableComboBox::setEditText(const QString &text) {
    lineEdit->setText(text);
}

QString SearchableComboBox::editText() const {
    return lineEdit->text();
}

void SearchableComboBox::onTextChanged(const QString &arg1) {
    emit editTextChanged(arg1);
    // 筛选出匹配的项
    QStringList filteredItems;
    for (const QString &item : originalItems) {
        if (item.contains(arg1, Qt::CaseInsensitive)) { // 匹配不区分大小写
            filteredItems.append(item);
        }
    }
    model->setStringList(filteredItems); // 更新模型以显示匹配项
    if (!filteredItems.isEmpty()) {
        showPopup(); // 显示下拉框
    } else {
        hidePopup(); // 隐藏下拉框
    }
}

void SearchableComboBox::showPopup() {
    QComboBox::showPopup();
}

void SearchableComboBox::hidePopup() {
    QComboBox::hidePopup();
}
