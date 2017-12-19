# Particle system

It is really a simple one but with a very satisfied effect. :sparkles:

<img src="https://github.com/haijianliu/particle-system-tease/blob/master/Assets/Textures/particle_fire%402x.png" width="600">

## Instance

For a good result, I create this fire effect with 4 Emitters and 1 light:

- `smoke`: 20 particles.
- `spark`: 40 particles.
- `drop`: 8 particles.
- `fire`: 26 particles with uv animation.
- `light`: a real light, color of (1, 0.4, 0.1).

So there will be a totally 94 particles for each fire effect.

## Component

Like all the other game engines, the whole particle system is simply combined with two parts: Emitter and Particle.

#### Emitter

Emitter is for randomly generating Particle components. And here are some features currently supporting:

- `uv animation` when set to true, play a uv animation.
- `divide` texture divide x and y for uv animation.
- `sample time` time between each frame for uv animation.
- `max particles` max number of particles for Emitter to generate.
- `spawn time` particle spawn time.
- `life time` defined by max and min.
- `init scale` init scale. (And not support randomly generating for now)
- `init velocity` defined by horizontal and vertical random range.
- `gravity` gravity behavior.

#### Particle

Particle component is really simple. It will just do a update.

## Techniques

#### Effect render pass

For a classic way to render a particle system, there usually is a sort progress for alpha test, during which all the particles will be sorted by their z position.

```
render scene -> sort particles -> render particles
```

But by using OpenGL depth map settings, there is a more efficient way to do a particle rendering. And this is also much more controllable.

```
render scene -> copy depth -> set depth to read only ->
set blend mode to add or alpha -> render particles
```

And here is the code:

```
glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fxPass.fbo);
glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
glClear(GL_COLOR_BUFFER_BIT);
glBlendFunc(GL_ONE, GL_ONE);
glDepthMask(GL_FALSE);
fxLayer->render(camera);
```

#### Instance draw

Since there will 94 draw calls for each fire effect, so just use OpenGL instance draw function. And there will be 4 draw calls for each fire after all.

```
template <typename T> void drawInstance(std::vector<T>* instances) {
  glBindBuffer(GL_ARRAY_BUFFER, this->vboInstance);
  glBufferSubData(GL_ARRAY_BUFFER, 0, instances->size() * sizeof(T), instances->data());
  glDrawElementsInstanced(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0, instances->size());
}
```

## Implementation

Scene the whole system is based on a ECS (Entity Component System), to implement this particle system will be something like this:

```
...
GameObject* fire = new GameObject();
Emitter* fireEmitter = fire->addComponent<Emitter>();
// emitter settings
fireEmitter->material = game->resources->getMaterial("fx_fire");
fireEmitter->animation = true;
...
// create particles
fireEmitter->createParticles("particle_name", scene);
// add to current scene
scene->addGameObject("object_name"), fire);
...
```

## Assets

- Character and animation from [mixamo](https://www.mixamo.com).
