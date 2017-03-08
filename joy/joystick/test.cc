// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright Drew Noakes 2013-2016

#include "joystick.hh"
#include <unistd.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

int main(int argc, char** argv)
{
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");

  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("open failed.\n");
    exit(1);
  }

  int last_left=0, last_right=0, uncapped=0;
  
  while (true)
  {
    // Restrict rate
    usleep(1000);

    // Attempt to sample an event from the joystick
    JoystickEvent event;
    if (joystick.sample(&event))
    {
      if (event.isButton())
      {
        //printf("Button %u is %s\n",
        //  event.number,
        //  event.value == 0 ? "up" : "down");
        if(event.number==5)
			uncapped=event.value!=0?1:0;
      }
      else if (event.isAxis())
      {
        //printf("Axis %u is at position %d\n", event.number, event.value);
        int abb = (event.value>0?1:-1)*max(0,(event.value>0?event.value:-event.value)-100*64);
		if(event.number==1)
			last_left=max(-1000.,min(1000.,-abb/(uncapped?25:50)));
		else if(event.number==4)
			last_right=max(-1000.,min(1000.,-abb/(uncapped?25:50)));
      }
    }
    printf("!G 1 %d_!G 2 %d_\n",last_left,last_right);
  }
}
