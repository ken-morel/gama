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

<canvas id="lineup" style="width: 100%;height: 100%;"></canvas>
