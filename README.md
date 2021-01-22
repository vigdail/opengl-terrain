# Terrain rendering with OpenGL

## Dependencies

Make sure you have ```opengl version>=3.3```, ```glfw``` and ```glm``` installed on you system.

## Build
### Linux
```git clone https://github.com/vigdail/opengl-terrain```

```cd opengl-terrain```

```mkdir build```

```cmake ../ && make```

## Roadmap

- [x] Perlin noise based landscape generation
- [ ] Normals calculation in compute shader using heightmap texture
- [ ] Heightmap generation in compute shader
- [ ] Multiple texture layers
- [ ] First person camera movement
- [ ] Procedural skybox
- [ ] Rendering procedural sun (+ day/night cycle)
- [ ] Lens flare effect
- [ ] Grass
- [ ] Quad tree based terrain tesselation
- [ ] Water
