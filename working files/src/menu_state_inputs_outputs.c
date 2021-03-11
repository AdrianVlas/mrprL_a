#include "header.h"

/*****************************************************/
//������� ����� ����������� ��������� ������ �����-������
/*****************************************************/
void make_ekran_list_inputs_outputs(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_INPUTS_OUTPUTS][MAX_COL_LCD] = 
  {
    {
      " ����.������    ",
      " ����.�������   "
    },
    {
      " ���� �����    ",
      " ���� ������   "
    },
    {
      " Inputs  state  ",
      " Outputs state  "
    },
    {
      " ʳ�� ��f����  ",
      " ��f�� ��f����  "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //�������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
    if (index_of_ekran < MAX_ROW_FOR_LIST_INPUTS_OUTPUTS)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� �������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*******************************************************/
//������� ����� ������ ����� ��� ������
/*
--------------------------------------------------------
input_output
    0 - �����
    1 - ������
--------------------------------------------------------
*/
/*******************************************************/
void make_ekran_state_inputs_or_outputs(unsigned int input_output)
{
#define MAX_COL_LCD_PART1 10

  static const unsigned char title_input_output[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD_PART1] = 
  {
    {
      " ���.     ",
      " ����.    "
    },
    {
      " ���.     ",
      " ����.    "
    },
    {
      " DI.      ",
      " DO.      "
    },
    {
      " ��.      ",
      " ���f.    "
    }
  };
  
  static const unsigned int index_of_number_di_do[MAX_NAMBER_LANGUAGE][2] = 
  {
    {5, 6},
    {5, 6},
    {4, 4},
    {4, 6}
  };
    
  static const unsigned char information_active[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    "���.  ",
    "���.  ",
    "Act.  ",
    "���.  "
  };
  static const unsigned char information_pasive[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    "��c�. ",
    "���.  ",
    "Pass. ",
    "����. "
  };
  static const unsigned char information_close[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1]  = 
  {
    "����. ",
    "�����.",
    "Closed",
    "����. "
  };
  static const unsigned char information_open[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1]   = 
  {
    "������",
    "�����",
    "Opened",
    "������"
  };
  
  int index_language = index_language_in_array(current_settings.language);
  unsigned int index_of_number_di_do_tmp = index_of_number_di_do[index_language][input_output];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int max_row, state;
  
  //��������� ������ ����� �� ���� � ����
  if (input_output == 0)
  {
    max_row = NUMBER_INPUTS;
    state = active_inputs;
  }
  else
  {
    max_row = NUMBER_OUTPUTS;
    state = state_outputs_raw;
  }
  
  /*******************************************************/
  //������� ����� � ��������. �������  ����� � ������� �����
  /*******************************************************/
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int number = index_of_ekran + 1;
    int tmp_1 = -1, tmp_2 = -1;

    if (input_output == ID_INPUT)
    {
      for (size_t j = 0; j < N_INPUT_BOARDS; j++)
      {
        if (number <= input_boards[j][0])
        {
          tmp_1 = input_boards[j][1];
          tmp_2 = (j == 0) ? number : number - input_boards[j - 1][0];
          
          break;
        }
      }
    }
    else
    {
      for (size_t j = 0; j < N_OUTPUT_BOARDS; j++)
      {
        if (number <= output_boards[j][0])
        {
          tmp_1 = output_boards[j][1];
          tmp_2 = (j == 0) ? number : number - output_boards[j - 1][0];
          
          break;
        }
      }
    }

    //�������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
    if (index_of_ekran < max_row)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++)
      {
        if (j < MAX_COL_LCD_PART1)
        {
          if ((j < index_of_number_di_do_tmp) || (j > (index_of_number_di_do_tmp + 2)))
            working_ekran[i][j] = title_input_output[index_language][input_output][j];
          else if (j == (index_of_number_di_do_tmp + 0))
          {
            if (tmp_1 < 0) working_ekran[i][j] = '?';
            else if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x40;
          }
          else if (j == (index_of_number_di_do_tmp + 1)) working_ekran[i][j] = '.';
          else     
          {
            if (tmp_2 < 0) working_ekran[i][j] = '?';
            else working_ekran[i][j] = tmp_2 + 0x30;
          }
        }
        else
        {
          if (input_output == 0)
          {
            if ((state & (1<<index_of_ekran)) == 0)
              working_ekran[i][j] = information_pasive[index_language][j - MAX_COL_LCD_PART1];
            else
              working_ekran[i][j] = information_active[index_language][j - MAX_COL_LCD_PART1];
          }
          else
          {
            if ((state & (1<<index_of_ekran)) == 0)
              working_ekran[i][j] = information_open[index_language][j - MAX_COL_LCD_PART1];
            else
              working_ekran[i][j] = information_close[index_language][j - MAX_COL_LCD_PART1];
          }
        }
      }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }  

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;

  //�������� �������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
  
#undef MAX_COL_LCD_PART1  
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
