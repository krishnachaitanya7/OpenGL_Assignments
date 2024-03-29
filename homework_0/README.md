# Homework - 1  
## Name: Kodur Krishna Chaitanya SID: 109258186
Q1) The frame rate reported by the program when the window is different sizes, specifically 1x1, 300x300 and full screen. Explain your results.
   
Ans) When I run the gears program in my personal laptop with Intel i5 and integrated intel HD graphics with ubuntu 19.04 OS, on average below the statistics:  

| Resolution | Vertical Sync ON FPS | Vertical Sync OFF FPS |  
|:------------:|:-----:|:-----:| 
| 1 x 1      | 60   | 1500 |  
| 300 x 300  | 60  | 1375 |  
| 1366 x 768 | 60  | 570 |  



You can see with Vertical Sync On(vblank_mode=0) the FPS is always constant, but with Vertical sync off (vblank_mode=0) you get higher FPS. "vblank_mode" in intel graphics and "__GL_SYNC_TO_VBLANK" in Nvidia graphics are same.

vblank_mode=0 ignores the monitor refresh rate and refreshes at the speed of the graphics card's highest possible speed. Screen refresh rate can never achieve the speed of graphics card anyways. Even if it updates at that speed, it's just for namesake, as the monitor itself cannot show it.

When vertical sync is on, the refresh happens at the refresh rate of the monitor, which is usually lower than graphics card speed, so for all resolutions (1x1, 300x300, 1366x768), the FPS was same which is 60 FPS.

When vertical sync is off, the refresh rate happens at the speed of the graphics card, if it's 1x1 window size  the graphics card has less load, so it gives the highest FPS. When the window size increases to 300x300, the FPS reduces a little bit, and it's least in the highest window size of 1366x768. This experiment shows that FPS is inversely proportional to window size.
     
Q2) On some systems the frame rate is a small round number like 60 or 72 or 85 frames per second, and sometimes it is a large number, thousands or tens of thousands of frames per second. Explain why this occurs.  
Ans) On some systems, by default the vblank_mode=1 (vertical sync on) which gives smaller FPS when the gears program executes. 
On the other hand in some systems vblank_mode=0 (vertical sync off) by default which attributes to high FPS speeds. 
When the graphics card starts displaying the window in more speed, which is not in sync with screen refresh rate, screen tearing happens. 
Also, as the graphics card is taking a more load, it increases the power consumption, which in turn dissipates the power. 
Also, some other processes running in the OS will affect the final FPS if vertical sync is off.


Q3) Time required to complete the assignment.  
Ans) 1.5 hours.