#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QUrl>
#include <QList>
#include <QString>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const auto languages = QStringList({"en", "ru", "de", "fr"});
    ui->fromComboBox->insertItems(0, languages);
    ui->toComboBox->insertItems(0, languages);

    ui->fromComboBox->setCurrentIndex(0);
    ui->toComboBox->setCurrentIndex(2);

    connect(ui->translateButton, &QPushButton::pressed, this, [this](){

        QUrl url("https://api.mymemory.translated.net/get");
        QUrlQuery query;

        auto inputText = ui->inputText->toPlainText();
        auto languageFrom = ui->fromComboBox->currentText();
        auto languageTo = ui->toComboBox->currentText();

        query.addQueryItem("q", inputText);
        auto languagePair = QString("%1|%2").arg(languageFrom, languageTo);
        query.addQueryItem("langpair", languagePair);
        url.setQuery(query.query());

        QNetworkRequest request(url);

        auto reply = networkAccessManager.get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply](){
            auto data = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
            QJsonObject jsonObject = jsonResponse.object();
            auto responseData = jsonObject["responseData"].toObject();
            auto translation = responseData["translatedText"].toString();
            auto translationLines = translation.split("&#10; ");
            auto translationPretty = translationLines.join("\n");
            ui->translationText->setText(translationPretty);
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
