#include "fontconfig/fontconfig.h"

#include "fontdatabase.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using namespace std;

FontDatabase::FontDatabase(QObject *parent) :
    QObject(parent),
    m_fontCount(0),
    m_nameList(vector<QString>()),
    m_fileList(vector<QString>()),
    m_styleList(vector<QString>())
{
    if (main_process(m_nameList, m_fileList, m_styleList) == 0)
    {
        m_fontCount = static_cast<int>(m_nameList.size());
    }
}

FontDatabase::~FontDatabase()
{}

int FontDatabase::fontCount() const
{
    return m_fontCount;
}

QString FontDatabase::getFontFamily(int input)
{
    return m_nameList.at(static_cast<size_t>(input));
}

QString FontDatabase::getFontFilePath(int input)
{
    return m_fileList.at(static_cast<size_t>(input));
}

QString FontDatabase::getFontFileName(int input)
{
    QStringList res = m_fileList.at(static_cast<size_t>(input)).split("/");
    return res.at(res.count() - 1);
}

QString FontDatabase::getFontStyle(int input)
{
    return m_styleList.at(static_cast<size_t>(input));
}

int FontDatabase::main_process(vector<QString> &nameList, vector<QString> &fileList, vector<QString> &styleList)
{
    FcConfig *config = FcInitLoadConfigAndFonts();
    FcPattern *pat = FcPatternCreate();
    FcObjectSet *os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_LANG, FC_FILE, (char *)0);
    FcFontSet *fs = FcFontList(config, pat, os);
    if (fs == nullptr)
    {
        return 1;
    }
    
    if (fs->nfont == 0)
    {
        if (fs) FcFontSetDestroy(fs);
        return 1;
    }
    
    nameList.reserve(static_cast<size_t>(fs->nfont));
    fileList.reserve(static_cast<size_t>(fs->nfont));
    styleList.reserve(static_cast<size_t>(fs->nfont));
    QString tmp_string;
    for (int i=0; fs && i < fs->nfont; ++i)
    {
        FcPattern* font = fs->fonts[i];
        FcChar8 *file, *style, *family;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
            FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
            FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch)
        {
            nameList.push_back(QString::fromStdString(string(family, family + strlen((char *)family))));
            fileList.push_back(QString::fromStdString(string(file, file + strlen((char *)file))));
            styleList.push_back(QString::fromStdString(string(style, style + strlen((char *)style))));
        }
    }
    
    if (fs) FcFontSetDestroy(fs);
    return 0;
}
