#include "gatewaymessage.h"

GatewayMessage::GatewayMessage(QObject *parent, QString raw, QString uuid) :
  QObject(parent), m_raw(raw), m_uuid(uuid), m_phoneid(0), m_enc_msg(false)
{
  if ((m_raw == NULL) || (m_raw.length() < 7))
    return;

  char ch = m_raw[0].toLatin1();
  if ((ch > '0') && (ch <= '9'))
    m_enc_msg = true;
}

QString GatewayMessage::getRaw()
{
  return m_raw;
}
int GatewayMessage::getPhoneId()
{
  return m_phoneid;
}

QString GatewayMessage::getUUID()
{
  return m_uuid;
}

/**
 * @brief GatewayMessage::getDecryption refer to plain c on encrypt.h
 * @return
 */
QString GatewayMessage::getDecryption()
{
  if (!m_enc_msg) {
    return m_raw;
  }
  unit_id = m_uuid.toStdString().c_str();
  Decrypt((unsigned char *) m_raw.toStdString().c_str());

  return QString::fromLocal8Bit((const char *) &encr_decr_message);
}

/**
 * @brief GatewayMessage::encrypt refer to plain c on encrypt.h
 */
QString GatewayMessage::encrypt(QString msg, int phoneid)
{
  if (phoneid == 0)
    return msg;
  else if (phoneid > 0)
    m_phoneid = phoneid;
  unit_id = m_uuid.toStdString().c_str();
  Encrypt((unsigned char *) msg.toStdString().c_str(), m_phoneid);

  return QString::fromLocal8Bit((const char *) &encr_decr_message);
}

void GatewayMessage::setUUID(QString uuid)
{
  m_uuid = uuid;
}
