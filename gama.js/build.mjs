// gama/build.js
import { build } from 'esbuild';
import { readFile, rm } from 'fs/promises';
import path from 'path';

const outDir = path.resolve(process.cwd(), 'dist');
const workerTempFile = path.join(outDir, 'worker.tmp.js');
const outputFile = path.join(outDir, 'gama.js');

async function buildProject() {
  try {
    // Ensure dist directory exists and is clean for a fresh build
    await rm(outDir, { recursive: true, force: true });
    await (await import('fs')).mkdirSync(outDir, { recursive: true });

    console.log('Building worker code...');
    // 1. Build the worker to a temporary file to get the bundled JS
    await build({
      entryPoints: ['src/worker.ts'],
      bundle: true,
      minify: true,
      write: true, // Ensure it writes to disk
      outfile: workerTempFile,
      platform: 'browser',
      target: 'es2020',
      format: 'iife', // Immediately Invoked Function Expression for the worker
    });

    console.log('Reading worker code...');
    // 2. Read the worker code from the temporary file
    const workerCode = await readFile(workerTempFile, 'utf-8');

    console.log('Building main library...');
    // 3. Build the main library, defining 'WORKER_CODE' as the worker's source code
    await build({
      entryPoints: ['src/index.ts'],
      bundle: true,
      minify: true,
      outfile: outputFile,
      platform: 'browser',
      target: 'es2020',
      format: 'esm', // Output as ES Module
      define: {
        // Need to JSON.stringify to make it a valid string literal in the code
        'WORKER_CODE': JSON.stringify(workerCode),
      },
    });

    console.log('Cleaning up temporary worker file...');
    // 4. Clean up the temporary worker file.
    await rm(workerTempFile);

    console.log('Generating TypeScript declarations...');
    // 5. Generate .d.ts files
    const { exec } = await import('child_process');
    await new Promise((resolve, reject) => {
      exec('tsc --emitDeclarationOnly --outDir dist', (error, stdout, stderr) => {
        if (error) {
          console.error('Declaration generation failed:', stderr);
          reject(error);
        } else {
          console.log(stdout);
          resolve();
        }
      });
    });

    console.log(`Build successful! Output in ${outputFile}`);
  } catch (error) {
    console.error('Build failed:', error);
    process.exit(1);
  }
}

buildProject();
