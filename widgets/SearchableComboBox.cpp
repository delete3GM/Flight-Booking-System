#include "SearchableComboBox.h"
#include <QStringListModel>

SearchableComboBox::SearchableComboBox(QWidget *parent) : QComboBox(parent) {
    setupUI();
    setupCompleter();
}

void SearchableComboBox::setupUI() {
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);
    setMaxVisibleItems(10);
    setMinimumContentsLength(10);
    setFixedWidth(160);

    setStyleSheet(
        "QComboBox {"
        "    padding: 5px;"
        "    border: 1px solid #ccc;"
        "    border-radius: 3px;"
        "    background: white;"
        "    font-size: 16px;"
        "}"
        "QComboBox:hover {"
        "    border: 1px solid #0078d4;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ccc;"
        "    selection-background-color: #0078d4;"
        "    selection-color: white;"
        "}"
        );
}

void SearchableComboBox::setupCompleter() {
    completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    setCompleter(completer);
}

void SearchableComboBox::initializeBox(const QVector<QString>& cities, const QMap<QString, CityInfo>& cityInfoMap) {
    clear();
    this->cityInfoMap = cityInfoMap;

    for (const QString& city : cities) {
        addItem(city);
    }
    QStringList cityList = cities.toList();
    completer->setModel(new QStringListModel(cityList));

    if (!cities.isEmpty()) {
        setCurrentIndex(0);
    }
    connect(this, &QComboBox::currentTextChanged, this, &SearchableComboBox::onTextChanged);
}

QString SearchableComboBox::getCurrentCity() const {
    return currentText();
}

void SearchableComboBox::onTextChanged(const QString& text) {
    emit citySelected(text);
}
