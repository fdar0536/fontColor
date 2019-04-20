#include <string>
#include "sqlhandler.hpp"

#include "QFile"
#include "QSqlError"
#include "QVariant"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using namespace std;

SQLHandler::SQLHandler() :
    m_db(QSqlDatabase()),
    m_query(QSqlQuery()),
    m_queryString(QString("")),
    m_lastError(QString("")),
    m_res(true)
{
    db_init();
    this->setAutoDelete(false);
}
SQLHandler::~SQLHandler()
{
    m_db.close();
}

void SQLHandler::setqueryString(QString &input)
{
    m_queryString = input;
}

void SQLHandler::setQuery(QSqlQuery &input)
{
    m_query = input;
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

QSqlQuery SQLHandler::getQuery() const
{
    return m_query;
}

QString SQLHandler::getlastError() const
{
    return m_lastError;
}

//run
void SQLHandler::run()
{
    m_query.finish();
    m_query = QSqlQuery(m_db);
    exec_string();
    if (!m_res)
    {
        m_lastError = m_query.lastError().text();
        emit done(m_fontFamilyList, m_fontIndex);
    }
    
    m_res = m_query.first();
    if (!m_res)
    {
        m_lastError = m_query.lastError().text();
        emit done(m_fontFamilyList, m_fontIndex);
    }
    
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
    m_res = m_query.exec(m_queryString);
    
    if (!m_res)
    {
        m_lastError = m_query.lastError().text();
    }
}

void SQLHandler::exec_prepared()
{
    m_res = m_query.exec();
    if (!m_res)
    {
        m_lastError = m_query.lastError().text();
    }
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
    
    string tmp_path(tmp_char);
    string::size_type str_index = 0;
    while ((str_index = tmp_path.find("\\", str_index)) != string::npos)
    {
        tmp_path.replace(str_index, 1, "/");
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
            m_lastError = "Fail to remove database";
            m_res = false;
            return;
        }
    }
    
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(db_path);
    
    if (!m_db.open())
    {
        m_lastError = "Fail to open database";
        m_res = false;
        return;
    }
    
    m_query = QSqlQuery(m_db);
    m_queryString = "drop table if exists fonts";
    exec_string();
    if (!m_res)
    {
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
