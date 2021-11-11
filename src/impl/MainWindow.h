#pragma once

#include <QMainWindow>

#include "Tags.h"
#include "SelfContainer.h"
#include "ITranslatorObserver.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow
    : public QMainWindow
    , public SelfContainer<MainWindow>
    , public ITranslatorObserver
{
    Q_OBJECT

public:
    using di_deps = std::tuple<TranslatorTag, LanguageProviderTag>;
    MainWindow(TranslatorPtr translator, LanguageProviderPtr languageProvider);
    ~MainWindow();

    void onLanguageChanged() override;
    void onTranslated(ApiResponse) override;

private:
    void updateUiLanguages();
    void updateTranslatorLanguages();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;

    TranslatorPtr m_translator;
    LanguageProviderPtr m_languageProvider;
};
