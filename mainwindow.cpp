#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "optionsmanager.h"
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QWebEngineSettings>
#include <QInputDialog>
#include <QMessageBox>

#define PAGE_PATH "/app.html"

#ifndef SERVER_ROOT
#define SERVER_ROOT "./assets"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , webViewManager(new WebViewManager(this))
    , server(new WebServer("0.0.0.0", 8000, SERVER_ROOT))
{
    ui->setupUi(this);
    webViewManager->hide();

    qInfo() << "Server Root: " << SERVER_ROOT;

    ui->tabWidget->setTabText(0,"Trascrittore");
    ui->tabWidget->addTab(webViewManager->getWebEngineView(), "Chatbot Manager");
    ui->tabWidget->setTabVisible(1,false);
    webViewManager->loadPage(PAGE_PATH);

    connect(server, &WebServer::serverStarted, [](){
        qInfo() << "Server started";
    });

    server->start();

    // Setup toolbar actions
    setupToolbarActions();

    // Load saved chatbots
    loadChatbots();

    if(this->chatbots.size() > 0) {
        ui->tabWidget->setTabVisible(1,true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppQuit()
{
    server->stop();
}

void MainWindow::changePage(const QString &page)
{
    qInfo() << "Loading page: " << page;
    webViewManager->setLocalUrl(page);
}

void MainWindow::on_actionLogout_triggered()
{
    webViewManager->logout();
}

void MainWindow::onPageLoaded(int page)
{
}

void MainWindow::setupToolbarActions()
{
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolBar->addWidget(spacer);

    QWidget *dummy = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 5, 0);


    chatbotComboBox = new QComboBox(this);
    chatbotComboBox->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    chatbotComboBox->resize(chatbotComboBox->width() + 20, chatbotComboBox->height() + 20);
    chatbotComboBox->setMinimumWidth(150);
    chatbotComboBox->setMinimumHeight(10);
    layout->addWidget(new QLabel("Chatbot: ", this));
    layout->addWidget(chatbotComboBox);
    dummy->setLayout(layout);
    ui->toolBar->addWidget(dummy);
    connect(chatbotComboBox, &QComboBox::currentTextChanged, this, &MainWindow::selectChatbot);
}

void MainWindow::addChatbot()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add Chatbot"),
                                         tr("Chatbot Name:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !name.isEmpty()) {
        QString url = QInputDialog::getText(this, tr("Add Chatbot"),
                                            tr("Base URL:"), QLineEdit::Normal,
                                            "", &ok);
        if (ok && !url.isEmpty()) {
            chatbots[url] = name;
            chatbotComboBox->addItem(name);
            OptionsManager::instance().saveChatbots(chatbots);
            chatbotComboBox->setCurrentText(name);
            selectChatbot(name);
            if(chatbots.size() >= 1) {
                ui->tabWidget->setTabVisible(1,true);
            }
        }
    }
}

void MainWindow::deleteChatbot()
{
    bool ok;
    QStringList items = chatbots.values();
    QString name = QInputDialog::getItem(this, tr("Delete Chatbot"),
                                         tr("Chatbot Name:"), items, 0, false, &ok);
    if (ok && !name.isEmpty()) {
        QString url = chatbots.key(name);
        chatbots.remove(url);
        chatbotComboBox->removeItem(chatbotComboBox->findText(name));
        OptionsManager::instance().saveChatbots(chatbots);
    }
}

void MainWindow::selectChatbot(const QString &name)
{
    QString url = chatbots.key(name);
    if (currentChatbot == url) return;
    currentChatbot = url;
    webViewManager->setBaseUrl(url);
    webViewManager->setCurrentChatbotName(name);
    webViewManager->loadPage(PAGE_PATH);
}

void MainWindow::loadChatbots()
{
    chatbots = OptionsManager::instance().getChatbots();
    for (const QString &name : chatbots.values()) {
        chatbotComboBox->addItem(name);
    }
}

void MainWindow::on_actionNuovo_Chatbot_triggered()
{
    this->addChatbot();
}

void MainWindow::on_actionRimuovi_Chatbot_triggered()
{
    this->deleteChatbot();
}
