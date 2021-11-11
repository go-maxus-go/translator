#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QString>
#include <QComboBox>

#include "ITranslator.h"
#include "ILanguageProvider.h"


MainWindow::MainWindow(
    TranslatorPtr translator,
    LanguageProviderPtr languageProvider)
    : QMainWindow(nullptr)
    , SelfContainer<MainWindow>(this)
    , m_ui(std::make_unique<Ui::MainWindow>())
    , m_translator{std::move(translator)}
    , m_languageProvider{std::move(languageProvider)}
{
    m_ui->setupUi(this);

    m_translator->attach(self());

    const auto languages = m_languageProvider->availableLanguages();
    m_ui->fromComboBox->addItems(languages);
    m_ui->toComboBox->addItems(languages);
    updateUiLanguages();

    connect(m_ui->translateButton, &QPushButton::pressed, this, [this](){
        m_translator->translate(m_ui->inputText->toPlainText());
    });

    connect(m_ui->fromComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](auto){ updateTranslatorLanguages(); });

    connect(m_ui->toComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](auto){ updateTranslatorLanguages(); });
}

MainWindow::~MainWindow() = default;

void MainWindow::onLanguageChanged()
{
    updateUiLanguages();
}

void MainWindow::onTranslated(ApiResponse apiResponse)
{
    m_ui->translationText->setText(apiResponse.translation);
}

void MainWindow::updateUiLanguages()
{
    const auto languages = m_languageProvider->availableLanguages();

    const auto fromIndex = languages.indexOf(m_translator->sourceLanguage());
    m_ui->fromComboBox->setCurrentIndex(fromIndex);

    const auto toIndex = languages.indexOf(m_translator->targetLanguage());
    m_ui->toComboBox->setCurrentIndex(toIndex);
}

void MainWindow::updateTranslatorLanguages()
{
    const auto languages = m_languageProvider->availableLanguages();

    const auto fromIndex = m_ui->fromComboBox->currentIndex();
    m_translator->setSourceLanguage(languages[fromIndex]);

    const auto toIndex = m_ui->toComboBox->currentIndex();
    m_translator->setTargetLanguage(languages[toIndex]);
}
