#ifndef GATEWAYMESSAGE_H
#define GATEWAYMESSAGE_H

#include <QObject>

class GatewayMessage : public QObject
{
  Q_OBJECT
public:
  explicit GatewayMessage(QObject *parent = 0);

signals:

public slots:

};

#endif // GATEWAYMESSAGE_H
