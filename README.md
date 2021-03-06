# Terrain rendering with OpenGL

## Dependencies

Make sure you have ```opengl version>=4.5```, ```glfw``` and ```glm``` installed on you system.

## Build
### Linux
```git clone https://github.com/vigdail/opengl-terrain```

```cd opengl-terrain```

```mkdir build```

```cmake ../ && make```

## Roadmap

- [x] Perlin noise based landscape generation
- [x] Normals calculation in compute shader using heightmap texture
- [x] Heightmap generation in compute shader
- [x] ImGui integration
- [x] Procedural skybox
- [x] Rendering procedural sun (+ day/night cycle)
- [x] Water
- [x] First person camera movement
- [x] Quad tree based terrain tesselation
- [ ] Multiple texture layers
- [ ] Grass
- [ ] Lens flare effect
- [ ] Cascade shadows
