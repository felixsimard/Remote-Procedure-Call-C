## ECSE 457 / COMP 310 - Operating Systems
### Assignment 1 - RPC - Felix Simard (260865674)
#####

###### Running the code
Here are the instructions to run my assignment.

Connecting to *lab2-4.cs.mcgill.ca* (the server I used throughout the assignment):

    ssh cs-username@lab2-4.cs.mcgill.ca

Running the **backend**:

    cd ~/path/to/my/assignment
    cd a1
    make rpc
    ./backend     // will default to 0.0.0.0 11657
    (or)
    ./backend <HOST> <PORT>


Running the **frontend** (in an new terminal):

    cd ~/path/to/my/assignment
    cd a1  
    ./frontend     // will default to 0.0.0.0 11657
    (or)
    ./frontend <HOST> <PORT>

##### Note on *exit* and *quit*
As mentioned in the "Difficulties Encountered" section of this README, there was so major confusion regarding the behavior of the *exit* and *quit* commands.
Here is how I implemented both:
- ***exit***: Terminate the client automatically, keep backend alive and open to accept new connections.
- ***quit***: Terminate the client, send a shutdown request to the backend. The backend will do the same thing as what's described in the instruction of 'shutdown' in the assignment PDF.


##### Difficulties Encountered

- The MIMI server was not allowing me to connect my frontend to a running backend (Failed to create server error). I tried to look into the Discussion forum threads to find answers, emailed TAs, but no success to fix my MIMI server. Hence, the *lab2-4.cs.mcgill.ca* server was used to complete the assignment.
- There was some confusion about the functionalities that "exit", "quit", and "shutdown" should have. I did my best to follow the grading rubric, the assignment PDF and the answered threads on the forum.
