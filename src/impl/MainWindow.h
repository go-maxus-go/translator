#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>

#include "Tags.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using di_deps = std::tuple<LanguageProviderTag>;
    MainWindow(LanguageProviderPtr languageProvider);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QNetworkAccessManager networkAccessManager;
};
