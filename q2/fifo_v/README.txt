**********************************
OS A3 : Q2 : With FIFO 
**********************************

In this part, I have created 2 processes P1 and P2. P1 generates an array of 50 random strings of fixed length each. P1 then sends a chunk of five consecutive elements of the array of strings to P2 along with the ID’s of the strings, where the ID is the index of the array corresponding to the string. The second program P2 accepts the received strings, and send back the highest ID received back to P1 to acknowledge the strings received. The program P2 simply prints the ID’s and the strings on the console. On receiving the acknowledged packet, P1 sends the next five strings, with the
string elements starting from the successor of the acknowledged ID.

After this, the total time of completion is calculated an printed.

Thanks!
