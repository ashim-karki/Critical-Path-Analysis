<h3><b>All SFML files have already been included in the folder. Just download and run to program in Visual Studio IDE.</b></h3>

<h3>Critical Path Analysis</h3>
Critical path analysis (CPA) is a: 
  - Project management technique 
  - Maps out every key task
  - It includes identifying the amount of time necessary to finish each activity 
  - The dependencies of each activity on any others
  - Then calculation of minimum time to complete a project

<h4>Concepts Used:</h4>
  - Graph data structure
  - Topological sorting
  - PERT Chart
  
![image](https://user-images.githubusercontent.com/86644466/226087684-991da9ec-d0d1-4ba7-9a09-f5468c4b3e5f.png)
 
The dependency table of the above example is:
| Task Name  | Time  | Dependency	| Last Node (y/n)	| Task                  |
| ---------- | ----- | ----------- | --------------- | --------------------- |
| A	         | 0     |	-	         | n	              | Start
| B	         | 3     |	A	         | n	              | Creating Wireframes    
| C	         | 8     |	A	         | n	              | Designing UI                 
| D	         | 15    |	B	         | n	              | Backend Development                 
| E	         | 17    |	B, C	     | n	              | Frontend Development                 
| F	         | 12    |	D, E	     | y	              | Testing and Debugging                 
| G	         | 6     |	E	         | y	              | Content Creation and Addition
| Final Node | -     |	-	         | -	              | Demo Launch

