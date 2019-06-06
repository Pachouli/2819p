
#ifndef __HAL_ERROR_H_
#define __HAL_ERROR_H_

/*int32 类型，请将错误类型数值定义在 uint16 范围内*/
/*HAL模块返回时，除了HAL_OK外，需要加上-号，比如 -HAL_INVALID_PARAMETER，这样返回值是负数，表示接口调用出错了！！ */

//通用错误类型
#define HAL_OK                     0 //正常返回
#define HAL_INVALID_PARAMETER      1 //无效参数错误
#define HAL_ILLEGAL_OPERATION      2 //非法操作错误
#define HAL_UNKNOWN_ERROR          3 //其他未知错误

//AUDIO模块错误类型
#define HAL_AUDIO_ERROR            4 //其他AUDIO错误

#endif
