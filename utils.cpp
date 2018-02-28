#include "utils.h"

utils::utils()
{

}


QString utils::formatfloat(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
QString utils::float2string(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf/10);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf/100);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf/1000);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf/10000);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
