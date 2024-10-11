/********************************************************************************
** Form generated from reading UI file 'mappage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPPAGE_H
#define UI_MAPPAGE_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapPage
{
public:
    QGridLayout *gridLayout;
    QComboBox *airlineComb;
    QPushButton *backUserBtn;
    QSpacerItem *horizontalSpacer;
    QWebEngineView *webEngineView;

    void setupUi(QWidget *MapPage)
    {
        if (MapPage->objectName().isEmpty())
            MapPage->setObjectName("MapPage");
        MapPage->resize(960, 540);
        MapPage->setMinimumSize(QSize(960, 540));
        gridLayout = new QGridLayout(MapPage);
        gridLayout->setObjectName("gridLayout");
        airlineComb = new QComboBox(MapPage);
        airlineComb->setObjectName("airlineComb");

        gridLayout->addWidget(airlineComb, 0, 0, 1, 1);

        backUserBtn = new QPushButton(MapPage);
        backUserBtn->setObjectName("backUserBtn");

        gridLayout->addWidget(backUserBtn, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(795, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        webEngineView = new QWebEngineView(MapPage);
        webEngineView->setObjectName("webEngineView");
        webEngineView->setProperty("url", QVariant(QUrl(QString::fromUtf8("about:blank"))));

        gridLayout->addWidget(webEngineView, 1, 0, 1, 3);


        retranslateUi(MapPage);

        QMetaObject::connectSlotsByName(MapPage);
    } // setupUi

    void retranslateUi(QWidget *MapPage)
    {
        MapPage->setWindowTitle(QCoreApplication::translate("MapPage", "Form", nullptr));
        backUserBtn->setText(QCoreApplication::translate("MapPage", "back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapPage: public Ui_MapPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPPAGE_H
