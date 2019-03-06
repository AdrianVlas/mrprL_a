#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 13000
//�������� ������� � ����� ������
#define END_ADR_REGISTER 13075

#define REGISTERS_REG 32

int privateREGBigGetReg2(int adrReg);

int getREGBigModbusRegister(int);//�������� ���������� ��������
int getREGBigModbusBit(int);//�������� ���������� ����
int setREGBigModbusRegister(int, int);//�������� ���������� ��������
int setREGBigModbusBit(int, int);//�������� ���������� ����

void setREGBigCountObject(void);//�������� �-�� �������
void preREGBigReadAction(void);//action �� ������
void preREGBigWriteAction(void);//action �� ������
int  postREGBigWriteAction(void);//action ����� ������
int passwordImunitetRegREGBigComponent(int adrReg);

COMPONENT_OBJ *regbigcomponent;

/**************************************/
//���������� ���������� REGistrator
/**************************************/
void constructorREGBigComponent(COMPONENT_OBJ *regbigcomp)
{
  regbigcomponent = regbigcomp;

  regbigcomponent->getModbusRegister = getREGBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->getModbusBit      = getREGBigModbusBit;//�������� ���������� ����
  regbigcomponent->setModbusRegister = setREGBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->setModbusBit      = setREGBigModbusBit;//�������� ���������� ����

  regbigcomponent->preReadAction   = preREGBigReadAction;//action �� ������
  regbigcomponent->preWriteAction  = preREGBigWriteAction;//action �� ������
  regbigcomponent->postWriteAction = postREGBigWriteAction;//action ����� ������

  regbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getREGBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//������������ ���������� ������� ����������� ������������
  if(offset<32) return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_analog_registrator[0],
                                                   REGISTERS_REG, offset );
