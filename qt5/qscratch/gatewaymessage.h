#ifndef GATEWAYMESSAGE_H
#define GATEWAYMESSAGE_H

#include <QObject>
#include "encrypt.h"

class GatewayMessage : public QObject
{
  Q_OBJECT
public:
  explicit GatewayMessage(QObject *parent = 0, QString raw="",
                          QString uuid="11111111112222222222333333333344444444441111111111222222222233333333334444444444");
  QString getRaw();
  int getPhoneId();
  QString getUUID();

  /**
   * @brief getDecryption convert raw message to be decrypted message
   * @return decrypted message
   */
  QString getDecryption();

  /**
   * @brief encrypt convert input message to be encrypted message
   * @param msg - input message
   * @param phoneid - set this value if it has change
   * @return encrypted message
   */
  QString encrypt(QString msg, int phoneid = -1);

  void setUUID(QString);

signals:

public slots:

private:
  QString m_raw;
  QString m_uuid;
  int m_phoneid;
  bool m_enc_msg;
};

#endif // GATEWAYMESSAGE_H
