#include "gatewaymessage.h"

GatewayMessage::GatewayMessage(QString uuid, QString raw) :
  m_uuid(uuid), m_raw(raw), m_phoneid(0), m_enc_msg(false)
{
  m_e.setUnitId(m_uuid.toStdString().c_str(), m_uuid.length());
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

QString GatewayMessage::registerMessage()
{
  QString str("0,R,U," + m_uuid + "\r\n");
  return str;
}

/**
 * @brief GatewayMessage::getDecryption refer to plain c on encrypt.h
 * @return
 */
QString GatewayMessage::getDecryption()
{
  QString data(m_raw);

  if (data.at(data.length() - 1) != '\n') {
    data += '\n';
  }
  if (!m_enc_msg) {
    return data;
  }

  m_e.Decrypt((unsigned char *) data.toStdString().c_str());

  return QString::fromLocal8Bit((const char *) &m_e.encr_decr_message);
}

/**
 * @brief GatewayMessage::encrypt refer to plain c on encrypt.h
 */
QString GatewayMessage::encrypt(QString msg, int phoneid)
{
  char ch1 = msg.at(0).toLatin1();
  char ch2 = msg.at(2).toLatin1();
  char ch_end = msg.at(msg.length() - 1).toLatin1();

  if (ch_end != '\n') {
    msg += "\r\n";
  }
  if ((ch1 >= '0') && (ch1 <= '9')) {
    phoneid = (int) ch1 - '0';
  } else if ((ch2 == 'V')
             || (ch2 == 'U')
             || (ch2 == 'N')) {
    return "0," + msg;
  }
  if (phoneid == 0)
    return msg;
  else if (phoneid > 0)
    m_phoneid = phoneid;
  m_e.Encrypt((unsigned char *) msg.toStdString().c_str(), '0' + phoneid);

  return QString::fromLocal8Bit((const char *) &m_e.encr_decr_message);
}

void GatewayMessage::setUUID(QString uuid)
{
  m_uuid = uuid;
}
