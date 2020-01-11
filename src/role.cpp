#include "role.h"

Role::Role(int id, int job, int level, int HP, int HPMax, int exp, int expMax,
           int physicalAttack, int physicalDefence, int IQ, int accurcy,
           int speed, int dodge, int range, int weapon0, int weapon1,
           int weapon2, int weapon3, int medicine0, int medicine1,
           int medicine2, int medicine3, int shield, int article, int crit)
    : ID(id), job(static_cast<ROLEJOB>(job)), level(level), HP(HP),
      HPMax(HPMax), exp(exp), expMax(expMax), physicalAttack(physicalAttack),
      physicalDefence(physicalDefence), IQ(IQ), accurcy(accurcy), speed(speed),
      dodge(dodge), shield(shield), article(article), range(range), crit(crit) {

  weapon[0] = weapon0;
  weapon[1] = weapon1;
  weapon[2] = weapon2;
  weapon[3] = weapon3;
  medicine[0] = medicine0;
  medicine[1] = medicine1;
  medicine[2] = medicine2;
  medicine[3] = medicine3;
}

QString Role::getJob() {
  switch (job) {
  case MEIZHI:
    return QString("妹纸");
  case MENGMEIZHI:
    return QString("萌妹纸");
  case NVSHEN:
    return QString("女神");
  case JIROUNAN:
    return QString("肌肉男");
  case BAOLINAN:
    return QString("暴力男");
  case QIGONGSHI:
    return QString("气功师");
  case ZHAINAN:
    return QString("宅男");
  case JISHUZHAI:
    return QString("技术宅");
  case JISHUSHEN:
    return QString("技术神");
  case ZIXINAN:
    return QString("自习男");
  case XUEBA:
    return QString("学霸");
  case XUEZHA:
    return QString("学渣");
  case DASHU:
    return QString("大叔");
  case SHUSHU:
    return QString("蜀黍");
  case GUAISHUSHU:
    return QString("怪蜀黍");
  case DAMA:
    return QString("大妈");
  case CAIDAOMA:
    return QString("菜刀妈");
  case ZHANDOUMA:
    return QString("战斗妈");
  case DIAOSI:
    return QString("屌丝");
  case DIAOBAO:
    return QString("碉堡");
  case NIXI:
    return QString("逆袭");
  case TUHAO:
    return QString("富二代");
  case FUERDAI:
    return QString("土豪");
  case GAOFUSHUAI:
    return QString("高富帅");
  default:
    qDebug() << job;
    assert(0);
  }
}
