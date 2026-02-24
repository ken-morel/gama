import { sveltekit } from '@sveltejs/kit/vite';
import kitDocs from '@svelteness/kit-docs/node';
import icons from 'unplugin-icons/vite';
import { defineConfig } from 'vite';

export default defineConfig({
  plugins: [icons({ compiler: 'svelte' }), kitDocs(), sveltekit()],
  server: {
    watch: {
      usePolling: true,
    },
    open: true,
    cors: true,
    fs: {
      strict: false,
    },
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp',
    },
    allowedHosts: ['jealomy', 'jealomy.local', 'localhost', 'gama.rbs.cm'],
  },
});
