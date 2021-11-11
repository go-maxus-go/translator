#pragma once

#include <di/tags.h>


class ILanguageProvider;
struct LanguageProviderTag : di::singleton_tag<ILanguageProvider> {};
using LanguageProviderPtr = std::shared_ptr<ILanguageProvider>;

class IApiUrlProvider;
struct ApiUrlProviderTag : di::singleton_tag<IApiUrlProvider> {};
using ApiUrlProviderPtr = std::shared_ptr<IApiUrlProvider>;

class ITranslationQueryBuilder;
struct TranslationQueryBuilderTag : di::singleton_tag<ITranslationQueryBuilder> {};
using TranslationQueryBuilderPtr = std::shared_ptr<ITranslationQueryBuilder>;

class INetworkManager;
struct NetworkManagerTag : di::singleton_tag<INetworkManager> {};
using NetworkManagerPtr = std::shared_ptr<INetworkManager>;

class IApiResponseParser;
struct ApiResponseParserTag : di::singleton_tag<IApiResponseParser> {};
using ApiResponseParserPtr  = std::shared_ptr<IApiResponseParser>;

class ITranslator;
struct TranslatorTag : di::singleton_tag<ITranslator> {};
using TranslatorPtr   = std::shared_ptr<ITranslator>;

class MainWindow;
struct MainWindowTag : di::singleton_tag<MainWindow> {};
using MainWindowPtr = std::shared_ptr<MainWindow>;

