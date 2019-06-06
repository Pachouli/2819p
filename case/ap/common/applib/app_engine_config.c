/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：引擎应用管理子模块，统一负责引擎类型和APP ID的转换。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

typedef struct
{
    uint8 engine_app_id;
    uint8 engine_type;
} app2engine_type_t;

const app2engine_type_t applib_app2eg_type[] =
{
    { APP_ID_MENGINE, ENGINE_MUSIC },
    { APP_ID_FMENGINE, ENGINE_RADIO },
    { APP_ID_LINEIN_EG, ENGINE_LINEIN },
    { APP_ID_BTPLAY_EG, ENGINE_BTPLAY },
    { APP_ID_BTCALL_EG, ENGINE_BTCALL },
    { APP_ID_USOUND_EG, ENGINE_USOUND },
};

//判断是否引擎
engine_type_e get_engine_type_by_appid_inner(uint8 app_id)
{
    uint8 i;

    for (i = 0; i < (sizeof(applib_app2eg_type) / sizeof(app2engine_type_t)); i++)
    {
        if (applib_app2eg_type[i].engine_app_id == app_id)
        {
            return applib_app2eg_type[i].engine_type;
        }
    }

    return ENGINE_NULL;
}

//获取引擎对应 app id
uint8 get_engine_appid_by_type_inner(engine_type_e engine_type)
{
    uint8 i;

    if (engine_type == ENGINE_NULL)
    {
        return APP_ID_MAX;
    }

    for (i = 0; i < (sizeof(applib_app2eg_type) / sizeof(app2engine_type_t)); i++)
    {
        if (applib_app2eg_type[i].engine_type == engine_type)
        {
            return applib_app2eg_type[i].engine_app_id;
        }
    }

    return APP_ID_MAX;
}

