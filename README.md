<p align="center">
    <h1 align="center"> Etheray </h1>
</p>
<p align="center">
    A very simple ray tracer written from scratch in C
</p>

<p>
    <img src="https://imgur.com/91NKuuZ.png" />
</p>

# Features

### File Format
The current output file format is BMP - very simple and straightforward for implementation from scratch.

### Objects Traced
Etheray supports tracing 
  - Sphere
  - Plane
  - Triangle
  
by using a combination of geometric and analytic solutions.

### Shading and Lighting
The scene renderer uses a single light source and Phong to calculate the shading and ligting model, as of materials only a Reflective material is supported as the Refractive is WIP.

Reflective depth is implemented to allow fine-tuning reflection bounces for more accurate color distribution.

### Sampling method
Etheray supports four different methods of distributing the ray inside of a single pixel
  - Centered - a single ray passing through the center of the pixel
  - Distributed - N evenly distributed rays accross the pixel
  - Jittered - TBD
  - Random - N randomly distributed rays accross the pixel





