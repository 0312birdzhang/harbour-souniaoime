#include <QString>
#include "opencc.h"
#include "opencc_service.hpp"

OpenCCService::OpenCCService(QQuickItem *parent) : QQuickItem(parent) {
  const std::string defaultMode("s2twp.json");
  converter = new opencc::SimpleConverter(defaultMode);
}

OpenCCService::~OpenCCService() { delete converter; }

void OpenCCService::chooseMode(QString mode) {
  try {
    mode.append(".json");
    const std::string newMode("/usr/share/harbour-souniaoime/data/config/" + mode.toStdString());
    opencc::SimpleConverter* newConverter =
        new opencc::SimpleConverter(newMode);
    delete converter;
    converter = newConverter;
  } catch (std::runtime_error& e) {
    qCritical("Error while changing mode: %s", e.what());
  }
}

QString OpenCCService::convert(const QString qstr) const {
  // Use this if you ever wish to add Windows support
  // std::string str = qstr.toLocal8Bit().constData();
  std::string str = qstr.toUtf8().constData();
  std::string newStr = converter->Convert(str);
  return QString(newStr.c_str());
}
