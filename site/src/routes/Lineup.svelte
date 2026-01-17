<script>
  import { GamaInstance } from "gama-svelte";

  import { onMount } from "svelte";

  onMount(() => {
    // const instance = new GamaInstance();
    // instance.bind(document.getElementById("lineup"));
    // instance.bindKeyboard(document);
    // instance.setup("/lineup.wasm").then(() => {
    //   instance.start();
    // });
  });
</script>

<div id="lineup">
  <GamaInstance binary="/lineup.wasm" uuid="a3d1d4f35704-1234-f567-8901" />
</div>

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
