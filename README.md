# Overview

<h5>This C program emulates a server incorporating multithreading to process jobs using client and server threads. This was a project I did while completing my operating systems class. </h5>

# How To Execute
1. Navigate to the directory where the proj1.c file is located using "cd filename".

2. Type in the command "gcc -o filename filename.c" to compile the code.

3. Type in "./program <num_client_threads> <num_server_threads> <Job_Buffer_Size> <Total_Num_Jobs> <Job_Processing_Time (ns)>" 
   to execute the code. Where you replace "<word>" with an integer value as an argument.

4. If executed properly, the command line output should show 
   how many jobs each client and server thread created or processed.
   
5. To view "service.log" that the program generated, type in "vim service.log" while in the proj1 directory.

6. Inside "service.log", you should see the timestamp of job execution in ns, thread type, thread id, 
   pool buffer index where jobs were pulled from, and the assigned job id starting at 
   "0 to amount of jobs total - 1"

## Output example using 5 server and client threads, 15 job buffer size, 20 jobs total, and 1ns job processing time
![Screenshot 2024-09-19 203924](https://github.com/user-attachments/assets/64332b17-f9c1-4485-8934-87ca57dd832c)

![Screenshot 2024-09-19 204009](https://github.com/user-attachments/assets/edd12a56-3d2f-48fa-b636-75e7231f40fb)


 
