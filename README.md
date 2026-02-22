This project implements a dynamic food web simulation written in C, designed to model predator-prey relationships using manually managed heap-allocated data structures. The application allows users to construct,
modify, and analyze an ecosystem through a command-line interface.

How to run the project:

Step #1 (Compile the Program):

gcc main.c -o main

Step #2 (Run the Program):

./main  


Program walkthrough (with screenshots):

The user is prompted to enter the names of organisms one at a time to construct the initial food web, continuing until they type DONE to signal completion.

<img width="298" height="126" alt="Screenshot 2026-02-22 at 2 38 29 PM" src="https://github.com/user-attachments/assets/44997e28-3a10-4f7a-92e9-484afdae6914" />

The user enters pairs of organism indices to define predator-prey relationships, continuing until an invalid index is provided to end input.

<img width="258" height="136" alt="Screenshot 2026-02-22 at 2 43 54 PM" src="https://github.com/user-attachments/assets/2c39b469-c3b0-4866-b2df-893f575af598" />

After the initial food web is created, the user can interact with a menu to expand the ecosystem, add new predator-prey relationships, or remove organisms until choosing to quit the program. The screenshot below shows a new organism being added (bear), and a new predator-prey relation (bear eats fox).

<img width="340" height="286" alt="Screenshot 2026-02-22 at 2 51 05 PM" src="https://github.com/user-attachments/assets/970cc4fb-d03a-4a38-bb46-ee74423f3481" />

If d is entered, the program displays an updated analysis of the current food web and its characteristics.

<img width="233" height="407" alt="Screenshot 2026-02-22 at 3 04 15 PM" src="https://github.com/user-attachments/assets/2e24c2f4-9956-480c-b0c1-90b5e01be863" />

