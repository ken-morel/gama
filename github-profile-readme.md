# 👋 Mbolo — I'm Ama (Ken Morel)

> Junior System Developer • Data Science student  
> First-year Software Engineering student | Building systems, UIs, and game tooling

[![Current Project: Gama](https://img.shields.io/badge/project-Gama-blue?logo=github)](https://github.com/ken-morel/gama)
[![MIT License](https://img.shields.io/badge/license-MIT-green.svg)]

---

Hi — I’m Ken. I build small, readable systems and developer tools that teach and ship. I love Julia for data work, V and C for systems-level control, and WebAssembly + TypeScript for making native things run in the browser. When I’m not coding I’m exploring new editors (Helix is my daily driver), designing tiny APIs, or iterating on game engine ideas.

Location: Yaoundé, Cameroon — open to collaborations and mentoring conversations.

---

## What I do now
- Lead development on **Gama**, a lightweight educational C game engine (native + WASM).
- Build reactive UI tooling and templating in Julia (Efus.jl, Ionic.jl).
- Experiment with systems tooling in V, C, and Rust-backed helpers (like rrr).
- Study Data Science in university and apply Julia to real examples.

---

## Top skills & tools
(ordered by priority)
1. Julia
2. V
3. C
4. Helix (editor)
5. WebAssembly
6. TypeScript

Other tools I use: Zig, TCC, GitHub Actions, SvelteKit

[![My Skills](https://skillicons.dev/icons?perline=7&i=julia,v,c,helix,wasm,typescript,github)](https://github.com/ken-morel)

---

## Featured projects

- Gama — A simple C game engine  
  https://github.com/ken-morel/gama  
  Lightweight, educational engine focused on a small C API, stack-first memory, immediate-mode 2D rendering, and easy WebAssembly builds.

- Efus.jl — Declarative UI templating for Julia  
  https://github.com/ken-morel/Efus.jl  
  A pug-like declarative templating macro that expands to Julian code for building UI components — great for composing Julia UIs.

- Ionic.jl — Reactive primitives for Julia UIs  
  https://github.com/ken-morel/Ionic.jl  
  Provides subscribable values, computed values, and subscription managers used by Efus.jl to build reactive interfaces with tracing and lifecycle helpers.

- rrr — Remote REPL Runner (Rust)  
  https://github.com/ken-morel/rrr  
  Small remote REPL runner that spawns processes and sends/receives commands via tiny shell-friendly tools — useful for automation and remote development scripts.

(See my pinned repositories for runnable examples and demos.)

---

## Quick start — try Gama
Create and run a new Gama project:
```bash
gama new my_first_game
cd my_first_game
gama dev
```

Tiny Gama snippet:
```c
#include <gama.h>

int main() {
  gm_init(600, 400, "Hello Gama!");
  gm_background(GM_DARKSLATEGRAY);

  while (gm_yield()) {
    gm_draw_rectangle(0, 0, 0.5, 0.3, GM_RED);
    gm_draw_text(0, 0, "Hello, World!", "default-ui", 0.1, GM_WHITE);
  }
  return 0;
}
```

---

## How I like to collaborate
- Small focused PRs or issues with a clear reproduction are easiest for me to review.
- I enjoy mentoring students and pairing on projects that teach a new concept.
- Open to contributions on Gama, Efus.jl, Ionic.jl — start with an issue or a small enhancement.

---

## Contact & links
- GitHub: [ken-morel](https://github.com/ken-morel)  
- LinkedIn: [Ken Morel](https://www.linkedin.com/in/ken-morel-8ba00a296)  
- X (Twitter): [@kenmorel8](https://twitter.com/kenmorel8)  
- Discord: kenmorel  
- Email: engonken8@gmail.com

---

## A few more things
- Tone: I keep things a mix of casual and professional — clear, friendly, and focused on learning.
- Education: First-year Software Engineering student (Level 1) — I apply coursework directly into small OSS projects.
- Fun fact: I’m a big fan of exploring new programming languages and editing ergonomics — I’ll often prototype a tiny language or macro to make my workflow simpler.

---

If you want this README tweaked (shorter, more formal, bilingual, or with custom badges/images), tell me what to change and I’ll update it. I can also generate a version tailored for recruiters, students, or collaborators.