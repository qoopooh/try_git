#ifndef GATEWAYMESSAGE_H
#define GATEWAYMESSAGE_H

#include <QObject>
#include "encrypt.h"

class GatewayMessage
{
public:
  explicit GatewayMessage(QString uuid="1111111111222222222233333333334444444444", QString raw="");
  QString getRaw();
  int getPhoneId();
  QString getUUID();
  QString registerMessage();

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
  QString m_uuid;
  QString m_raw;
  int m_phoneid;
  bool m_enc_msg;
  Encryption m_e;
};

#endif // GATEWAYMESSAGE_H
