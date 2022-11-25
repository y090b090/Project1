#ifndef __FND_DRV_H__
#define __FND_DRV_H__
#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM + 2)


typedef struct FNDWriteDataForm_tag
{
char DataNumeric[FND_DATA_BUFF_LEN];//숫자0-9
char DataDot[FND_DATA_BUFF_LEN];//숫자0or1
char DataValid[FND_DATA_BUFF_LEN];//숫자0or1
}stFndWriteForm,*pStFndWriteForm;

int fndDisp(int num, int dotflag);

#endif//__FND_DRV_H__
