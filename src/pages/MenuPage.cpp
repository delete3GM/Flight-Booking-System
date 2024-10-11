#include "pages/MenuPage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::MenuPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    connect(ui->searchBtn, SIGNAL(released()), this, SLOT(menu2search()));
    connect(ui->userBtn, SIGNAL(released()), this, SLOT(menu2user()));
    connect(ui->exitBtn, SIGNAL(released()), this, SLOT(Exit()));
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
