<script>
  import { onMount } from "svelte";

  // Animation state
  let heroVisible = false;
  let featuresVisible = false;
  let principlesVisible = false;
  let applicationsVisible = false;
  let ctaVisible = false;

  onMount(() => {
    // Simple fade-in animations
    setTimeout(() => {
      heroVisible = true;
    }, 100);
    setTimeout(() => {
      featuresVisible = true;
    }, 600);
    setTimeout(() => {
      principlesVisible = true;
    }, 1000);
    setTimeout(() => {
      applicationsVisible = true;
    }, 1200);
    setTimeout(() => {
      ctaVisible = true;
    }, 1400);
  });

  $: heroClasses = `hero-content ${heroVisible ? "show" : ""}`;
  $: featureCards = features.map((_, i) =>
    featuresVisible ? "feature-card show" : "feature-card",
  );
  $: principleCards = principles.map((_, i) =>
    principlesVisible ? "principle-card show" : "principle-card",
  );
  $: applicationCards = applications.map((_, i) =>
    applicationsVisible ? "application-card show" : "application-card",
  );
  $: ctaClasses = `cta-content ${ctaVisible ? "show" : ""}`;

  // Features data expanded with more specific Gama features
  const features = [
    {
      title: "Lightweight C Library",
      description:
        "Simple, easy-to-learn C API with clear function names designed specifically for students and beginners.",
      icon: "⚙️",
    },
    {
      title: "Built-in 2D Physics Engine",
      description:
        "Physics system with bodies, shapes, and collision detection supporting restitution, friction, and constraints.",
      icon: "⚖️",
    },
    {
      title: "Immediate Mode Rendering",
      description:
        "Draw shapes directly without creating objects. Reduces pointer usage and manual memory management.",
      icon: "🎯",
    },
    {
      title: "Comprehensive Animation System",
      description:
        "Multiple easing functions and wave animations for smooth, professional-looking motion.",
      icon: "⏱️",
    },
    {
      title: "Modern CLI Tool",
      description:
        "Fast project manager written in V with zero-dependency setup using embedded TCC compiler.",
      icon: "🔧",
    },
    {
      title: "Advanced Input System",
      description:
        "Intuitive key and mouse input with predefined shortcuts for arrows, space, and mouse buttons.",
      icon: "🎮",
    },
    {
      title: "Cross-Platform Development",
      description:
        "Target Windows, Linux, and Web with consistent APIs and behavior.",
      icon: "🌐",
    },
    {
      title: "Educational Focus",
      description:
        "Designed specifically for teaching C programming with safety and simplicity in mind.",
      icon: "🎓",
    },
  ];

  // Principles data
  const principles = [
    {
      title: "Stack More, Heap Less",
      description:
        "Gama encourages stack allocation in functions for safer memory management, reducing common C pitfalls for beginners.",
    },
    {
      title: "You Have Control",
      description:
        "Your code owns the mainloop, allowing you to control exactly how your game runs without hidden abstractions.",
    },
    {
      title: "No Global State",
      description:
        "Functions can serve as scenes with automatic initialization, destruction, and object management.",
    },
    {
      title: "Immediate Mode",
      description:
        "Draw shapes directly when needed rather than creating and managing objects, simplifying game logic.",
    },
  ];

  // Applications data
  const applications = [
    {
      title: "Game Development",
      description:
        "Create 2D games with physics, animation and user input handling using simple C code.",
      icon: "🎮",
    },
    {
      title: "Data Visualization",
      description:
        "Build interactive tools to visualize data and algorithms with animated elements.",
      icon: "📊",
    },
    {
      title: "Scientific Simulations",
      description:
        "Develop physics simulations, mathematical visualizations, and educational tools.",
      icon: "🔬",
    },
    {
      title: "Educational Tools",
      description:
        "Create interactive applications for teaching programming, math, and science concepts.",
      icon: "📚",
    },
  ];

  // Code example based on the lineup project
  const codeExample = `#include <gama.h>

int main() {
  // Initialize with window size and title
  gm_init(800, 600, "My Game");
  gm_background(GM_BLACK);

  // Create a physics body
  gmBody player = gm_rectangle_body(1.0, 400, 300, 32, 32);

  // Game loop
  do {
    // Handle input
    if (gm_key('U')) player.velocity.y += 5.0;  // Move up
    if (gm_key('D')) player.velocity.y -= 5.0;  // Move down
    if (gm_key('L')) player.velocity.x -= 5.0;  // Move left
    if (gm_key('R')) player.velocity.x += 5.0;  // Move right

    // Draw elements
    gm_draw_body(&player, GM_BLUE);

    // Check for exit condition
    if (gm_key_pressed('s', 'x')) {
      gm_quit();
    }

  } while (gm_yield());

  return 0;
}`;

  // CLI commands
  const cliCommands = [
    {
      command: "gama create",
      description: "Create a new Gama project interactively",
    },
    {
      command: "gama dev",
      description: "Build and run with auto-rebuild on changes",
    },
    {
      command: "gama build",
      description: "Compile your project to an executable",
    },
    {
      command: "gama package",
      description: "Package your project for distribution",
    },
  ];
