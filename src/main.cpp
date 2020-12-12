//-------------------------------------------------
// INCLUDES
//-------------------------------------------------
#include "daisysp.h"
#include "daisy_seed.h"
#include "DaisyDrone.h"

using namespace daisysp;
using namespace daisy;

//-------------------------------------------------
// DEFINITIONS
//-------------------------------------------------

//-------------------------------------------------
// VARIABLES
//-------------------------------------------------
DaisyDrone * drone = nullptr;

//-------------------------------------------------
// FUNCTIONS
//-------------------------------------------------

int main(void)
{
    drone = DaisyDrone::GetInstance();
    drone->Init();
    while (1)
    {
        //drone->Update();
    }
}
