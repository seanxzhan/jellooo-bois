# jellooo-bois

Note: In ShapesScene.cpp, in the function setSkyboxUniforms(), you should
change the **absolute** paths appropriately.

Added 
- tick function called m_fps times per second (specified in SupportCanvas3D)
- JelloCube and JelloUtil holds main code functionality.  
    - JelloCube: calculateNormals will calculate normals for arbitrary cube face positions and loadVAO
    will prepare m_vertexData for initializeOpenGLShapeProperties() like in shapes code
    - Every call to tick involves a call to rk4 (in JelloUtil) which estimates next cube positions by 
    solving the ODE for position with current acceleration and velocity 
    - Acceleration is computed using Hooke's law between vertices 
- Environment mapping and glass shader added to Jello Cube 
- GUI updates
    - View cube as either similar to shapes code visuals, glass shader, or with only springs shown
    - Change physics constants 
        - kElastic and dElastic refers to Hooke's constant and dampening factor for inter cube connections 
        - kCollision and dCollision refer to Hooke's constant and dampening factor for bounding box/plane collisions 
    - Turn environment mapping on/off

Allocation of work

Adrian: Physics, tick functionality, tesselating faces and calculating normals 

Sean: Spring mass cube visualization, GUI, shaders, GL stuff

Marc: Environment mapping, glass shaders, plane intersection

Luca: Physics, GUI, tried multithreading (somehow slower so not included)

DEMO Video: https://www.youtube.com/watch?app=desktop&v=1JgazX0uai4&feature=youtu.be
