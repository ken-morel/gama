<script lang="ts">
  import Gama from "gama";
  import { onMount } from "svelte";

  onMount(async () => {
    const canvas = document.querySelector("canvas#lineup");
    const lineup = await Gama.create("/lineup.wasm", "ac35a8e00047d46679d46");
    if (canvas) lineup.attach(canvas as HTMLCanvasElement);
    lineup.bindKeyboard(document);
    await lineup.start();
  });
</script>

<canvas id="lineup"></canvas>

<style>
  @media all and (orientation: portrait) {
    #lineup {
      width: 100% !important;
    }
  }
  @media all and (orientation: landscape) {
    #lineup {
      width: 80% !important;
    }
  }
  #lineup {
    margin: auto;
  }
</style>
