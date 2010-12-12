#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QObject>
#include <QMap>

class ConfigLoader: public QObject {
    Q_OBJECT
    public:
	  explicit ConfigLoader(QObject *parent=NULL);

	  bool loadConfiguration(const QString &file);
	  bool saveConfiguration(const QString &file);

	  void clear();

	  void setValueForKey(const QString &key, const QString &value);
	  QString valueForKey(const QString &key);

    private:
	  QMap<QString, QString> m_Keys;
};

#endif // CONFIGLOADER_H
