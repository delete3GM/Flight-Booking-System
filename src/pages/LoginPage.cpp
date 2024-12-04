#include "qcompleter.h"
#include "qprocess.h"
#include "ui_loginpage.h"
#include "pages/LoginPage.h"
#include "flight_ticket_management_system.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include "Utils.h"
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDir>

LoginPage::LoginPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initLoginPage();

    connect(ui->loginBtn, SIGNAL(released()), this, SLOT(Login()));
    connect(ui->signupBtn, SIGNAL(released()), this, SLOT(signUp()));
    connect(ui->changePswBtn, &QLabel::linkActivated, this, &LoginPage::changePsw);

    toggleAction = ui->typePsw->addAction(QIcon(":/images/resources/images/eye_close.png"), QLineEdit::TrailingPosition);
    connect(toggleAction, &QAction::triggered, this, &LoginPage::togglePasswordVisibility);
}

LoginPage::~LoginPage() {
    delete ui;
}

void LoginPage::initLoginPage() {
    QString scriptPath = PYTHON_FILE;
    executePythonScript(scriptPath);
    ui->typeAcnt->clear();
    ui->typePsw->clear();
    ui->typeAcnt->setFocusPolicy(Qt::ClickFocus);
    ui->typePsw->setFocusPolicy(Qt::ClickFocus);
    ui->typePsw->setEchoMode(QLineEdit::Password);
    ui->typeAcnt->setPlaceholderText("请输入身份证号");
    ui->typePsw->setPlaceholderText("请输入密码");

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(
        QRegularExpression("^[1-9]\\d{5}(18|19|20)\\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\\d{3}[0-9Xx]$"), this);
    ui->typeAcnt->setValidator(validator);
    ui->changePswBtn->setText("<a href='#'>忘记密码？</a>");
    loadCityInfo();

    QPixmap backgroundImage(":/images/resources/images/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    mainWindow->network.clearData();
    mainWindow->network.readFlightFromFile(FLIGHT_FILE);
}

QString LoginPage::getID() const {
    return ui->typeAcnt->text();
}

QString LoginPage::getPassword() const {
    return ui->typePsw->text();
}

bool LoginPage::isUserRegistered(const QString& id, const QString& psw) {
    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件: " << USER_FILE;
        return false;
    }
    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() == 2 && fields[0] == id && fields[1] == hashPassword(psw)) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void LoginPage::Login() {
    mainWindow->currentUser.setID(getID());
    QString Psw = getPassword();
    if (mainWindow->currentUser.getID().isEmpty() || Psw.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "身份证号或密码不能为空！");
        return;
    }
    if (loginAttempts >= MAX_LOGIN_ATTEMPTS) {
        QMessageBox::warning(this, "登录失败", "已达最大尝试次数，账户已被锁定！");
        mainWindow->exitWindow();
        return;
    }
    if (isUserRegistered(mainWindow->currentUser.getID(), Psw)) {
        QMessageBox::information(this, "登录成功", "欢迎回来！");
        mainWindow->currentUser.loadUserOrders();
        mainWindow->showMenuPage();
        loginAttempts = 0;
    } else {
        QMessageBox::warning(this, "登录失败", "身份证号或密码不正确！");
        loginAttempts++;
    }
}

void LoginPage::writeUserToFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "无法打开文件: " << filename;
        return;
    }
    QTextStream out(&file);
    QString id = getID();
    QString psw = getPassword();
    out << id << " " << hashPassword(psw) << "\n";
    file.close();
}

void LoginPage::signUp() {
    QString id = getID();
    QString psw = getPassword();

    if (id.isEmpty() || psw.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "身份证号或密码不能为空！");
        return;
    }

    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "注册失败", "无法打开用户文件！");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() >= 1 && fields[0] == id) {
            file.close();
            QMessageBox::warning(this, "注册失败", "该身份证号已被注册！");
            return;
        }
    }
    file.close();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "注册失败", "无法写入用户文件！");
        return;
    }
    QTextStream out(&file);
    out << id << " " << hashPassword(psw) << "\n";
    file.close();

    QMessageBox::information(this, "注册成功", "用户注册成功！");
    mainWindow->currentUser.setID(id);
    mainWindow->currentUser.loadUserOrders();
    mainWindow->showMenuPage();
}

