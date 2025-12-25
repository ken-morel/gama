<script>
  import { GamaInstance } from "https://github.com/ken-morel/gama/blob/dev/runners/web/gama.js?raw=true";

  const instance = new GamaInstance();

  instance.bind(document.getElementById("lineup"));
  instance.bindKeyboard(document);

  instance.setup("/lineup.wasm").then(() => {
    instance.start();
  });
</script>

<canvas id="lineup" style="width: 100%;height: 100%;"></canvas>
