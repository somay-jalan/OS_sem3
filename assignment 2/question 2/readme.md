The code uses the basic scheduling policies taught in class Default, Round RObin and FIFO. A for loop runs so that we can make 3 child processes. in each iteration we do a fork . 
Since we need 3 different policilies conditional statements are used to implement the policiy we need for each fork. FIrst its default then rr and lastly fifo.
INside it we make struct parameter pointer which is needed as an argument for set scheduler call that sets the policy we want. Execl executes out counting file.
we take pid of the process we have to catch its time when it ends at waitpid returns value when process ends
We catch time at start and end and subtract to get final time.