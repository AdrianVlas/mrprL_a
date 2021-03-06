
//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 445
//�������� ������� � ����� ������
#define END_ADR_REGISTER 477

#define MM_ADDRESS_FIRST_MEASUREMENTS_DR       BEGIN_ADR_REGISTER


#define MDR_OFFSET_VMP                         32

#include "header.h"

int getRAISmallModbusRegister(int);//�������� ���������� ��������
int getRAISmallModbusBit(int);//�������� ���������� ����
int setRAISmallModbusRegister(int, int);//�������� ���������� ��������
int setRAISmallModbusBit(int, int);//�������� ���������� ����

int  postRAISmallWriteAction(void);//action ����� ������

int privateRAISmallGetReg2(int adrReg);

COMPONENT_OBJ *raismallcomponent;

/**************************************/
//��������� ��������� � ������ ������
/**************************************/
void constructorRAISmallComponent(COMPONENT_OBJ *raismallcomp)
{
  raismallcomponent = raismallcomp;

  raismallcomponent->getModbusRegister = getRAISmallModbusRegister;//�������� ���������� ��������
  raismallcomponent->getModbusBit      = getRAISmallModbusBit;//�������� ���������� ����
  raismallcomponent->setModbusRegister = setRAISmallModbusRegister;//�������� ���������� ��������
  raismallcomponent->setModbusBit      = setRAISmallModbusBit;//�������� ���������� ����

  raismallcomponent->postWriteAction = postRAISmallWriteAction;//action ����� ������
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getRAISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  
  if (
    //�� ������ ����������������� ����������� ���������� ������ ����������� ����������
    //pointInterface=0 ����� ���������� 0-USB 1-RS485
    ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB == 0xffff)) ||
    ((pointInterface == RS485_RECUEST) && (number_record_of_dr_for_RS485 == 0xffff))
#if (MODYFIKACIA_VERSII_PZ >= 10)
      ||
    ((pointInterface == LAN_RECUEST) && (number_record_of_dr_for_LAN == 0xffff))
#endif
  ) return MARKER_ERRORPERIMETR;
  if (
    //����� ��� ���������� ��� ���������� ������ ����������� ����������, ��� ������� ����, ���� �� �������� � ��������� �����������
    ((clean_rejestrators & CLEAN_DR) != 0) ||
    (
      ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
      ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
      ||
      ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
    )
  ) return MARKER_ERRORPERIMETR; //�� ������ ����������������� ����������� ���������� ������ ����������� ����������

  //����� ������ ����
  int offset;
  unsigned char *point_to_buffer;
  offset = (adrReg - MM_ADDRESS_FIRST_MEASUREMENTS_DR);
  if (pointInterface == USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_dr;
  else if (pointInterface == RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface == LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
#endif
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(203);
  }

  switch (offset)
  {
  case MDR_OFFSET_VMP:
  {
    unsigned char *point_unsigned_char = (unsigned char *)(point_to_buffer + FIRST_INDEX_VMP_VALUE);
    unsigned int *point_unsigned_int = (unsigned int*)point_unsigned_char;
    int value_int = *point_unsigned_int;

    if (((unsigned int)value_int) != ((unsigned int)UNDEF_VMP))
    {
      if (abs(value_int) >= 10)
        return (value_int/10) &0xFFFF;
      else
      {
        if (value_int >= 0)
          return 1;
        else
          return (-1)&0x0000FFFF;
      }
    }
    return 0;
  }//case MDR_OFFSET_VMP:
  }//switch
  return 0;
}//getDVModbusRegister(int adrReg)
int getRAISmallModbusBit(int x) {
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setRAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setRAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postRAISmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateRAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateRAISmallGetReg2(int adrReg)

