<script lang="ts">
  import Highlight from "svelte-highlight/Highlight.svelte";
  import c from "svelte-highlight/languages/c";
  import monokai from "svelte-highlight/styles/monokai";
  import Lineup from "./Lineup.svelte";

  // Using objects with id for easier icon mapping
  const features = [
    {
      id: "library",
      title: "Lightweight C Library",
      description:
        "A simple and clean C99 API designed for students and beginners, focusing on core concepts without overwhelming complexity.",
    },
    {
      id: "physics",
      title: "Built-in 2D Physics",
      description:
        "An effective physics engine is included, with support for bodies, shapes, and collision detection to easily add dynamic interactions.",
    },
    {
      id: "rendering",
      title: "Immediate Mode Rendering",
      description:
        "Draw shapes and UI widgets directly to the screen each frame, simplifying state management and making your code easier to reason about.",
    },
    {
      id: "platform",
      title: "Cross-Platform by Design",
      description:
        "Write your game once in C and build it for native desktop (Linux, Windows) or for the Web via WebAssembly, using the exact same code.",
    },
    {
      id: "animation",
      title: "Animation System",
      description:
        "A complete sprite system with support for sprite sheets, custom animation sequences, and a collection of easing functions for smooth motion.",
    },
    {
      id: "asset_loading",
      title: "Asset Loading",
      description:
        "Natively load common 3D model formats like `.obj` and `.gltf` and popular image formats for textures and sprites.",
    },
  ];

  const philosophy = [
    {
      title: "You Have Control",
      description:
        "Gama gives you full control over the main game loop. The engine provides utilities but stays out of your way, allowing you to structure your game exactly as you see fit.",
    },
    {
      title: "Stack First, Heap Less",
      description:
        "The API encourages using stack-allocated structs to leverage C's automatic memory management, reducing the cognitive load and common pitfalls of manual memory allocation.",
    },
    {
      title: "No Global State",
      description:
        "Designed to avoid global state, Gama allows you to structure your game into different scenes, each with its own state, initialization, and cleanup logic.",
    },
  ];

  const toolchain = [
    {
      title: "gama dev",
      description:
        "Instantly run your project with the fast TCC compiler and enjoy hot-reloading on every file change for a rapid development feedback loop.",
    },
    {
      title: "gama build",
      description:
        "Create optimized release builds for native or web targets using the powerful Zig C compiler for maximum performance.",
    },
    {
      title: "Zero-Setup on Windows",
      description:
        "Gama for Windows bundles pre-configured TCC and Zig compilers. No external downloads or PATH setup needed.",
    },
    {
      title: "gama bake",
      description:
        "Embed assets like images and models directly into your game's executable, simplifying distribution and improving load times.",
    },
  ];

  const codeExample = `#include <gama.h>

int main() {
  // Initialize with window size and title
  gm_init(800, 600, "My Gama Game");
  gm_background(gm_rgb(20, 20, 25)); // Dark background

  // Game loop
  do {
    // Draw a moving, purple rectangle
    gm_draw_rectangle(
      gm_sin(gm_time() * 2.0) * 0.5, // Animate x-position
      0, 0.3, 0.2, GM_rgb(170, 119, 170)
    );

  } while (gm_yield()); // Handles events, clears screen, and loops

  return 0;
}`;

  const icons = {
    library: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M14 2l4 4-9 9-4-4 9-9z"/><path d="M3 21v-4l4 4H3z"/><path d="m15 5 4 4"/></svg>`,
    physics: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="2"/><path d="M12 2v2"/><path d="M12 20v2"/><path d="m4.93 4.93 1.41 1.41"/><path d="m17.66 17.66 1.41 1.41"/><path d="M2 12h2"/><path d="M20 12h2"/><path d="m4.93 19.07 1.41-1.41"/><path d="m17.66 6.34 1.41-1.41"/></svg>`,
    rendering: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2L2 7l10 5 10-5-10-5z"/><path d="M2 17l10 5 10-5"/><path d="M2 12l10 5 10-5"/></svg>`,
    platform: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="3" width="20" height="14" rx="2" ry="2"/><line x1="8" y1="21" x2="16" y2="21"/><line x1="12" y1="17" x2="12" y2="21"/></svg>`,
    animation: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 15v-2c0-1.1.9-2 2-2h14a2 2 0 0 1 2 2v2M3 15h18V9H3v6z"/><path d="M7 12V6M17 12V6"/></svg>`,
    asset_loading: `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="7 10 12 15 17 10"/><line x1="12" y1="15" x2="12" y2="3"/></svg>`,
  };
