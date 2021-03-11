
//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 350
//�������� ������� � ����� ������
#define END_ADR_REGISTER 444

#include "header.h"

int getAISmallModbusRegister(int);//�������� ���������� ��������
int getAISmallModbusBit(int);//�������� ���������� ����
int setAISmallModbusRegister(int, int);//�������� ���������� ��������
int setAISmallModbusBit(int, int);//�������� ���������� ����

int  postAISmallWriteAction(void);//action ����� ������
void loadAISmallActualData(void);

int privateAISmallGetReg2(int adrReg);

int getAISmallModbusRegister_guest(int offset);

COMPONENT_OBJ *aismallcomponent;

/**************************************/
//��������� ���������
/**************************************/
void constructorAISmallComponent(COMPONENT_OBJ *aismallcomp)
{
  aismallcomponent = aismallcomp;

  aismallcomponent->getModbusRegister = getAISmallModbusRegister;//�������� ���������� ��������
  aismallcomponent->getModbusBit      = getAISmallModbusBit;//�������� ���������� ����
  aismallcomponent->setModbusRegister = setAISmallModbusRegister;//�������� ���������� ��������
  aismallcomponent->setModbusBit      = setAISmallModbusBit;//�������� ���������� ����

  aismallcomponent->postWriteAction = postAISmallWriteAction;//action ����� ������
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getAISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getAISmallModbusRegister_guest(adrReg-BEGIN_ADR_REGISTER);
}//getDVModbusRegister(int adrReg)
int getAISmallModbusBit(int x) {
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postAISmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

