#include <string>

#include "QDebug"
#include "QFile"
#include "QSqlError"
#include "QApplication"
#include "QThreadPool"

#include "fontconfig/fontconfig.h"

#include "fontdatabase.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

FontDatabase::FontDatabase(QObject *parent) :
    QAbstractItemModel(parent),
    m_fontCount(0),
    m_sql(new SQLHandler()),
    m_fontFamilyList(QList<QString>()),
    m_fontIndex(QList<int>()),
    m_debug(false),
    m_program_name("")
{
    QStringList args = QApplication::arguments();
    if (args.count() > 1) //args.at(0) is program's file name
    {
        //arguments have been filterd in the main function
        m_debug = true;
        m_program_name = args.at(0);
    }

    init();

    connect(m_sql, SIGNAL(done(QList<QString> &, QList<int> &)),
            this, SLOT(handle_sorting_done(QList<QString> &, QList<int> &)));
}

FontDatabase::~FontDatabase()
{
    if (QThreadPool::globalInstance()->activeThreadCount())
        QThreadPool::globalInstance()->waitForDone();

    delete m_sql;
}

int FontDatabase::fontCount() const
{
    return m_fontCount;
}

QString FontDatabase::getFontFamily(int input)
{
    QString query = "select font_family from fonts where id=" + QString::number(input);
    m_sql->setqueryString(query);
    m_sql->exec_string();
    if (!m_sql->getRes())
    {
        return QString("");
    }

    QSqlQuery res = m_sql->getQuery();
    if(res.first())
    {
        return res.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontFilePath(int input)
{
    QString query = "select font_file from fonts where id=" + QString::number(input);
    m_sql->setqueryString(query);
    m_sql->exec_string();
    if (!m_sql->getRes())
    {
        return QString("");
    }

    QSqlQuery res = m_sql->getQuery();
    if(res.first())
    {
        return res.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontFileName(int input)
{
    QString query = "select font_file from fonts where id=" + QString::number(input);
    m_sql->setqueryString(query);
    m_sql->exec_string();
    if (!m_sql->getRes())
    {
        return QString("");
    }

    QSqlQuery res = m_sql->getQuery();
    if(res.first())
    {
        QStringList list = res.value(0).toString().split("/");
        return list.at(list.count() - 1);
    }
    else
    {
        return QString("");
    }
}

QString FontDatabase::getFontStyle(int input)
{
    QString query = "select font_style from fonts where id=" + QString::number(input);
    m_sql->setqueryString(query);
    m_sql->exec_string();
    if (!m_sql->getRes())
    {
        return QString("");
    }

    QSqlQuery res = m_sql->getQuery();
    if(res.first())
    {
        return res.value(0).toString();
    }
    else
    {
        return QString("");
    }
}

void FontDatabase::font_asc_sort()
{
    beginResetModel();
    QString query = "select id, font_family from fonts order by font_family asc";
    m_sql->setqueryString(query);
    m_sql->setfontFamilyList(m_fontFamilyList);
    m_sql->setfontIndex(m_fontIndex);
    m_sql->setfontCount(m_fontCount);
    QThreadPool::globalInstance()->start(m_sql);
}

void FontDatabase::font_desc_sort()
{
    beginResetModel();
    QString query = "select id, font_family from fonts order by font_family desc";
    m_sql->setqueryString(query);
    m_sql->setfontFamilyList(m_fontFamilyList);
    m_sql->setfontIndex(m_fontIndex);
    m_sql->setfontCount(m_fontCount);
    QThreadPool::globalInstance()->start(m_sql);
}

int FontDatabase::getCurrentFontIndex(int input)
{
    return m_fontIndex.at(input);
}

QString FontDatabase::getCurrentFontFamily(int input)
{
    if (input < 0 || input >= m_fontIndex.count()) return QString("");
    return m_fontFamilyList.at(input);
}

//pure virtual functions
int FontDatabase::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fontCount;
}

int FontDatabase::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant FontDatabase::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 ||
        index.row() >= m_fontCount ||
        index.column() < 0 ||
        index.column() > 2)
        return QVariant();

    if (role == IdRole)
        return m_fontIndex.at(index.row());
    else if (role == FamilyRole)
        return m_fontFamilyList.at(index.row());
    return QVariant();
}

QModelIndex FontDatabase::index(int, int, const QModelIndex &) const
{
    return QModelIndex();
}

QModelIndex FontDatabase::parent(const QModelIndex &) const
{
    return QModelIndex();
}

//public slots
void FontDatabase::handle_sorting_done(QList<QString> &fontFamilyList, QList<int> &fontIndex)
{
    if (!m_sql->getRes())
    {
        m_fontCount = 0;
        emit sortingDone();
        return;
    }

    //update data
    m_fontFamilyList = fontFamilyList;
    m_fontIndex = fontIndex;
    m_fontCount = fontIndex.count();

    endResetModel();
    emit sortingDone();
}

//protected menber function
QHash<int, QByteArray> FontDatabase::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[FamilyRole] = "family";
    return roles;
}

//private member function
void FontDatabase::init()
{
    if (!m_sql->getRes())
    {
        m_fontCount = 0;
        if (m_debug) qDebug() << m_program_name << ": " << m_sql->getlastError();
        return;
    }

    if (main_process() != 0)
    {
        m_fontCount = 0;
    }
}

int FontDatabase::main_process()
{
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

    std::string queryStr, font_file, font_family, font_style;
    m_fontCount = fs->nfont;
    for (int i = 0; fs && i < fs->nfont; ++i)
    {
        FcPattern* font = fs->fonts[i];
        FcChar8 *file, *style, *family;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
            FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
            FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch)
        {
            font_file = std::string(file, file + strlen((char *)file));
            font_family = std::string(family, family + strlen((char *)family));
            font_style = std::string(style, style + strlen((char *)style));

            queryStr = "INSERT INTO fonts (id, font_file, font_family, font_style)";
            queryStr += "VALUES (?, ?, ?, ?)";

            SQLiteInfo *sql = m_sql->getSqlInfo();
            std::unique_lock<std::mutex> lock(sql->mutex);

            if (sqlite3_prepare_v2(sql->db,
                                   queryStr.c_str(), queryStr.length(),
                                   &sql->stmt, NULL))
            {
                qCritical() << "Fail to build prepared statment: " << sqlite3_errmsg(sql->db);
                return 1;
            }

            query.prepare(queryStr);
            query.bindValue(":id", i);
            query.bindValue(":file", font_file);
            query.bindValue(":family", font_family);
            query.bindValue(":style", font_style);
            m_sql->setQuery(std::move(query));
            m_sql->exec_prepared();

            if (!m_sql->getRes())
            {
                if (fs) FcFontSetDestroy(fs);
                return 1;
            }
        }
    }

    if (fs) FcFontSetDestroy(fs);
    return 0;
}
