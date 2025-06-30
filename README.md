Code for a Vex V5 Clawbot made at a summer camp. The robot was programmed to navigate a maze.


The navigation is based on the colors of a block tower. If the tower is Blue, you would turn right, Green, turn left, and if it sees Red it does a 180. 
Additionally, April Tags are placed around the arena and spell out a secret messages. In certain special cases, they can be used for navigation commands.


If I'd had more time, I'd definitley change and add some things:

- A cheap change, but nontheless a change. I'd extend the program to deal with the second half of the overall challenge, which was retreival.
In Retreival the robot must grab a block at the end of the maze and bring it back. To solve this, I planned to write the pathway previously taken
to get to the end of the maze in reverse order. Then I'd reverse the values and have the robot follow the path backwards. Unfortunatley this was a
piece of code that got cut out due to neccesary last minute changes to the navigation code.

- Improve the color detection system. Firstly, the system for determining the color of a tower that has the same color on the bottom and top block,
is horrible and unspecific. In theory it works, as it has gone through the other options first, but the AI Vision sensor was often picking up colors
from elsewhere. While temporarily we just changed the order for the challenge, if say, the maze was larger and more complex, the robot would be
triggering movement code in the wrong order. The solution would be to add some sort of size constraint, rather than the plain if blank color object
exists (and all other options don't). For instance, a same color block tower is taller than one block alone. So you would just need to check whether
the height of the detected red object was taller than an individual block. The other solution leads us nicely into the next point.

- Utilize the color sensor in place of the AI Vision snesor for certain applications. The AI Vision sensor was picking up colors that were not
neccesary to the robot at that point in time. The Color sensor is more linear. Later in the camp, I had implemented a system to adjust the course
based on where the object was on the screen. This worked beautiffully! It turned out to be the solution the most succesful teams used as well. This
of course would eliminate the challenges based on a linear sensor, yet boost its strengths of a linear sensor. Of course the AI Vision sensor would
still be utilized for things such as the Arpil tags or recently mentioned course adjustment system.

- Finally, I'd like to point out how many if-statements I used. It was a lot. I fear that if the same methods I used for the logic were applied to a
larger system, with more variables, more possible commands, it would be to cumbersome and rigid to expand. If a person were to have to program instructions
for say, 20 colors. They  might even say thats too much, it would undoubtedly be dreadful to program and maintain. Now perhaps I'm wrong. I have yet to
apply my programming skills in a professional setting, but the programmer in me says there has to be a better was. I guess I could utilize case-statements,
but I feel as though that is no different then using if-statements. Of course, I have no proposed solution, so this could very well be an unreachable critique.


Overall, I'm not sure the camp was designed for students to be victorious. Its designed for learning, and its only over the course of two weeks. That also 
seemed to be the mindeset of the person who runs the program. However, I came so incredibly, deliciously close, it would seem, to beating the Navigation
challenge.

I may or may not return to program some of these theorized improvements, but know if I do, I am not able to test for bugs due to having no access to a VEX
Clawbot. So think of the modifications as a semi-psuedo, semi-c++ code.

Anyway, I hope you found this code informative, useful, inspiring, or of some other good.
