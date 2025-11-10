#include <string>
#include "sqlhandler.hpp"

#include "QFile"
#include "QVariant"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

SQLiteInfo::~SQLiteInfo()
{
    std::unique_lock<std::mutex> lock(mutex);

    if (stmt)
    {
        static_cast<void>(sqlite3_finalize(stmt));
        stmt = nullptr;
    }

    if (db)
    {
        static_cast<void>(sqlite3_close(db));
        db = nullptr;
    }
}

SQLHandler::SQLHandler() :
    m_queryString(""),
    m_lastError(QString("")),
    m_res(true)
{
    db_init();
    this->setAutoDelete(false);
}
SQLHandler::~SQLHandler()
{}

void SQLHandler::setqueryString(QString &input)
{
    m_queryString = input.toUtf8().toStdString();
}

void SQLHandler::setfontFamilyList(QList<QString> &fontFamilyList)
{
    m_fontFamilyList = fontFamilyList;
}

void SQLHandler::setfontIndex(QList<int> &fontIndex)
{
    m_fontIndex = fontIndex;
}

void SQLHandler::setfontCount(int &fontCount)
{
    m_fontCount = fontCount;
}

bool SQLHandler::getRes() const
{
    return m_res;
}

QString SQLHandler::getlastError() const
{
    return m_lastError;
}

SQLiteInfo *SQLHandler::getSqlInfo()
{
    return &m_sql;
}

//run
void SQLHandler::run()
{
    exec_string();
    if (!m_res)
    {
        static_cast<void>(sqlite3_finalize(m_sql.stmt));
        m_sql.stmt = nullptr;
        emit done(m_fontFamilyList, m_fontIndex);
    }

    static_cast<void>(sqlite3_finalize(m_sql.stmt));
    m_sql.stmt = nullptr;

    m_fontFamilyList.clear();
    m_fontIndex.clear();
    m_fontFamilyList.reserve(m_fontCount);
    m_fontIndex.reserve(m_fontCount);

    QSqlQuery style_res = QSqlQuery(m_db);
    QString style_query;
    int index(0);
    for (index = 0; index < m_fontCount; ++index)
    {
        int id = m_query.value(0).toInt();
        QString family = m_query.value(1).toString();

        style_query = "select font_style from fonts where id=" + QString::number(id);
        if (!style_res.exec(style_query))
        {
            emit done(m_fontFamilyList, m_fontIndex);
            return;
        }

        m_res = style_res.first();
        if (!m_res)
        {
            emit done(m_fontFamilyList, m_fontIndex);
            return;
        }

        family += (" (" + style_res.value(0).toString() + ")");
        m_fontIndex.append(id);
        m_fontFamilyList.append(family);

        if (!m_query.next())
        {
            break;
        }
    }

    emit done(m_fontFamilyList, m_fontIndex);
}

void SQLHandler::exec_string()
{
    if (sqlite3_prepare_v2(m_sql.db,
                           m_queryString.c_str(), m_queryString.length(),
                           &m_sql.stmt, NULL))
    {
        m_lastError = QString("Fail to build prepared statment:") +
                      QString(sqlite3_errmsg(m_sql.db));
        m_res = false;
        return;
    }

    if (sqlite3_step(m_sql.stmt) != SQLITE_DONE)
    {
        m_res = false;
    }

    static_cast<void>(sqlite3_finalize(m_sql.stmt));
    m_sql.stmt = nullptr;
}

//private member function
void SQLHandler::db_init()
{
#ifdef Q_OS_WIN32
    char *tmp_char = getenv("TEMP");
    if (!tmp_char)
    {
        m_lastError = "Fail to get temp directory";
        m_res = false;
        return;
    }

    std::string tmp_path(tmp_char);
    std::string::size_type str_index = 0;
    while ((str_index = tmp_path.find("\\", str_index)) != string::npos)
    {
        tmp_path.replace(str_index, 1, "/");
        ++str_index;
    }
#else
    std::string tmp_path= "/tmp";
#endif

    std::string path = tmp_path + "/fontColor.db";

    QString db_path = QString::fromStdString(path);
    if (QFile::exists(db_path))
    {
        QFile file(db_path);
        if (!file.remove())
        {
            m_lastError = "Fail to remove database";
            m_res = false;
            return;
        }
    }

    if (sqlite3_open(path.c_str(), &m_sql.db))
    {
        m_lastError = QString("Fail to open SQLite: ") +
                      QString(sqlite3_errmsg(m_sql.db));
        m_sql.db = nullptr;
        m_res = false;
        return;
    }

    m_queryString = "drop table if exists fonts";
    exec_string();
    if (!m_res)
    {
        m_lastError = QString("Fail to drop table");
        return;
    }

    m_queryString = "create table if not exists fonts (";
    m_queryString += "id INTEGER,";
    m_queryString += "font_file TEXT,";
    m_queryString += "font_family TEXT,";
    m_queryString += "font_style TEXT)";
    exec_string();

    if (!m_res)
    {
        return;
    }
}
