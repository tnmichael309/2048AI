# 2048 AI

Some ugly codes of TD-Learning and Expectimax Search for game 2048. 

(Developed by K.H Yeh and I.C Wu from CGI-Lab @NCTU).

We use TD-Lambda and several features to train 2048 by self-playing.

Those features includes:

1. Number of large tiles

2. Number of pairs of merge-able tiles

3. Number of disintinct tiles

4. Number of empty tiles

5. Number of layered tiles (Twice larger or smaller than neighbors)

6. Axe-shape six-tuples

7. Rectangular six-tuples

The download link for the trained features weights for this program:

1. http://140.113.210.143/~cgilab/download/

2. http://140.113.210.143/~cgilab/download/2048%20features%20trained.rar

Performances: (1000 games)

The AI is on the website: http://2048.aigames.nctu.edu.tw/


To see the record of reaching 65536: http://2048.aigames.nctu.edu.tw/replay.php

<table>

<tr align=center> 
       <th>Metrics</th> 
       <th>Values</th> 
</tr> 
<tr align=center> 
        <td>Average</td> 
        <td>446116</td> 
</tr> 
<tr align=center> 
        <td>Max</td> 
        <td>833300</td> 
</tr>
<tr align=center> 
        <td>2048 rate</td> 
        <td>100%</td> 
</tr>
<tr align=center> 
        <td>4096 rate</td> 
        <td>99.8%</td> 
</tr>
<tr align=center> 
        <td>8192 rate</td> 
        <td>99.5%</td> 
</tr>
<tr align=center> 
        <td>16384 rate</td> 
        <td>93.6%</td> 
</tr>
<tr align=center> 
        <td>32768 rate</td> 
        <td>33.5%</td> 
</tr>
<tr align=center> 
        <td>Speed</td> 
        <td>500 moves/sec</td> 
</tr>
<tr align=center> 
        <td>Search depth</td> 
        <td>2.5 (5)</td> 
</tr>
              
</table>

The program's result along with other experiments are in the IEEE Journal Paper: http://ieeexplore.ieee.org/document/7518633/
