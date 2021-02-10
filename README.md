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

### February 9, 2021

Pretty much exactly 4 months, at least I'm consistently not consistent

At this point there's a very rudimentary scene structure. The memory management and structure of uploading constant buffers and all that is still a bit of a mess, need to work on that.

I mostly concentrated on adding some lighting.

- Moveable point light (only 1 hardcoded at the moment)
  - Renders a fancy little white square around the point for reference.
- Added a floor (currently being messed up by the extra textures - I only wrote one shader with no way of providing fewer textures :D)
- Basic ambient + diffuse + specular lighting
  - Pretty much blinn-phong-ish
- Reading AO and Metallic texture maps

That's about it, here's some fancy screenshots

First take at lighting - messed up by including the translation part of the world transform when transforming the normal vectors:
![Messed up normals](/images/2021-02-08-BadNormals.png)

The same scene after fixing that:
![Fixed normals](/images/2021-02-08-FixedNormals.png)

After adding a fancy concrete floor texture:
![Concrete floor](/images/2021-02-08-ConcreteFloor.png)

Reading in an AO texture (it applies to the floor too, oops)
![Concrete floor](/images/2021-02-09-AO.png)

Reading in a metallic texture for the specular constant
![Concrete floor](/images/2021-02-09-AOandMetallic.png)