void LoginPage::changePsw() {
    QString currentId = getID();

    if (currentId.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入身份证号！");
        return;
    }

    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开用户文件！");
        return;
    }
    bool userFound = false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() == 2 && fields[0] == currentId) {
            userFound = true;
            break;
        }
    }
    file.close();
    if (!userFound) {
        QMessageBox::warning(this, "错误", "当前用户不存在，无法修改密码！");
        initLoginPage();
        return;
    }
    showChangePasswordDialog();
}

bool LoginPage::updatePassword(const QString &newPassword, const QString &confirmPassword) {
    if (newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "所有字段都不能为空！");
        return false;
    }
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "错误", "新密码和确认密码不一致！");
        return false;
    }
    QString oldPassword = getPasswordById(getID());

    if (oldPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "未找到用户记录！");
        return false;
    }
    if (hashPassword(newPassword) == oldPassword) {
        QMessageBox::warning(this, "错误", "新密码不能与旧密码相同！");

        return false;
    }

    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "无法打开文件: " << USER_FILE;
        QMessageBox::warning(this, "错误", "无法打开用户文件！");
        return false;
    }

    QTextStream in(&file);
    QStringList updatedLines;
    bool passwordUpdated = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() == 2 && fields[0] == getID()) {
            fields[1] = hashPassword(newPassword);
            passwordUpdated = true;
        }
        updatedLines.append(fields.join(" "));
    }
    file.close();

    if (!passwordUpdated) {
        QMessageBox::warning(this, "错误", "未找到当前用户，无法更新密码！");
        return false;
    }

    // 写回文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件进行写入: " << USER_FILE;
        QMessageBox::warning(this, "错误", "无法写入用户文件！");
        return false;
    }
    QTextStream out(&file);
    foreach (const QString &line, updatedLines) {
        out << line << "\n";
    }
    file.close();
    return true;
}

void LoginPage::showChangePasswordDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("修改密码");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *newPasswordInput = new QLineEdit(&dialog);
    newPasswordInput->setEchoMode(QLineEdit::Password);
    newPasswordInput->setPlaceholderText("请输入新密码");
    layout->addWidget(newPasswordInput);

    QLineEdit *confirmPasswordInput = new QLineEdit(&dialog);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setPlaceholderText("请确认新密码");
    layout->addWidget(confirmPasswordInput);

    QPushButton *okButton = new QPushButton("确认", &dialog);
    layout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, [this, &dialog, newPasswordInput, confirmPasswordInput]() {
        QString newPassword = newPasswordInput->text();
        QString confirmPassword = confirmPasswordInput->text();

        if (updatePassword(newPassword, confirmPassword)) {
            QMessageBox::information(this, "成功", "密码修改成功！");
            dialog.accept();
            ui->typePsw->clear();
        }
    });
    dialog.exec();
}

void LoginPage::togglePasswordVisibility() {
    QIcon eye_close(":/images/resources/images/eye_close.png");
    QIcon eye_open(":/images/resources/images/eye_open.png");
    if (ui->typePsw->echoMode() == QLineEdit::Password) {
        ui->typePsw->setEchoMode(QLineEdit::Normal);
        QPixmap pixmap = eye_open.pixmap(64, 64);
        toggleAction->setIcon(QIcon(pixmap));
    } else {
        ui->typePsw->setEchoMode(QLineEdit::Password);
        QPixmap pixmap = eye_close.pixmap(64, 64);
        toggleAction->setIcon(QIcon(pixmap));
    }
}

void LoginPage::executePythonScript(const QString &scriptPath) {
    QProcess *process = new QProcess();
    QFileInfo exeFileInfo(scriptPath);
    if (!exeFileInfo.exists() || !exeFileInfo.isFile()) {
        qWarning() << "Executable file does not exist or is not a file:" << scriptPath;
        delete process;
        return;
    }
    process->setProgram(scriptPath);
    process->setWorkingDirectory(exeFileInfo.dir().path());
    process->start();

    if (!process->waitForFinished(5000)) {
        qWarning() << "Error:" << process->errorString();
    } else {
        //qDebug() << "Process finished successfully.";
    }
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
}

void LoginPage::loadCityInfo() {
    QFile file(CITY_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件";
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() >= 6) {
            QString cityName = parts[0].trimmed();
            CityInfo info;
            info.latitude = parts[1].trimmed().toDouble();
            info.longitude = parts[2].trimmed().toDouble();
            info.weather = parts[3].trimmed();
            info.temperature = parts[4].trimmed();
            info.wind = parts[5].trimmed();

            mainWindow->cityInfoMap.insert(cityName, info);
        }
    }
    file.close();
}


