# Learning DirectX 11

Nothing impressive, just slowly learning DirectX11.

Hopefully I'll make something that I can use as a base for future projects!

## Progress

### June 14, 2020
![June 14 2020](/images/14-06-2020.png)

### July 5, 2020
![July 5 2020](/images/06-07-2020.png)

### July 6, 2020
![Upside down texture](/images/06-07-2020-BadTexture.png)
Issues with the newly added texture loading code - it's upside down!

![Good texture](/images/06-07-2020-GoodTexture.png)
After correcting it. Need to implement mipmaps but would rather start on the camera :)

### July 9, 2020
Added position controls, using DearImGui
![Position controls](/images/09-07-2020-PositionControl.png)

More controls!
![More controls](/images/09-07-2020-RotationCameraControl.png)

### Novermber 23, 2020
Can't believe it's been 4 months!
Decided to get back into this with some refactoring so no fancy screenshots this time
- Pulled out UI rendering code into its own subsystem
- Created an `Entity` class which holds a `Material` and a `Mesh`
    - Hopefully this is the start of an actual `Scene` system
- Started work on a `Camera` system

See you in another 4 months :D
