#ifndef OPENCC_SERVICE_HPP
#define OPENCC_SERVICE_HPP

#include <QQuickItem>
#include <QString>
#include "opencc.h"

class OpenCCService : public QQuickItem
{
  Q_OBJECT
  Q_DISABLE_COPY(OpenCCService)
private:
  opencc::SimpleConverter* converter;
public:
  OpenCCService(QQuickItem *parent = nullptr);
  ~OpenCCService();
  Q_INVOKABLE void chooseMode(QString mode);
  Q_INVOKABLE QString convert(const QString qstr) const;

};

#endif // OPENCC_SERVICE_HPP
