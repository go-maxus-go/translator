#include "Module.h"

#include "Tags.h"

#include "MainWindow.h"
#include "Translator.h"
#include "ApiUrlProvider.h"
#include "NetworkManager.h"
#include "LanguageProvider.h"
#include "ApiResponseParser.h"
#include "TranslationQueryBuilder.h"


di::context createContext()
{
    di::context context;

    context.put<MainWindowTag, MainWindow>();
    context.put<TranslatorTag, Translator>();
    context.put<ApiUrlProviderTag, ApiUrlProvider>();
    context.put<NetworkManagerTag, NetworkManager>();
    context.put<LanguageProviderTag, LanguageProvider>();
    context.put<ApiResponseParserTag, ApiResponseParser>();
    context.put<TranslationQueryBuilderTag, TranslationQueryBuilder>();

    return context;
}
