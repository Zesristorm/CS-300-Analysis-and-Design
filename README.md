# CS-300-Analysis-and-Design
Project Portfolio Reflection

What was the problem you were solving in the projects for this course?
I needed to design and implement a small advising tool that loads Computer Science course data, validates it, and helps academic advisors quickly find and print course information. The key requirements were to load a CSV file, store courses in a data structure, print an alphanumeric list of courses, and show an individual course’s title and prerequisites.

How did you approach the problem? Why are data structures important?
I began by writing pseudocode for each data structure (vector, hash table, and binary search tree) that described how to read the file, create course objects, validate prerequisites, and search/print results. Data structures matter because they directly affect performance and how easily I can produce the required outputs, especially printing courses in order and doing frequent lookups. Comparing the structures guided me to choose a BST for ordered output with good average-case search/insert performance.

How did you overcome roadblocks during the activities or project?
My main issues were file parsing edge cases, function signature mismatches, and ensuring prerequisite validation worked correctly. I fixed them by simplifying the CSV split function, normalizing course numbers to uppercase, checking for malformed lines, and confirming variable types and scopes. I also tested with the real CSV early to catch format assumptions.

How has your work on this project expanded your approach to designing software and developing programs?
I learned to separate concerns. Parsing and validation, storage, and presentation. Writing pseudocode first made it easier to implement and test each part independently. I also focused on choosing the structure that matched the user’s workflow.

How has your work evolved the way you write programs that are maintainable, readable, and adaptable?
I used descriptive names, small functions with single responsibilities, and inline comments for tricky parts. By isolating helpers and the BST, I can swap the data structure later without changing the menu or output logic. This makes the program easier to extend and maintain.
