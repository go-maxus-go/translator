#include "Module.h"

#include "Tags.h"

#include "MainWindow.h"
#include "ApiUrlProvider.h"
#include "LanguageProvider.h"
#include "TranslationQueryBuilder.h"


di::context createContext()
{
    di::context context;

    context.put<MainWindowTag, MainWindow>();
    context.put<ApiUrlProviderTag, ApiUrlProvider>();
    context.put<LanguageProviderTag, LanguageProvider>();
    context.put<TranslationQueryBuilderTag, TranslationQueryBuilder>();

    return context;
}
