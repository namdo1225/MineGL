#include "include.h"
#include "controller.h"

int main() {

    Controller controller = Controller("MineGL", 1200, 800);
    controller.loop();
    controller.end();

}