</script>

<div class="hero">
  <div class="content-wrapper">
    <div class={heroClasses}>
      <div class="logo-container">
        <img src="/gama-text.png" alt="Gama Logo" />
      </div>
      <h1>Gama Engine - Simple 2D Game Development in C</h1>
      <p class="subtitle">
        A lightweight, minimalist game engine that combines a powerful C library
        with a modern toolchain. Perfect for students learning C programming and
        developers who want a zero-dependency, educational approach to game
        development.
      </p>
      <div class="buttons-container">
        <a href="/docs" class="button">Get Started</a>
        <a href="/docs/cli" class="button button-secondary">CLI Guide</a>
      </div>
    </div>
  </div>
</div>

<div class="features">
  <div class="content-wrapper">
    <div class="section-header">
      <h2 class="section-title">Powerful Features</h2>
      <p class="section-subtitle">
        Everything you need to build impressive 2D games and visualizations
      </p>
    </div>
    <div class="features-grid">
      {#each features as feature, i}
        <div class={featureCards[i]}>
          <span class="feature-icon">{feature.icon}</span>
          <h3 class="feature-title">{feature.title}</h3>
          <p class="feature-description">{feature.description}</p>
        </div>
      {/each}
    </div>
  </div>
</div>

<div class="principles">
  <div class="container">
    <div class="section-header">
      <h2 class="section-title">Design Principles</h2>
      <p class="section-subtitle">
        Built with educational excellence and practical safety at its core
      </p>
    </div>
    <div class="principles-grid">
      {#each principles as principle, i}
        <div class="principle-item">
          {#if i % 2 === 0}
            <!-- Left side: Icon/Title -->
            <div class="principle-content">
              <div class="principle-icon">
                {#if i == 0}
                  🏗️
                {/if}
                {#if i == 1}
                  ⚙️
                {/if}
                {#if i == 2}
                  🔄
                {/if}
                {#if i == 3}
                  ⚡
                {/if}
              </div>
              <h3 class="principle-title">{principle.title}</h3>
            </div>
            <!-- Right side: Card with description -->
            <div class={principleCards[i]}>
              <p class="principle-description">{principle.description}</p>
            </div>
          {:else}
            <!-- Left side: Card with description -->
            <div class={principleCards[i]}>
              <p class="principle-description">{principle.description}</p>
            </div>
            <!-- Right side: Icon/Title -->
            <div class="principle-content">
              <div class="principle-icon">
                {#if i == 0}
                  🏗️
                {/if}
                {#if i == 1}
                  ⚙️
                {/if}
                {#if i == 2}
                  🔄
                {/if}
                {#if i == 3}
                  ⚡
                {/if}
              </div>
              <h3 class="principle-title">{principle.title}</h3>
            </div>
          {/if}
        </div>
      {/each}
    </div>
  </div>
</div>

<div class="applications">
  <div class="container">
    <div class="applications-container">
      <div class="section-header">
        <h2 class="section-title">Real-World Applications</h2>
        <p class="section-subtitle">
          Gama is not just for games! Perfect for education, visualization, and
          scientific applications
        </p>
      </div>
      <div class="applications-grid">
        {#each applications as app, i}
          <div class={applicationCards[i]}>
            <span class="application-icon">{app.icon}</span>
            <h3 class="application-title">{app.title}</h3>
            <p class="application-description">{app.description}</p>
          </div>
        {/each}
      </div>
    </div>
  </div>
</div>

<div class="code-section">
  <div class="container">
    <div class="code-header">
      <h2 class="code-title">Clean & Readable Code</h2>
      <p class="code-subtitle">
        Build games with minimal, easy-to-understand C code
      </p>
    </div>
    <div class="code-container">
      <div class="code-header-tabs">
        <div class="tab-dot"></div>
        <div class="tab-dot"></div>
        <div class="tab-dot"></div>
      </div>
      <pre class="code-block"><code
          >{`// Interactive physics example with Gama
#include <gama.h>

int main() {
  // Initialize with window size and title
  gm_init(800, 600, "My Game");
  gm_background(GM_BLACK);

  // Create a physics body
  gmBody player = gm_rectangle_body(1.0, 400, 300, 32, 32);

  // Game loop
  do {
    // Handle input
    if (gm_key('U')) player.velocity.y += 5.0;  // Move up
    if (gm_key('D')) player.velocity.y -= 5.0;  // Move down
    if (gm_key('L')) player.velocity.x -= 5.0;  // Move left
    if (gm_key('R')) player.velocity.x += 5.0;  // Move right

    // Draw elements
    gm_draw_body(&player, GM_BLUE);

    // Check for exit condition
    if (gm_key_pressed('s', 'x')) {
      gm_quit();
    }

  } while (gm_yield());

  return 0;
}`}</code
        ></pre>
    </div>
  </div>
</div>

<div class="cli-section">
  <div class="container">
    <div class="cli-container">
      <div class="section-header">
        <h2 class="section-title">Powerful CLI Tool</h2>
        <p class="section-subtitle">
          Fast project management with zero dependencies
        </p>
      </div>
      <div class="cli-commands">
        {#each cliCommands as cmd, i}
          <div class="cli-command">
            <span class="cli-command-text">{cmd.command}</span>
            <span class="cli-description">{cmd.description}</span>
          </div>
        {/each}
      </div>
    </div>
  </div>
</div>

<div class="cta">
  <div class="container">
    <div class={ctaClasses}>
      <h2 class="cta-title">Ready to Start Building?</h2>
      <p class="cta-subtitle">
        Join thousands of students learning game development with Gama's
        beginner-friendly approach
      </p>
      <div class="buttons-container">
        <a href="/docs" class="button">Start Learning</a>
        <a
          href="https://github.com/ken-morel/gama"
          class="button button-secondary">View on GitHub</a
        >
      </div>
    </div>
  </div>
</div>

<div class="footer">
  <div class="container">
    <p>Gama Engine - A C toolkit for safe and simple 2D game development</p>
    <p>Website: <a href="https://gama.rbs.cm">https://gama.rbs.cm</a></p>
  </div>
</div>

<style>
  :global(body) {
    margin: 0;
    padding: 0;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
      Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
    background-color: #f8fafc;
  }

  .container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 1.5rem;
  }

  .hero {
    padding: 4rem 0 2rem;
    text-align: center;
    background: linear-gradient(135deg, #f0f9ff 0%, #e0f2fe 100%);
    border-bottom: 1px solid #e2e8f0;
  }

  .hero-content {
    opacity: 0;
    transform: translateY(20px);
    transition:
      opacity 0.8s ease,
      transform 0.8s ease;
  }

  .hero-content.show {
    opacity: 1;
    transform: translateY(0);
  }

  h1 {
    font-size: 3.5rem;
    margin: 0 0 1rem;
    font-weight: 700;
    color: #0f172a;
    line-height: 1.1;
  }

  .subtitle {
    font-size: 1.5rem;
    color: #475569;
    margin-bottom: 2rem;
    max-width: 700px;
    margin-left: auto;
    margin-right: auto;
    line-height: 1.6;
  }

  .logo-container {
    margin: 0 auto 2rem;
    width: 180px;
    height: 180px;
  }

  .logo-container img {
    width: 100%;
    height: 100%;
    object-fit: contain;
  }

  .features {
    padding: 4rem 0;
    background-color: #f8fafc;
  }

  .section-header {
    text-align: center;
    margin-bottom: 3rem;
  }

  .section-title {
    font-size: 2.5rem;
    color: #0f172a;
    margin: 0 0 1rem;
  }

  .section-subtitle {
    font-size: 1.25rem;
    color: #64748b;
    max-width: 600px;
    margin: 0 auto;
  }

  .features-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    margin-top: 2rem;
    max-width: 1200px;
    margin-left: auto;
    margin-right: auto;
    padding: 0 1.5rem;
  }

  .feature-card {
    background: white;
    border-radius: 1rem;
    padding: 2rem;
    box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.1);
    transition:
      transform 0.3s ease,
      box-shadow 0.3s ease;
    opacity: 0;
    transform: translateY(20px);
    transition:
      opacity 0.6s ease,
      transform 0.6s ease;
  }

  .feature-card.show {
    opacity: 1;
    transform: translateY(0);
  }

  .feature-card:nth-child(2n) {
    transition-delay: 0.1s;
  }

  .feature-card:nth-child(3n) {
    transition-delay: 0.2s;
  }

  .feature-card:hover {
    transform: translateY(-5px);
    box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.1);
  }

  .feature-icon {
    font-size: 3rem;
    margin-bottom: 1rem;
    display: block;
  }

  .feature-title {
    font-size: 1.5rem;
    color: #0f172a;
    margin: 0 0 1rem;
  }

  .feature-description {
    color: #64748b;
    margin: 0;
    line-height: 1.6;
  }

  .principles {
    padding: 4rem 0;
    background-color: #f1f5f9;
  }

  .principles .container {
    max-width: 100%;
    padding: 0 1.5rem;
  }

  .principles-container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 1.5rem;
  }

  .principles-grid {
    display: flex;
    flex-direction: column;
    gap: 4rem;
    margin-top: 3rem;
  }

  .principle-item {
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  .principle-item:nth-child(odd) {
    justify-content: flex-start;
  }

  .principle-item:nth-child(even) {
    justify-content: flex-end;
  }

  .principle-card {
    background: rgba(255, 255, 255, 0.15);
    backdrop-filter: blur(10px);
    border-radius: 1rem;
    padding: 2rem;
    opacity: 0;
    transform: translateX(-20px);
    transition:
      opacity 0.6s ease,
      transform 0.6s ease;
    max-width: 500px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  }

  .principle-card.show {
    opacity: 1;
    transform: translateX(0);
  }

  .principle-item:nth-child(even) .principle-card {
    transform: translateX(20px);
  }

  .principle-item:nth-child(even) .principle-card.show {
    transform: translateX(0);
  }

  .principle-content {
    flex: 1;
    padding: 0 2rem;
  }

  .principle-icon {
    font-size: 4rem;
    margin-bottom: 1rem;
  }

  .principle-title {
    font-size: 1.75rem;
    color: #0f172a;
    margin: 0 0 1rem;
  }

  .principle-description {
    color: #64748b;
    margin: 0;
    line-height: 1.6;
  }

  .applications {
    padding: 4rem 0;
    background: white;
  }

  .applications-container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 1.5rem;
  }

  .applications-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
    gap: 2rem;
    margin-top: 3rem;
  }

  .application-card {
    background: #f8fafc;
    border-radius: 1rem;
    padding: 2rem;
    text-align: center;
    opacity: 0;
    transform: translateY(20px);
    transition:
      opacity 0.6s ease,
      transform 0.6s ease;
  }

  .application-card.show {
    opacity: 1;
    transform: translateY(0);
  }

  .application-icon {
    font-size: 3rem;
    margin-bottom: 1rem;
    display: block;
  }

  .application-title {
    font-size: 1.5rem;
    color: #0f172a;
    margin: 0 0 1rem;
  }

  .application-description {
    color: #64748b;
    margin: 0;
    line-height: 1.6;
  }

  .code-section {
    padding: 4rem 0;
    background: #0f172a;
    color: white;
  }

  .code-header {
    text-align: center;
    margin-bottom: 2rem;
  }

  .code-title {
    font-size: 2.5rem;
    color: white;
    margin: 0 0 1rem;
  }

  .code-subtitle {
    font-size: 1.25rem;
    color: #94a3b8;
    max-width: 600px;
    margin: 0 auto;
  }

  .code-container {
    max-width: 800px;
    margin: 0 auto;
    background: #1e293b;
    border-radius: 0.75rem;
    overflow: hidden;
    box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.2);
  }

  .code-header-tabs {
    background: #334155;
    padding: 0.75rem 1rem;
    display: flex;
    gap: 0.5rem;
  }

  .tab-dot {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    background: #f59e0b;
  }

  .code-block {
    padding: 1.5rem;
    font-family: "Fira Code", "Monaco", "Consolas", monospace;
    font-size: 0.9rem;
    line-height: 1.6;
    overflow-x: auto;
    color: #cbd5e1;
    margin: 0;
  }

  .code-block code {
    display: block;
    white-space: pre;
  }

  .code-block .highlight-comment {
    color: #94a3b8;
  }

  .code-block .highlight-function {
    color: #60a5fa;
  }

  .code-block .highlight {
    color: #34d399;
  }

  .cli-section {
    padding: 4rem 0;
    background: white;
  }

  .cli-container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 0 1.5rem;
  }

  .cli-commands {
    display: flex;
    flex-direction: column;
    gap: 1rem;
    margin-top: 2rem;
  }

  .cli-command {
    background: #f1f5f9;
    border-radius: 0.5rem;
    padding: 1.5rem;
    display: flex;
    justify-content: space-between;
    align-items: center;
  }

  .cli-command-text {
    font-family: "Fira Code", "Monaco", "Consolas", monospace;
    font-size: 1.1rem;
    color: #0f172a;
    font-weight: 600;
    background: #e2e8f0;
    padding: 0.5rem 1rem;
    border-radius: 0.375rem;
  }

  .cli-description {
    color: #64748b;
    margin-left: 1rem;
  }

  .cta {
    padding: 4rem 0;
    text-align: center;
    background: linear-gradient(135deg, #dbeafe 0%, #e0f2fe 100%);
  }

  .cta-content {
    max-width: 600px;
    margin: 0 auto;
    opacity: 0;
    transform: translateY(20px);
    transition:
      opacity 0.8s ease,
      transform 0.8s ease;
  }

  .cta-content.show {
    opacity: 1;
    transform: translateY(0);
  }

  .cta-title {
    font-size: 2.5rem;
    color: #0f172a;
    margin: 0 0 1rem;
  }

  .cta-subtitle {
    font-size: 1.25rem;
    color: #64748b;
    margin-bottom: 2rem;
  }

  .button {
    display: inline-block;
    background: #3b82f6;
    color: white;
    padding: 1rem 2rem;
    border-radius: 0.5rem;
    text-decoration: none;
    font-weight: 600;
    font-size: 1.1rem;
    transition:
      background-color 0.3s ease,
      transform 0.2s ease;
    border: none;
    cursor: pointer;
  }

  .button:hover {
    background: #2563eb;
    transform: translateY(-2px);
  }

  .button-secondary {
    background: transparent;
    color: #3b82f6;
    border: 2px solid #3b82f6;
    margin-left: 1rem;
  }

  .button-secondary:hover {
    background: #eff6ff;
  }

  .buttons-container {
    display: flex;
    justify-content: center;
    gap: 1rem;
    margin-top: 1.5rem;
  }

  .footer {
    padding: 2rem 0;
    text-align: center;
    color: #64748b;
    border-top: 1px solid #e2e8f0;
    margin-top: 2rem;
  }

  /* Dark mode styles */
  :global(.dark) .hero,
  :global(.dark) .cta {
    background: linear-gradient(135deg, #1e293b 0%, #0f172a 100%);
  }

  :global(.dark) .hero h1,
  :global(.dark) .hero .subtitle,
  :global(.dark) .section-title,
  :global(.dark) .section-subtitle {
    color: #f1f5f9;
  }

  :global(.dark) .features {
    background-color: #0f172a;
  }

  :global(.dark) .feature-card {
    background: #1e293b;
    color: #e2e8f0;
  }

  :global(.dark) .feature-title {
    color: #f1f5f9;
  }

  :global(.dark) .feature-description {
    color: #cbd5e1;
  }

  :global(.dark) .applications {
    background-color: #0f172a;
  }

  :global(.dark) .application-card {
    background: #1e293b;
    color: #e2e8f0;
  }

  :global(.dark) .application-title {
    color: #f1f5f9;
  }

  :global(.dark) .application-description {
    color: #cbd5e1;
  }

  :global(.dark) .principles {
    background-color: #0c1a2b;
  }

  :global(.dark) .principle-card {
    background: rgba(30, 41, 59, 0.3);
    border: 1px solid rgba(255, 255, 255, 0.1);
  }

  :global(.dark) .principle-title {
    color: #f1f5f9;
  }

  :global(.dark) .principle-description {
    color: #cbd5e1;
  }

  :global(.dark) .cli-section {
    background-color: #0f172a;
  }

  :global(.dark) .cli-command {
    background: #1e293b;
    color: #e2e8f0;
  }

  :global(.dark) .cli-command-text {
    color: #f1f5f9;
    background: #334155;
  }

  :global(.dark) .cli-description {
    color: #94a3b8;
  }

  :global(.dark) .button {
    background: #2563eb;
  }

  :global(.dark) .button:hover {
    background: #3b82f6;
  }

  :global(.dark) .footer {
    color: #94a3b8;
    border-top: 1px solid #334155;
  }

  @media (max-width: 768px) {
    h1 {
      font-size: 2.5rem;
    }

    .subtitle {
      font-size: 1.25rem;
    }

    .section-title {
      font-size: 2rem;
    }

    .logo-container {
      width: 140px;
      height: 140px;
    }

    .buttons-container {
      flex-direction: column;
      align-items: center;
    }

    .button-secondary {
      margin-left: 0;
      margin-top: 1rem;
    }

    .features-grid {
      grid-template-columns: 1fr;
    }

    .cli-command {
      flex-direction: column;
      align-items: flex-start;
      gap: 0.5rem;
    }

    .cli-description {
      margin-left: 0;
    }

    .principle-card {
      flex-direction: column !important;
      text-align: center;
    }

    .principle-card .title-container {
      padding-right: 0 !important;
      padding-left: 0 !important;
    }
  }
</style>
