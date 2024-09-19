/**
 * Name: Carter, Cody
 * Email: ccar115@lsu.edu
 * Project: PA-1 (Multithreading)
 * Instructor: Feng Chen
 * Server Login: cs410314
 */
 
Instructions For Execution:

1. Navigate to the directory where the proj1.c file is located using "cd proj1".

2. Type in the command "gcc -o proj1 proj1.c" to compile the code.

3. Type in "./program <num_client_threads> <num_server_threads> <Job_Buffer_Size> <Total_Num_Jobs> <Job_Processing_Time (ns)>" 
   to execute the code. Where you replace "<word>" with an integer value as an argument.

4. If executed properly, the command line output should show 
   how many jobs each client and server thread created or processed.
   
5. To view "service.log" that the program generated, type in "vim service.log" while in the proj1 directory.

6. Inside "service.log", you should see the timestamp of job execution in ns, thread type, thread id, 
   pool buffer index where jobs were pulled from, and the assigned job id starting at 
   "0 to amount of jobs total - 1"


 