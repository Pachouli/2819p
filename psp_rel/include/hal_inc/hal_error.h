
#ifndef __HAL_ERROR_H_
#define __HAL_ERROR_H_

/*int32 ���ͣ��뽫����������ֵ������ uint16 ��Χ��*/
/*HALģ�鷵��ʱ������HAL_OK�⣬��Ҫ����-�ţ����� -HAL_INVALID_PARAMETER����������ֵ�Ǹ�������ʾ�ӿڵ��ó����ˣ��� */

//ͨ�ô�������
#define HAL_OK                     0 //��������
#define HAL_INVALID_PARAMETER      1 //��Ч��������
#define HAL_ILLEGAL_OPERATION      2 //�Ƿ���������
#define HAL_UNKNOWN_ERROR          3 //����δ֪����

//AUDIOģ���������
#define HAL_AUDIO_ERROR            4 //����AUDIO����

#endif
