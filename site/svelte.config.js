import adapter from "@sveltejs/adapter-vercel";

/** @type {import('@sveltejs/kit').Config} */
const config = {
  extensions: [".svelte", ".md"],

  kit: {
    adapter: adapter({
      runtime: 'nodejs20.x'
    }),
    prerender: {
      handleMissingId: "warn",
    },
    files: {
      hooks: {
        server: "./src/hook.server.ts"
      }
    }
  },
};

export default config;
