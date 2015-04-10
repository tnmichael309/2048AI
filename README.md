# 2048AI

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

Performances: (1000 games)
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
