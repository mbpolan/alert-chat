#include <QDebug>
#include <QDesktopServices>
#include <QFile>

#include "configloader.h"

ConfigLoader::ConfigLoader(QObject *parent): QObject(parent) {
}

bool ConfigLoader::loadConfiguration(const QString &file) {
    QFile f(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/"+file);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	  return false;

    while(!f.atEnd()) {
	  QString line=QString(f.readLine().data());

	  // skip blank lines
	  if (line[0]=='\n')
		continue;

	  // trim the newline character at the end of the string
	  line=line.left(line.size()-1);

	  // extract the key
	  int pos=line.indexOf("=");
	  if (pos==-1)
		continue;

	  QString key=line.left(pos-1);
	  QString value=line.right(line.size()-pos-2);

	  m_Keys[key]=value;
    }

    f.close();
    return true;
}

bool ConfigLoader::saveConfiguration(const QString &file) {
    QFile f(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/"+file);
    if (!f.open(QIODevice::WriteOnly))
	  return false;

    for (QMap<QString,QString>::iterator it=m_Keys.begin(); it!=m_Keys.end(); ++it) {
	  QByteArray line="";
	  line+=it.key();
	  line+=" = ";
	  line+=it.value();
	  line+="\n";

	  f.write(line);
    }

    f.close();
    return true;
}

void ConfigLoader::clear() {
    m_Keys.clear();
}

void ConfigLoader::setValueForKey(const QString &key, const QString &value) {
    m_Keys[key]=value;
}

QString ConfigLoader::valueForKey(const QString &key) {
    if (m_Keys.contains(key))
	  return m_Keys[key];
    else
	  return "";
}
