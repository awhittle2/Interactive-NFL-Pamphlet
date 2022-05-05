#include <nfldatatable.h>
#include <string>

//using each teams capacity from within the vector.
//taking that data and totaling it while;
//-making sure no repeats
//-making sure all stadiums present

/*
std::string stadiumCapacities[5][2] = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "b", "2" }, { "e", "5" } };
int totalSumCapacity = std::stoi(stadiumCapacities[0][1]);

for (int i = 1; i < sizeof(stadiumCapacities) / sizeof(stadiumCapacities[0]); i++) {
    for (int j = 0; j < i; j++) {
        if (stadiumCapacities[i][0] == stadiumCapacities[j][0]) {
            break;
        }
        if (j == i - 1) {
            totalSumCapacity += std::stoi(stadiumCapacities[i][1]);
        }
    }
}
*/
