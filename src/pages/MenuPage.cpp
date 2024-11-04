#include "pages/MenuPage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::MenuPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    connect(ui->searchBtn, &QPushButton::released, this, &MenuPage::menu2search);
    connect(ui->userBtn, &QPushButton::released, this, &MenuPage::menu2user);
    connect(ui->exitBtn, &QPushButton::released, this, &MenuPage::Exit);
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
