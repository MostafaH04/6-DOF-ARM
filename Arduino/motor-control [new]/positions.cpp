#include "positions.h"

Positions::Positions(){
    for(int i = 0; i < 6; i++){
        positions[i] = 0;
    }
}

void Positions::setPositions(long pos1, long pos2, long pos3, long pos4, long pos5, long pos6){
    positions[0] = pos1;
    positions[1] = pos2;
    positions[2] = pos3;
    positions[3] = pos4;
    positions[4] = pos5;
    positions[5] = pos6;
}

void Positions::setPositionsAngles(long angle1, long angle2, long angle3, long angle4, long angle5, long angle6){
    positions[0] = angle1/1.8;
    positions[1] = angle2/1.8;
    positions[2] = angle3/1.8;
    positions[3] = angle4/1.8;
    positions[4] = angle5/1.8;
    positions[5] = angle6/1.8;
}

long *Positions::getPositions(){
    return positions;
}