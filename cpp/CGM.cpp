#include "headers/CGM.h"
#include <cstdlib>
#include <ctime>

int CGM :: getCurrentGlucose(){
    std :: srand(std :: time(nullptr));
    CGM :: currentGlucose =  std :: rand() % 10 + 1;
    return 1.0;
}

int CGM :: getCurrentCarbs(){
    std :: srand(std :: time(nullptr));
    CGM :: currentCarbs = std :: rand() % 50 + 1;
    return 1.0;
}

void CGM :: updateGlucoseLevel(double glucose){

}
