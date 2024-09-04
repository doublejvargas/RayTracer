# Short Dev Log

### After setting up SDL and testing pinhole camera
![ep2 testing camera](https://github.com/user-attachments/assets/f107a7f3-bf1b-4929-9580-4cc3c201120e)

### First ray-traced circle!
![ep3 first ray traced circle first look](https://github.com/user-attachments/assets/ec6c7880-f48b-45ad-bb74-a248fc807586)

### Adding simple shading to circle
![ep3 shaded circle](https://github.com/user-attachments/assets/0bdaf46c-af2a-4d74-b0d7-47297260afbd)

### Added a point light and shading to make the circle a sphere!
![ep4 first look at point light shaded sphere](https://github.com/user-attachments/assets/d45314db-b792-47e5-84fe-9a4cdb73247b)

### First look at geometric transformations
![ep5 first look at geometric trasnformations](https://github.com/user-attachments/assets/9fbd8ecb-e5a2-452e-9003-71ae29f55380)

### First look at planes ray-traced planes
![ep6 first look at planes fixed](https://github.com/user-attachments/assets/67ee474f-d28f-4e7b-9d17-e7236eb71a54)

### Adding shadows
![ep6 first look at shadows](https://github.com/user-attachments/assets/852fcd37-872d-4ab9-b2b3-e7acb7c2335a)

### Specular and diffused lighting!
![ep7 first look at specular and diffused lighting!](https://github.com/user-attachments/assets/03c9b6c5-bc20-441f-94bb-1fe8cc6075e6)

### Reflections!!!
![ep7 reflections!!!](https://github.com/user-attachments/assets/62f4cd5d-4b4b-482f-abce-038e5931ee03)

### Introduced tile-based rendering on a single thread
I am using a custom made vector and matrix class to store all data at this point, the class is not optimized
![](https://github.com/doublejvargas/RayTracer/blob/main/zgif-single%20thread%20w%20custom%20vector%20class.gif)

### Tile-based rendering on multiple threads!
Multithreaded rendering improves performance
![](https://github.com/doublejvargas/RayTracer/blob/main/zgif-multi%20thread%20w%20custom%20vectors.gif)

### Migration to glm library
glm vectors and matrices provide an impactful performance optimization to my application
![](https://github.com/doublejvargas/RayTracer/blob/main/zgif-multi%20thread%20w%20glm.gif)
