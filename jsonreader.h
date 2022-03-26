#ifndef JSONREADER_H
#define JSONREADER_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QVariant>
#include <QJsonObject>

class JsonReader : public QObject
{
    Q_OBJECT
public:
    explicit JsonReader(QObject *parent = nullptr);
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_INVOKABLE bool writeJsonFile(const QJsonObject & object);
    Q_INVOKABLE QJsonObject readJsonFile();
    QString source() {return m_source;}
    void setSource(const QString & path) {
        if (m_source != path) {
            m_source = path;
            emit sourceChanged(path);
        }
    }
    static void clearJsonObject(QJsonObject & object);
signals:
    void sourceChanged(const QString & path);
private:
    QString m_source;
};

#endif // JSONREADER_H
