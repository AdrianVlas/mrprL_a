//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2572
//�������� ������� � ����� ������
#define END_ADR_REGISTER 2587

#define REGISTERS_XOR 2

#include "header.h"

int privateXORBigGetReg2(int adrReg);

int getXORBigModbusRegister(int);//�������� ���������� ��������
int getXORBigModbusBit(int);//�������� ���������� ����
int setXORBigModbusRegister(int, int);//�������� ���������� ��������
int setXORBigModbusBit(int, int);//�������� ���������� ����

int  postXORBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *xorbigcomponent;

/**************************************/
//���������� ���������� XOR
/**************************************/
void constructorXORBigComponent(COMPONENT_OBJ *xorbigcomp)
{
  xorbigcomponent = xorbigcomp;

  xorbigcomponent->getModbusRegister = getXORBigModbusRegister;//�������� ���������� ��������
  xorbigcomponent->getModbusBit      = getXORBigModbusBit;//�������� ���������� ����
  xorbigcomponent->setModbusRegister = setXORBigModbusRegister;//�������� ���������� ��������
  xorbigcomponent->setModbusBit      = setXORBigModbusBit;//�������� ���������� ����

  xorbigcomponent->postWriteAction = postXORBigWriteAction;//action ����� ������
}//prepareDVinConfig

int getXORBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_d_xor[0], REGISTERS_XOR, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getXORBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(xorbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(xorbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  //�������� �� ������
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //�������� �� ������������
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postXORBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = xorbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = xorbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_xor[0], REGISTERS_XOR, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateXORBigGetReg2(int adrReg)
{
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateYustBigGetReg2(int adrReg)

