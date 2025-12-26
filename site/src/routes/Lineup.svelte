<script>
  import GamaInstance from "./gama.js";
  import { onMount } from "svelte";

  onMount(() => {
    const instance = new GamaInstance();
    instance.bind(document.getElementById("lineup"));
    instance.bindKeyboard(document);

    instance.setup("/lineup.wasm").then(() => {
      instance.start();
    });
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
