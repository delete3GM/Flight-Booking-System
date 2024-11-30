#include "pages/MenuPage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::MenuPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    connect(ui->searchBtn, &QPushButton::released, this, &MenuPage::menu2search);
    connect(ui->userBtn, &QPushButton::released, this, &MenuPage::menu2user);
    connect(ui->exitBtn, &QPushButton::released, this, &MenuPage::Exit);
    mainWindow->currentUser.updateVIPLevel();
    ui->userLbl->setText("用户:" + mainWindow->currentUser.getID() + "\nVIP " +
                         QString::number(mainWindow->currentUser.getVIPLevel()));
    QPixmap pixmap(":/images/resources/images/user.png");
    ui->iconLbl->setPixmap(pixmap);
    ui->iconLbl->setScaledContents(true);

    QPixmap backgroundImage(":/images/resources/images/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

MenuPage::~MenuPage() {
    delete ui;
}
void MenuPage::menu2search() {
    mainWindow->showSearchPage();
}

void MenuPage::menu2user() {
    mainWindow->showUserPage();
}

void MenuPage::Exit() {
    mainWindow->exitWindow();
}
