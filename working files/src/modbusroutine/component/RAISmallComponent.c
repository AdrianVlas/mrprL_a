
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 445
//�������� ������� � ����� ������
#define END_ADR_REGISTER 800

#define MMEASUREMENTS_DR_WIDTH                35
#define MM_ADDRESS_FIRST_MEASUREMENTS_DR       BEGIN_ADR_REGISTER


#define MDR_OFFSET_VMP                         32

int getRAISmallModbusRegister(int);//�������� ���������� ��������
int getRAISmallModbusBit(int);//�������� ���������� ����
int setRAISmallModbusRegister(int, int);//�������� ���������� ��������
int setRAISmallModbusBit(int, int);//�������� ���������� ����

void preRAISmallReadAction(void);//action �� ������
void preRAISmallWriteAction(void);//action �� ������
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

  raismallcomponent->preReadAction   = preRAISmallReadAction;//action �� ������
  raismallcomponent->preWriteAction  = preRAISmallWriteAction;//action �� ������
  raismallcomponent->postWriteAction = postRAISmallWriteAction;//action ����� ������

  raismallcomponent->isActiveActualData = 0;
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
  ) return MARKER_ERRORPERIMETR;
  if (
    //����� ��� ���������� ��� ���������� ������ ����������� ����������, ��� ������� ����, ���� �� �������� � ��������� �����������
    ((clean_rejestrators & CLEAN_DR) != 0) ||
    (
      ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
      ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
    )
  ) return MARKER_ERRORPERIMETR; //�� ������ ����������������� ����������� ���������� ������ ����������� ����������

  //����� ������ ���
  int number_block, offset;
  unsigned char *point_to_buffer;
  number_block = (adrReg - MM_ADDRESS_FIRST_MEASUREMENTS_DR) / MMEASUREMENTS_DR_WIDTH;
  offset = (adrReg - MM_ADDRESS_FIRST_MEASUREMENTS_DR) - number_block*MMEASUREMENTS_DR_WIDTH;
  if (pointInterface == USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_dr;
  else point_to_buffer = buffer_for_RS485_read_record_dr;

  if (!(
        number_block < (
/*
#define FIRST_INDEX_START_START_RECORD_DR     0
#define FIRST_INDEX_DATA_TIME_DR              1
#define FIRST_INDEX_NAME_OF_CELL_DR           8
#define FIRST_INDEX_NUMBER_ITEMS_DR           40
#define FIRST_INDEX_NUMBER_CHANGES_DR         41
#define FIRST_INDEX_VMP_VALUE                 43
#define FIRST_INDEX_VMP_EQUAL_MORE            47
#define FIRST_INDEX_FIRST_DATA_DR             48
*/
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_PHASE_DR  )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MIN_Z_DR      )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3I0_DR    )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3U0_DR    )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MIN_U_DR      )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_U_DR      )) +
//          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_ZOP_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_VMP_VALUE      )) +
          (*(point_to_buffer + FIRST_INDEX_VMP_EQUAL_MORE ))
        )
      )) return 0;//���� ������
//  unsigned int index;

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

void preRAISmallReadAction(void) {
//action �� ������
  raismallcomponent->isActiveActualData = 1;
}//

void preRAISmallWriteAction(void) {
//action �� ������
  raismallcomponent->operativMarker[0] = -1;
  raismallcomponent->operativMarker[1] = -1;//����������� ������
  raismallcomponent->isActiveActualData = 1;
}//
int postRAISmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateRAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateRAISmallGetReg2(int adrReg)

