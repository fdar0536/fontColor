#include <string>

#include "QDebug"
#include "QFile"
#include "QSqlError"
#include "QApplication"

#include "fontconfig/fontconfig.h"

#include "fontdatabase.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using namespace std;

FontDatabase::FontDatabase(QObject *parent) :
    Common(parent),
    m_fontCount(0)
{
    if (main_process() != 0)
    {
        m_fontCount = 0;
    }
}

FontDatabase::~FontDatabase()
{
    m_db.close();
}

int FontDatabase::fontCount() const
{
    return m_fontCount;
}

QString FontDatabase::getFontFamily(int input)
{
    QString query = "select font_family from fonts where id=" + QString::number(input);
    if (!exec_db_string(m_query, query))
    {
        return QString("");
    }
    
    if(m_query.first())
    {
        return m_query.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontFilePath(int input)
{
    QString query = "select font_file from fonts where id=" + QString::number(input);
    if (!exec_db_string(m_query, query))
    {
        return QString("");
    }
    
    if(m_query.first())
    {
        return m_query.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontFileName(int input)
{
    QString query = "select font_file from fonts where id=" + QString::number(input);
    if (!exec_db_string(m_query, query))
    {
        return QString("");
    }
    
    if(m_query.first())
    {
        QStringList res = m_query.value(0).toString().split("/");
        return res.at(res.count() - 1);
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontStyle(int input)
{
    QString query = "select font_style from fonts where id=" + QString::number(input);
    if (!exec_db_string(m_query, query))
    {
        return QString("");
    }
    
    if(m_query.first())
    {
        return m_query.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

//Ascending order
bool FontDatabase::font_asc_sort_init()
{
    m_query_asc.finish();
    m_query_asc = QSqlQuery(m_db);
    QString query = "select id, font_family from fonts order by font_family asc";
    if (exec_db_int(m_query_asc, query) != 0)
    {
        return false;
    }
    
    return m_query_asc.first();
}

int FontDatabase::font_asc_sort_getID()
{
    return font_sort_getID(m_query_asc);
}

QString FontDatabase::font_asc_sort_getFamily()
{
    return font_sort_getFamily(m_query_asc);
}

bool FontDatabase::font_asc_sort_next()
{
    return font_sort_next(m_query_asc);
}

//Descending order
bool FontDatabase::font_desc_sort_init()
{
    m_query_desc.finish();
    m_query_desc = QSqlQuery(m_db);
    QString query = "select id, font_family from fonts order by font_family desc";
    if (exec_db_int(m_query_desc, query) != 0)
    {
        return false;
    }
    
    return m_query_desc.first();
}

int FontDatabase::font_desc_sort_getID()
{
    return font_sort_getID(m_query_desc);
}

QString FontDatabase::font_desc_sort_getFamily()
{
    return font_sort_getFamily(m_query_desc);
}

bool FontDatabase::font_desc_sort_next()
{
    return font_sort_next(m_query_desc);
}

//private member function
int FontDatabase::exec_db_int(QSqlQuery &m_query, QString &query)
{
    if(m_query.exec(query))
    {
        return 0;
    }
    else
    {
        if (m_debug) qDebug() << m_program_name << ": " << m_query.lastError().text();
        return 1;
    }
}

bool FontDatabase::exec_db_string(QSqlQuery &m_query, QString &query)
{
    if(m_query.exec(query))
    {
        return true;
    }
    else
    {
        if (m_debug) qDebug() << m_program_name << ": " << m_query.lastError().text();
        return false;
    }
}

bool FontDatabase::exec_db_string(QSqlQuery &m_query)
{
    if(m_query.exec())
    {
        return true;
    }
    else
    {
        if (m_debug) qDebug() << m_program_name << ": " << m_query.lastError().text();
        return false;
    }
}

int FontDatabase::main_process()
{
#ifdef Q_OS_WIN32
    char *tmp_char = getenv("TEMP");
    if (!tmp_char)
    {
        if (m_debug) qDebug() << m_program_name << ": Fail to get temp directory";
        return 1;
    }
    
    string tmp_path(tmp_char);
    string::size_type str_index = 0;
    while ((str_index = tmp_path.find("\\", str_index)) != string::npos)
    {
        tmp_str.replace(index, 1, "/");
        ++str_index;
    }
#else
    string tmp_path= "/tmp";
#endif
    
    QString db_path = QString::fromStdString(tmp_path + "/fontColor.db");
    if (QFile::exists(db_path))
    {
        QFile file(db_path);
        if (!file.remove())
        {
            if (m_debug) qDebug() << m_program_name << ": Fail to remove database";
            return 1;
        }
    }
    
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(db_path);
    
    if (!m_db.open())
    {
        if (m_debug) qDebug() << m_program_name << ": Fail to open database";
        return 1;
    }
    
    m_query = QSqlQuery(m_db);
    
    QString query;
    query = "drop table if exists fonts";
    if (!exec_db_string(m_query, query))
    {
        return 1;
    }
    
    query = "create table if not exists fonts (";
    query += "id INTEGER,";
    query += "font_file TEXT,";
    query += "font_family TEXT,";
    query += "font_style TEXT)";
    if (!exec_db_string(m_query, query))
    {
        return 1;
    }
    
    FcConfig *config = FcInitLoadConfigAndFonts();
    FcPattern *pat = FcPatternCreate();
    FcObjectSet *os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_LANG, FC_FILE, (char *)0);
    FcFontSet *fs = FcFontList(config, pat, os);
    if (fs == nullptr)
    {
        if (m_debug) qDebug() << m_program_name << ": FcFontList is nullptr";
        return 1;
    }
    
    if (fs->nfont == 0)
    {
        if (m_debug) qDebug() << m_program_name << ": FcFontList->nfont is 0";
        if (fs) FcFontSetDestroy(fs);
        return 1;
    }
    
    QString tmp_string, font_file, font_family, font_style;
    m_fontCount = fs->nfont;
    for (int i = 0; fs && i < fs->nfont; ++i)
    {
        FcPattern* font = fs->fonts[i];
        FcChar8 *file, *style, *family;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
            FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
            FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch)
        {
            font_file = QString::fromStdString(string(file, file + strlen((char *)file)));
            font_family = QString::fromStdString(string(family, family + strlen((char *)family)));
            font_style = QString::fromStdString(string(style, style + strlen((char *)style)));
            
            query = "INSERT INTO fonts (id, font_file, font_family, font_style)";
            query += "VALUES (:id, :file, :family, :style)";
            
            m_query.prepare(query);
            m_query.bindValue(":id", i);
            m_query.bindValue(":file", font_file);
            m_query.bindValue(":family", font_family);
            m_query.bindValue(":style", font_style);
            
            if (!exec_db_string(m_query))
            {
                if (fs) FcFontSetDestroy(fs);
                return 1;
            }
        }
    }
    
    m_db.commit();
    if (fs) FcFontSetDestroy(fs);
    return 0;
}

int FontDatabase::font_sort_getID(QSqlQuery &query)
{
    return query.value(0).toInt();
}

QString FontDatabase::font_sort_getFamily(QSqlQuery &query)
{
    return query.value(1).toString();
}

bool FontDatabase::font_sort_next(QSqlQuery &query)
{
    return query.next();
}
