# Packaging your app

Once you've got something convincing you may want to share your app with others, and need a something you could give them to run, a few packaging methods are available.

## Packaging as zip (linux and windows)

To package your app as a zip file, run `gama package zip`(`gama package zip -w` for wine on linux).
This creates a new zip file named with your app name and version and you can share to others.

### Installing required libraries(linux)

For windows users, that should work very easy, with windows 7, just need to unzip the folder and run the `{projectName}.exe` file. But on linux the person who runs the app should make sure to have a few libraries
installed:

```bash
sudo apt install libglfw3 libxi libxxf86vm
```

Then you should be able to run this.

> [!TODO]
> Reduce these libraries to strict necessary.

> [!WARNING]
> Running on another linux machine has not been tested yet.

## Other packaging methods

Packaging as linux `.deb` archive and windows setup using `nsis` is planned... But not yet implemented
until then hope this method is okay.
