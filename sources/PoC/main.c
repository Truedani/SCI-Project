#include "CySecDrv.h"


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

void main() {
    CySecDrvInit();
    teCySecDrvStatus status = CySecDrvGetSecurityState();
    if (status == SECURED) {
        printf("Security State: SECURED");
    } else if (status == NOT_SECURED) {
        printf("Security State: NOT_SECURED");
    }
}