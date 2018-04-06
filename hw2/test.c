//
// Created by ds-ubuntu on 18. 4. 6.
//

typedef unsigned char tinyfp;

// returns 1 if tf is +inf
// returns -1 is td is -inf
// returns 0 if tf is not +-inf
// returns 2 is tf is nan

int checkSpecial(tinyfp tf) {

    if ((tf | 0b01111000) != tf)
        return 0;
    if ((tf << 5) == 0) {
        if (tf >> 7 == 0)
            return 1;
        return -1;
    }
    return 2;
}



int main (){

}