#include "webserver.h"
#include <iostream>
#include <QDebug>

using namespace httplib;

WebServer::WebServer(const char* host, int port, const char* web_root, QObject *parent)
    : QObject(parent), host_(host), port_(port), web_root_(web_root), running_(false), thread_(new QThread(this)) {
    moveToThread(thread_);
    connect(thread_, &QThread::started, this, &WebServer::run);
    connect(this, &WebServer::serverStopped, thread_, &QThread::quit);
}

WebServer::~WebServer() {
    stop();
    thread_->wait();  // Ensure the thread has stopped before deleting the manager
}

void WebServer::start() {
    if (!running_) {
        thread_->start();
    }
}

void WebServer::stop() {
    if (running_) {
        running_ = false;
        server_.stop();
        emit serverStopped();
    }
}

void WebServer::run() {
    running_ = true;
    // Set the base directory to serve files from
    qInfo() << "Serving files from " << QString::fromUtf8(web_root_);
    server_.set_mount_point("/", web_root_);

    // server_.Get("/app.html#/:page", [](const Request & req, Response &res) {
    //     auto page = req.path_params.at("page");
    //     qInfo() << "Serving page: " << page.c_str();
    //     //res.set_redirect("http://localhost:8000/app.html#" + page);
    // });

    emit serverStarted();
    if (!server_.listen(host_, port_)) {
        std::cerr << "Failed to start server on " << host_ << ":" << port_ << "\n";
        emit serverStopped();
        return;
    }

    emit serverStopped();
}
