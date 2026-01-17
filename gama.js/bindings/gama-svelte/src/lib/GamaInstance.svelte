<script lang="ts">
  import { onMount } from "svelte";
  import Gama from "gama";

  let canvas: HTMLCanvasElement | null = $state(null);

  let { binary, uuid }: { binary: string; uuid: string } = $props();

  onMount(async () => {
    const gama = await Gama.create(binary, uuid);

    gama.attach(canvas!);

    await gama.start();
  });
</script>

<canvas bind:this={canvas}></canvas>