</script>

<svelte:head>
  {@html monokai}
</svelte:head>

<div class="background-effect"></div>

<div class="content-wrapper">
  <!-- Hero Section -->
  <div class="hero">
    <img src="/gama-text.png" alt="Gama Logo" class="hero-logo" />
    <h1 class="hero-title">A Simple C Game Engine</h1>
    <p class="hero-subtitle">
      Gama provides a lightweight C library and a modern toolchain, designed to
      make 2D game development approachable and fun for students and developers.
    </p>
    <div class="hero-buttons">
      <a href="/docs/getting-started" class="button button-primary"
        >Get Started</a
      >
      <a
        href="https://github.com/ken-morel/gama"
        class="button button-secondary">View on GitHub</a
      >
    </div>
  </div>

  <!-- Core Features Section (Updated Design) -->
  <div class="section">
    <h2 class="section-title">Core Features</h2>
    <div class="features-alternating-list">
      {#each features as feature, i}
        <div class="feature-item" class:reverse={i % 2 !== 0}>
          <div class="feature-icon-container">
            <div class="feature-icon">
              {@html icons[feature.id]}
            </div>
          </div>
          <div class="feature-card">
            <h3 class="feature-title">{feature.title}</h3>
            <p class="feature-description">{feature.description}</p>
          </div>
        </div>
      {/each}
    </div>
  </div>

  <!-- Philosophy Section (Updated Design) -->
  <div class="section philosophy-section">
    <h2 class="section-title">Guiding Philosophy</h2>
    <div class="philosophy-list">
      {#each philosophy as item, i}
        <div class="philosophy-item">
          <div class="philosophy-number">
            <span>0{i + 1}</span>
          </div>
          <div class="philosophy-card">
            <h3 class="philosophy-title">{item.title}</h3>
            <p class="philosophy-description">{item.description}</p>
          </div>
        </div>
      {/each}
    </div>
  </div>

  <!-- Toolchain Section (New Superposed Design) -->
  <div class="section">
    <h2 class="section-title">A Modern Toolchain</h2>
    <div class="toolchain-grid">
      {#each toolchain as item}
        <div class="toolchain-item">
          <div class="toolchain-title-card">
            <h3 class="toolchain-title">{item.title}</h3>
          </div>
          <div class="toolchain-content-card">
            <p class="toolchain-description">{item.description}</p>
          </div>
        </div>
      {/each}
    </div>
  </div>

  <!-- Code Example Section -->
  <div class="section">
    <h2 class="section-title">Clean & Readable Code</h2>
    <div class="code-container">
      <div class="code-header-tabs">
        <div class="tab-dot"></div>
        <div class="tab-dot"></div>
        <div class="tab-dot"></div>
      </div>
      <Highlight language={c} code={codeExample} />
    </div>
  </div>

  <!-- Live Demo Section (Updated Design) -->
  <div class="section">
    <h2 class="section-title">Live WebAssembly Demo</h2>
    <p class="section-subtitle">
      Gama projects compile directly to WebAssembly. Here is a small interactive
      sample running entirely in your browser.
    </p>
    <div class="demo-container">
      <Lineup />
    </div>
  </div>
</div>

<style>
  :root {
    --bg-color: #1a1a1a;
    --tile-bg-color: rgba(35, 35, 40, 0.6);
    --text-color: #f0f0f0;
    --subtitle-color: #a0a0a0;
    --accent-color: rgb(170, 119, 170);
    --border-color: rgba(255, 255, 255, 0.1);
  }

  :global(body) {
    margin: 0;
    padding: 0;
    font-family:
      "Segoe UI",
      -apple-system,
      BlinkMacSystemFont,
      Roboto,
      Oxygen,
      Ubuntu,
      Cantarell,
      "Open Sans",
      "Helvetica Neue",
      sans-serif;
    background-color: var(--bg-color);
    color: var(--text-color);
    overflow-x: hidden;
  }

  .background-effect {
    position: fixed;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background: radial-gradient(
        circle at 20% 20%,
        var(--accent-color) 0%,
        transparent 25%
      ),
      radial-gradient(circle at 80% 70%, #0078d4 0%, transparent 25%);
    filter: blur(120px) opacity(0.2);
    z-index: -1;
  }

  .content-wrapper {
    max-width: 1200px;
    margin: 0 auto;
    padding: 2rem;
  }

  /* Sections */
  .section {
    padding: 6rem 0;
  }
  .section-title {
    font-size: 2.5rem;
    font-weight: 600;
    text-align: center;
    margin-bottom: 4rem;
  }
  .section-subtitle {
    font-size: 1.1rem;
    color: var(--subtitle-color);
    max-width: 600px;
    margin: -3rem auto 2rem;
    line-height: 1.6;
    text-align: center;
  }

  /* Hero */
  .hero {
    text-align: center;
    padding: 4rem 0;
  }
  .hero-logo {
    width: 150px;
    height: 150px;
    margin-bottom: 2rem;
  }
  .hero-title {
    font-size: 3.5rem;
    font-weight: 600;
    margin: 0 0 1rem;
  }
  .hero-subtitle {
    font-size: 1.25rem;
    color: var(--subtitle-color);
    max-width: 600px;
    margin: 0 auto 2.5rem;
    line-height: 1.6;
  }
  .hero-buttons {
    display: flex;
    justify-content: center;
    gap: 1rem;
  }
  .button {
    display: inline-block;
    padding: 0.75rem 1.5rem;
    text-decoration: none;
    font-weight: 600;
    transition: all 0.2s ease-in-out;
    border: 2px solid transparent;
  }
  .button-primary {
    background-color: var(--accent-color);
    color: white;
  }
  .button-primary:hover {
    background-color: rgb(150, 99, 150);
    box-shadow: 0 0 15px rgba(170, 119, 170, 0.5);
  }
  .button-secondary {
    color: var(--text-color);
    border-color: var(--border-color);
  }
  .button-secondary:hover {
    background-color: var(--border-color);
  }

  /* Core Features (Alternating Icon/Card with Large Glowing Icon) */
  .features-alternating-list {
    display: flex;
    flex-direction: column;
    gap: 8rem;
  }
  .feature-item {
    display: grid;
    grid-template-columns: 1fr 2fr;
    align-items: center;
    gap: 4rem;
  }
  .feature-item.reverse {
    grid-template-columns: 2fr 1fr;
  }
  .feature-item.reverse .feature-icon-container {
    order: 2;
  }
  .feature-icon-container {
    display: flex;
    justify-content: center;
    align-items: center;
    position: relative;
  }
  .feature-icon {
    color: var(--accent-color);
    position: relative;
    z-index: 2;
  }
  .feature-icon :global(svg) {
    /* Force size on the SVG element itself */
    width: 90px !important;
    height: 90px !important;
  }
  .feature-icon-container::before {
    content: "";
    position: absolute;
    width: 100%;
    height: 100%;
    background: var(--accent-color);
    filter: blur(120px); /* Increased blur for huge glow */
    opacity: 0.8;
    z-index: 1;
  }
  .feature-card {
    background-color: var(--tile-bg-color);
    border: 1px solid var(--border-color);
    padding: 2.5rem;
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
  }
  .feature-title {
    font-size: 1.75rem;
    margin: 0 0 1rem;
    font-weight: 600;
  }
  .feature-description {
    color: var(--subtitle-color);
    line-height: 1.7;
    font-size: 1.1rem;
  }

  /* Philosophy Section (Number above card, glowing, centered) */
  .philosophy-list {
    display: flex;
    flex-direction: column;
    gap: 6rem;
    max-width: 600px;
    margin: 0 auto;
    align-items: center;
  }
  .philosophy-item {
    display: flex;
    flex-direction: column;
    align-items: center;
    text-align: center;
    width: 100%;
  }
  .philosophy-number {
    width: 90px;
    height: 90px;
    display: flex;
    align-items: center;
    justify-content: center;
    background: linear-gradient(135deg, var(--accent-color), #583758);
    box-shadow: 0 0 35px rgba(170, 119, 170, 0.7);
    margin-bottom: -45px;
    position: relative;
    z-index: 2;
  }
  .philosophy-number span {
    font-size: 2.5rem;
    font-weight: 700;
    color: white;
  }
  .philosophy-card {
    width: 100%;
    padding: 5rem 2rem 2rem;
    background: var(--tile-bg-color);
    border: 1px solid var(--border-color);
    backdrop-filter: blur(20px);
    z-index: 1;
  }
  .philosophy-title {
    font-size: 1.75rem;
    margin: 0 0 1rem;
    font-weight: 600;
  }
  .philosophy-description {
    color: var(--subtitle-color);
    line-height: 1.7;
    font-size: 1.1rem;
  }

  /* Toolchain Section (New Superposed Design) */
  .toolchain-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
    gap: 4rem 2rem; /* More row gap */
    margin-top: 4rem;
  }
  .toolchain-item {
    position: relative;
  }
  .toolchain-title-card {
    position: absolute;
    top: -2rem; /* Superpose */
    left: 1rem;
    background: var(--accent-color);
    padding: 0.75rem 1.5rem;
    z-index: 2;
    box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
  }
  .toolchain-title {
    font-size: 1.25rem;
    font-weight: 600;
    color: white;
    margin: 0;
    font-family: "Fira Code", "Consolas", monospace;
  }
  .toolchain-content-card {
    background: var(--tile-bg-color);
    border: 1px solid var(--border-color);
    padding: 4rem 2rem 2rem; /* More top padding */
    backdrop-filter: blur(20px);
    height: 100%;
  }
  .toolchain-description {
    font-size: 1rem;
    color: var(--subtitle-color);
    margin: 0;
    line-height: 1.6;
  }

  /* Code & Demo Sections */
  .code-container {
    max-width: 800px;
    margin: 0 auto;
    background: #1e1e1e;
    border: 1px solid var(--border-color);
    overflow: hidden;
  }
  .demo-container {
    width: 100%;
    max-width: 100%;
    aspect-ratio: 16 / 9;
    background: #1e1e1e;
    border: 1px solid var(--border-color);
    overflow: hidden;
    display: flex;
    margin: 0 auto;
  }
  .demo-container > :global(*) {
    flex-grow: 1;
  }
  .code-header-tabs {
    background: #2d2d2d;
    padding: 0.75rem 1rem;
    display: flex;
    gap: 0.5rem;
    border-bottom: 1px solid var(--border-color);
  }
  .tab-dot {
    width: 12px;
    height: 12px;
    background: #555;
  }

  /* Universal: No border-radius */
  * {
    border-radius: 0 !important;
  }

  @media (max-width: 768px) {
    .hero-title {
      font-size: 2.5rem;
    }
    .section {
      padding: 4rem 0;
    }
    .feature-item,
    .feature-item.reverse {
      grid-template-columns: 1fr;
      text-align: center;
    }
    .feature-item.reverse .feature-icon-container {
      order: 0;
    }
    .toolchain-grid {
      grid-template-columns: 1fr;
    }
  }
</style>