//������������ ���������� ������� ����������� ������������
  if(offset>=36&&offset<70)
      return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_digital_registrator[0],
                                                               REGISTERS_REG, offset-36 );

  switch(offset)
    {
    case 32://����� ������ ����������� ������������ (����������� ������)
      return (current_settings_interfaces.prefault_number_periods) &0xFFFF; //� ������� ��������� �������� �� ���������, � ������� ������
    case 33://����� ������ ����������� ������������ (����������� ������)
      return (current_settings_interfaces.postfault_number_periods) &0xFFFF; //� ������� ��������� �������� �� ���������, � ������� ������
    case 34://���������� ���������� �������������
      return (info_rejestrator_ar.number_records) &0xFFFF;
#define IMUNITET_REG35 35
    case IMUNITET_REG35://������� ���������� �����������
      if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
        return (number_record_of_ar_for_USB) &0xFFFF;
      else
        return (number_record_of_ar_for_RS485) &0xFFFF;
    case 70://���������� ���������� �������������
      return (info_rejestrator_dr.number_records) &0xFFFF;
#define IMUNITET_REG71 71
    case IMUNITET_REG71://������� ���������� �����������
      if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
        return (number_record_of_dr_for_USB) &0xFFFF;
      else
        return (number_record_of_dr_for_RS485) &0xFFFF;
    case 74://�������� ���������� �����������
      return MARKER_ERRORPERIMETR;
    case 75://�������� ���������� �����������
      return MARKER_ERRORPERIMETR;
    }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getREGBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(regbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(regbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  int offset = adrReg-BEGIN_ADR_REGISTER;
//������������ ���������� ������� ����������� ������������
  if(offset<32 || (offset>=36&&offset<70)) return validN_BIGACMD(dataReg);

  switch(offset)
    {
    case 32://����� ������ ����������� ������������ (����������� ������)
      if(dataReg<TIMEOUT_PREFAULT_MIN/20 || dataReg>TIMEOUT_PREFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 33://����� ������ ����������� ������������ (����������� ������)
      if(dataReg<TIMEOUT_POSTFAULT_MIN/20 || dataReg>TIMEOUT_POSTFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 34://���������� ���������� �������������
      return MARKER_ERRORDIAPAZON;
    case 35://������� ���������� �����������
      if (
        ((clean_rejestrators & CLEAN_AR) != 0) ||
        (
          ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
          ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
        )
      ) return MARKER_ERRORDIAPAZON;
      if(! ((unsigned int)dataReg < info_rejestrator_ar.number_records) &&
          ((unsigned int)dataReg < max_number_records_ar             )) return MARKER_ERRORDIAPAZON;

      break;
    case 70://���������� ���������� �������������
      return MARKER_ERRORDIAPAZON;
    case 71://������� ���������� �����������
      if (
        ((clean_rejestrators & CLEAN_DR) != 0) ||
        (
          ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
          ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
        )
      ) return MARKER_ERRORDIAPAZON;
      if(! ((unsigned int)dataReg < info_rejestrator_dr.number_records) &&
          (dataReg < MAX_NUMBER_RECORDS_INTO_DR             )) return MARKER_ERRORDIAPAZON;
      break;
    case 74://�������� ���������� �����������
      if(dataReg!=CMD_WORD_CLEAR_AR) return MARKER_ERRORDIAPAZON;
      break;
    case 75://�������� ���������� �����������
      if(dataReg!=CMD_WORD_CLEAR_DR) return MARKER_ERRORDIAPAZON;
      break;
    }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preREGBigReadAction(void)
{
//action �� ������
  regbigcomponent->isActiveActualData = 1;
}//
void preREGBigWriteAction(void)
{
//action �� ������
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//����������� ������
  regbigcomponent->isActiveActualData = 1;
}//
int postREGBigWriteAction(void)
{
  extern int upravlSchematic;//���� Rang
  extern int upravlSetting;//���� Setting
//action ����� ������
  int beginAdr = regbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = regbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  int flag1=1;
  int flag2=1;
  int countReg=countAdr;
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      if(offset<32&&flag1)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_analog_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER);
          flag1=0;
          upravlSchematic = 1;//���� Rang
        }//if

      if(offset>=36&&offset<70&&flag2)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_digital_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER+36);
          flag2=0;
          upravlSchematic = 1;//���� Rang
        }//if
      switch(offset)
        {
        case 32://����� ������ ����������� ������������ (����������� ������)
          edition_settings.prefault_number_periods = tempWriteArray[offsetTempWriteArray+i];// /20; //� ������� ��������� �������� �� ���������, � ������� ������
          upravlSetting = 1;//���� Setting
          break;
        case 33://����� ������ ����������� ������������ (����������� ������)
          edition_settings.postfault_number_periods = tempWriteArray[offsetTempWriteArray+i];// /20; //� ������� ��������� �������� �� ���������, � ������� ������
          upravlSetting = 1;//���� Setting
          break;
        case 35://������� ���������� �����������
        {
          unsigned int *point_to_number_record_of_ar;
          int *point_to_first_number_time_sample, *point_to_last_number_time_sample;

          if (pointInterface == USB_RECUEST)
            {
              point_to_number_record_of_ar = &number_record_of_ar_for_USB;
              point_to_first_number_time_sample = &first_number_time_sample_for_USB;
              point_to_last_number_time_sample = &last_number_time_sample_for_USB;
            }
          else
            {
              point_to_number_record_of_ar = &number_record_of_ar_for_RS485;
              point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
              point_to_last_number_time_sample = &last_number_time_sample_for_RS485;
            }

          //������������ ����� ������ ����������� ���������� ��� �������
          *point_to_number_record_of_ar = tempWriteArray[offsetTempWriteArray+i];
          //������ ������� ������� ����������� ���������� ���  ����������

          //����������� ������� ��������� ������ ������ ������ � ������, ������ �������, ������� ����
          *point_to_first_number_time_sample = -1;
          int last_number_time_sample_tmp = (SIZE_PAGE_DATAFLASH_2 - sizeof(__HEADER_AR))/((NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int));
          int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;
          if (last_number_time_sample_tmp <= max_number_time_sample)
            {
              *point_to_last_number_time_sample = last_number_time_sample_tmp - 1;//����� ���������� �������� ���� �������
            }
          else
            {
              *point_to_last_number_time_sample = max_number_time_sample - 1;
            }

          //������ ������� ������� ���� � ����� ���'��
          if (pointInterface == USB_RECUEST)
            control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB;
          else
            control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485;

        }
        break;//case 35
        case 71://������� ���������� �����������

          if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
            {
//int test = tempWriteArray[offsetTempWriteArray+i];
              number_record_of_dr_for_USB = tempWriteArray[offsetTempWriteArray+i];
              //������ ������� ������� ����������� ���������� ��� ���������� USB

              //����������� ����� ������� ������
              part_reading_dr_from_dataflash_for_USB = 0;
              //������ ������� ������� ���� � ����� ���'��
              control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB;
            }//if
          else
            {
              number_record_of_dr_for_RS485 = tempWriteArray[offsetTempWriteArray+i];
              //������ ������� ������� ����������� ���������� ��� ���������� RS-485

              //����������� ����� ������� ������
              part_reading_dr_from_dataflash_for_RS485 = 0;
              //������ ������� ������� ���� � ����� ���'��
              control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485;
            }

          break;
        case 74://�������� ���������� �����������
//������ ��������
          if (
            (current_ekran.current_level == EKRAN_DATA_LADEL_AR)
            ||
            (state_ar_record             != STATE_AR_NO_RECORD )
            ||
            (
              (control_tasks_dataflash & (
                 TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR |
                 TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR      |
                 TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB                         |
                 TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485                       |
                 TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU
               )
              ) != 0
            )
            ||
            ((clean_rejestrators & CLEAN_AR) != 0)
          ) return ERROR_VALID2;//������ ���������

          clean_rejestrators |= CLEAN_AR;
          break;
        case 75://�������� ���������� �����������
//������ ��������
          if (
            (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
            ||
            (current_ekran.current_level == EKRAN_DATA_LADEL_DR             )
            ||
            (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR        )
            ||
            (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES          )
            ||
            (current_ekran.current_level == EKRAN_MAX_VALUES                )
            ||
            (
              (control_tasks_dataflash & (
                 TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                    |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                  |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU
               )
              ) != 0
            )
            ||
            ((clean_rejestrators & CLEAN_DR) != 0)
          ) return ERROR_VALID2;//������ ���������

          //�������, �� ����� �������� ����������� ����������
          clean_rejestrators |= CLEAN_DR;
          break;
        }//switch
    }//for

  return 0;
}//

int privateREGBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int passwordImunitetRegREGBigComponent(int adrReg)
{
  //����������� � ������ ������ ��������� 0 - ���� ��������
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_REG35://������� ���������� �����������
  case BEGIN_ADR_REGISTER+IMUNITET_REG71://������� ���������� �����������
    return 0;//���� ��������
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegREGBigComponent(int adrReg)
