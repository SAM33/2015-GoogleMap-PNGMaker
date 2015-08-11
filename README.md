GoogleMap-PNGMaker<br>
Author : Sam33<br>
Date   : 2015/08/11<br>
National Central University, Taiwan<br>
<br>
Compile: gcc GoogleMap_PNGMaker.c -o GoogleMap_PNGMaker<br>
Usage  : GoogleMap_PNGMaker [latitude] [longitude] [output file name]<br>
For example:<br>
./GoogleMap_PNGMaker 24.9708264 121.1882077 test.png<br>
It will create a picture called test.png that show National Central University,Taiwan<br>
<br>
Usage  : GoogleMap_PNGMaker [latitude] [longitude] [zoom] [ouput file width] [output file height] [maptype] [output file name]<br>
For example:<br>
./GoogleMap_PNGMaker 24.9708264 121.1882077 15 500 500 satellite test.png<br>
It will create a 500x500 satellite map called test.png that show National Central University, Taiwan<br>
